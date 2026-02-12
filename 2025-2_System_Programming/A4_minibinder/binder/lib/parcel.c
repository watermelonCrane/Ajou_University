#include "parcel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static parcel_t *parcel_pool[PARCEL_POOL_SIZE];
static size_t    parcel_pool_size = 0;

static void parcel_init(parcel_t *p) {
    if (!p) return;
    p->pos = 0;
    p->len = 0;
    p->binder_offset = -1;
    memset(p->buf, 0, PARCEL_DEFAULT_CAPACITY);
}

parcel_t *parcel_obtain(void) {
    parcel_t *p = NULL;
    if (parcel_pool_size > 0) {
        p = parcel_pool[--parcel_pool_size];
        parcel_pool[parcel_pool_size] = NULL;
    } else {
        p = (parcel_t *)calloc(1, sizeof(parcel_t));
        parcel_init(p);
    }
    return p;
}

void parcel_recycle(parcel_t *p) {
    parcel_init(p);
    if (parcel_pool_size < PARCEL_POOL_SIZE) {
        parcel_pool[parcel_pool_size++] = p;
    } else {
        free(p);
    }
}

void set_pos(parcel_t *p, size_t pos) {
    p->pos = pos;
}

void write_value(parcel_t *p, const void *v, size_t len) {
    if (p->pos + len > PARCEL_DEFAULT_CAPACITY) {
        fprintf(stderr,
                "[parcel] write_value overflow: pos=%zu, len=%zu, cap=%d\n",
                p->pos, len, PARCEL_DEFAULT_CAPACITY);
        return;
    }
    memcpy(p->buf + p->pos, v, len);
    p->pos += len;
    if (p->pos > p->len) p->len = p->pos;
}

void write_token(parcel_t *p, const token_t t) {
    write_value(p, t, sizeof(token_t));
}

void write_binder(parcel_t *p, binder_t b) {
    if (p->binder_offset < 0) {
        p->binder_offset = (ssize_t)p->pos;
    }
    write_value(p, &b, sizeof(b));
}

void read_value(parcel_t *p, void *v, size_t len) {
    if (p->pos + len > p->len) {
        fprintf(stderr,
                "[parcel] read_value overflow: pos=%zu, len=%zu, total=%zu\n",
                p->pos, len, p->len);
        return;
    }
    memcpy(v, p->buf + p->pos, len);
    p->pos += len;
}

void read_token(parcel_t *p, token_t *t) {
    read_value(p, t, sizeof(token_t));
}

void read_binder(parcel_t *p, binder_t *b) {
    read_value(p, b, sizeof(binder_t));
}

void parcel_dump(const parcel_t *p) {
    if (!p) {
        printf("[parcel_dump] (null parcel)\n");
        printf("parcel_pool_size=%zu\n", parcel_pool_size);
        return;
    }

    printf("===== PARCEL DUMP =====\n");
    printf("pos=%zu, len=%zu\n", p->pos, p->len);

    printf("parcel_pool_size=%zu\n", parcel_pool_size);

    if (p->binder_offset >= 0) {
        printf("binder_offset=%zd\n", (ssize_t)p->binder_offset);
    } else {
        printf("binder_offset=(none)\n");
    }

    printf("data(hex): ");
    size_t dump_len = p->len < 64 ? p->len : 64; 
    for (size_t i = 0; i < dump_len; i++) {
        printf("%02X ", p->buf[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n           ");
        }
    }
    if (p->len > dump_len) {
        printf("... (%zu bytes total)\n", p->len);
    } else {
        printf("\n");
    }

    printf("========================\n");
}