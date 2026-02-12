#include "abi.h"
#include "binder.h"
#include "looper.h"
#include "parcel.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

struct binder_driver { 
    binder_t binder;
    int fd; 
    pthread_t thread;
};

struct main_ctx {
    int (*entry)(int argc, char **argv);
    int   argc;
    char **argv;
};

struct transaction_ctx {
    binder_t  *b;
    u32        code;
    parcel_t  *data;   
    u32        handle; 
};

#define MAX_LOCAL_BINDERS   16 

static binder_t local_binders[MAX_LOCAL_BINDERS];
static size_t   local_binder_count = 0;

static uint8_t *binder_buf      = NULL;
static size_t   binder_buf_size = 0;
static struct binder_driver *driver = NULL;

static void  _free_driver(void);
static void *_binder_thread_routine(void *arg);
static void  _handle_transaction(void *arg);
static void  _init_shm(void);

void   str2tok(const char *s, token_t *t);
void   binder_init(binder_t *b, const token_t t, handle_t h);
void   free_binder(binder_t *b);

void   attach_process(int (*__main)(int argc, char **argv), int argc, char **argv);
void   detach_process(void);

int    binder_register(binder_t *b);
int    binder_unregister(binder_t *b);
int    bind(binder_t *b, const token_t *token);
int    unbind(binder_t *b);

int    transact(binder_t b, int code, parcel_t *data, parcel_t **reply);

void str2tok(const char *s, token_t* t) {
    size_t len = strlen(s);
    if (len >= BINDER_TOKEN_SIZE) len = BINDER_TOKEN_SIZE - 1;
    memset(*t, 0, BINDER_TOKEN_SIZE);
    memcpy(*t, s, len);
}

void binder_init(binder_t *b, const token_t t, handle_t h) {
    if (!b) return;
    memcpy(b->token, t, sizeof(token_t));
    b->token[BINDER_TOKEN_SIZE - 1] = '\0';
    b->handle = h;
}

void free_binder(binder_t *b) {
    if (!b) return;
    b->handle = 0;
    memset(b->token, 0, sizeof(token_t));
    b->on_transact = NULL;
}

static void _main_task(void *arg) {
    struct main_ctx *ctx = (struct main_ctx *)arg;
    if (!ctx || !ctx->entry) return;

    int ret = ctx->entry(ctx->argc, ctx->argv);
    (void)ret; 

    free(ctx);
}

static void _free_driver(void) {
    if (!driver) return;

    if (driver->thread) {
        pthread_cancel(driver->thread);
        pthread_join(driver->thread, NULL);
        driver->thread = 0;
    }

    if (binder_buf && binder_buf != MAP_FAILED) {
        munmap(binder_buf, binder_buf_size);
        binder_buf      = NULL;
        binder_buf_size = 0;
    }

    if (driver->fd >= 0) {
        close(driver->fd);
        driver->fd = -1;
    }

    driver = NULL;
}

static void _init_shm(void) {
    if (!driver) return;

    binder_buf_size = BINDER_BUFFER_SIZE;
    binder_buf = mmap(
        NULL,
        binder_buf_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        driver->fd,
        0
    );
    if (binder_buf == MAP_FAILED) {
        perror("[binder] mmap");
        binder_buf      = NULL;
        binder_buf_size = 0;
        _free_driver();
        return;
    }
}

void attach_process(int (*__main) (int argc, char **argv), int argc, char **argv) {
    _free_driver();
    static struct binder_driver d;
    memset(&d, 0, sizeof(d));

    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {}
    handle_t h;
    ioctl(fd, BINDER_IOC_ATTACH, &h);

    binder_t b;
    token_t t;
    str2tok("binder_driver", &t);
    binder_init(&b, t, h);
    d.binder = b;
    d.fd = fd;
    driver = &d;

    _init_shm();
        if (!binder_buf) {
        close(fd);
        driver = NULL;
        return;
    }

    looper_init();
    struct main_ctx *ctx = calloc(1, sizeof(struct main_ctx));
    if (!ctx) {
        fprintf(stderr, "[binder] failed to allocate main_ctx\n");
        looper_quit();
        _free_driver();
        return;
    }
    ctx->entry = __main;
    ctx->argc  = argc;
    ctx->argv  = argv;

    post(_main_task, ctx);
    int err = pthread_create(&driver->thread, NULL, _binder_thread_routine, NULL);
    if (err != 0) {
        fprintf(stderr, "[binder] pthread_create failed: %d\n", err);
    }
    looper_run();
}

void detach_process() {
    looper_quit();
    _free_driver();
}

int binder_register(binder_t *b) {
    if (!driver) return -1;
    if (local_binder_count >= MAX_LOCAL_BINDERS) return -1;

    parcel_t *data  = parcel_obtain();
    parcel_t *reply = NULL;

    write_token(data, b->token);
    int ret = transact(driver->binder, TRANSACT_REGISTER, data, &reply);
    parcel_recycle(data);
    if (ret != 0) {
        return ret;  
    }

    set_pos(reply, 0);
    handle_t h = 0;
    read_value(reply, &h, sizeof(handle_t));
    b->handle = h;

    local_binders[local_binder_count++] = *b;
    return 0;
}

int binder_unregister(binder_t *b) {
    if (!driver || !b) return -1;
    if (b->handle == 0) {
        return -1;
    }

    parcel_t *data  = parcel_obtain();

    handle_t h = b->handle;
    write_value(data, &h, sizeof(handle_t));

    int ret = transact(driver->binder, TRANSACT_UNREGISTER, data, NULL);
    parcel_recycle(data);

    if (ret != 0) {
        return ret;
    }

    for (size_t i = 0; i < local_binder_count; i++) {
        if (local_binders[i].handle == h) {
            local_binders[i] = local_binders[local_binder_count - 1];
            local_binder_count--;
            break;
        }
    }
    return 0;
}

int bind(binder_t *out, const token_t *token) {
    if (!driver || !out || !token) return -1;

    parcel_t *data  = parcel_obtain();
    parcel_t *reply = NULL;

    write_token(data, *token);

    int ret = transact(driver->binder, TRANSACT_BIND, data, &reply);
    parcel_recycle(data);

    if (ret != 0) {
        return ret;
    }

    set_pos(reply, 0);
    handle_t h = 0;
    read_value(reply, &h, sizeof(handle_t));

    if (h == 0) {
        return -1;
    }

    memcpy(out->token, *token, sizeof(token_t));
    out->token[BINDER_TOKEN_SIZE - 1] = '\0';
    out->handle      = h;
    out->on_transact = NULL;   // 클라이언트 쪽이므로 콜백은 없음
    return 0;
}

int unbind(binder_t *b) {
    if (!driver || !b) return -1;
    if (b->handle == 0) return -1;   // 이미 비어 있거나 유효하지 않음

    parcel_t *data  = parcel_obtain();

    handle_t h = b->handle;
    write_value(data, &h, sizeof(handle_t));

    int ret = transact(driver->binder, TRANSACT_UNBIND, data, NULL);
    parcel_recycle(data);

    if (ret != 0) {
        return ret;
    }

    return 0;
}

int transact(binder_t b, int code, parcel_t *data, parcel_t **reply) {
    if (!driver) return -1;

    struct binder_ipc msg;
    memset(&msg, 0, sizeof(msg));

    msg.handle   = (u32)b.handle;
    msg.code     = (u32)code;
    msg.data_ptr = (u64)(uintptr_t)data; 

    int ret = ioctl(driver->fd, BINDER_IOC_TRANS_SEND, &msg);
    if (ret < 0)
        return -1;

    if (msg.offset + sizeof(parcel_t) > binder_buf_size) {
        return -1;
    }

    parcel_t *shm_reply = (parcel_t *)(binder_buf + msg.offset);
    *reply = shm_reply;  

    return 0;
}

static void *_binder_thread_routine(void *arg) {
    if (!driver || !binder_buf || binder_buf_size < sizeof(parcel_t))
        return;

    while (1) {
        struct binder_ipc msg;
        memset(&msg, 0, sizeof(msg));

        int ret = ioctl(driver->fd, BINDER_IOC_TRANS_RECV, &msg);
        if (ret < 0) {
            continue;
        }

        if (msg.offset + sizeof(parcel_t) > binder_buf_size) {
            continue;
        }

        parcel_t *data = (parcel_t *)(binder_buf + msg.offset);

        binder_t *b = NULL;
        for (size_t i = 0; i < local_binder_count; i++) {
            if (local_binders[i].handle == msg.handle) {
                b = &local_binders[i];
                break;
            }
        }
        if (!b || !b->on_transact) {
            continue;
        }

        struct transaction_ctx *ctx = malloc(sizeof(*ctx));
        if (!ctx) {
            continue;
        }
        ctx->b      = b;
        ctx->code   = msg.code;
        ctx->data   = data;       
        ctx->handle = msg.handle;

        post(_handle_transaction, ctx);
    }
}

static void _handle_transaction(void *arg) {
    struct transaction_ctx *ctx = (struct transaction_ctx *)arg;
    if (!ctx || !ctx->b || !ctx->b->on_transact) {
        free(ctx);
        return;
    }

    parcel_t *reply = parcel_obtain();
    set_pos(reply, 0);

    ctx->b->on_transact(ctx->b, ctx->code, ctx->data, reply);

    struct binder_ipc resp;
    memset(&resp, 0, sizeof(resp));
    resp.handle   = ctx->handle;
    resp.code     = ctx->code;
    resp.data_ptr = (u64)(uintptr_t)reply; 

    (void)ioctl(driver->fd, BINDER_IOC_TRANS_SEND, &resp);

    parcel_recycle(reply); 
    free(ctx);
}