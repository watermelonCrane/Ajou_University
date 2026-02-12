#ifndef BINDER_H
#define BINDER_H

#include "abi.h"
#include <stddef.h>
#include <stdint.h>

/*
 * MiniBinder user-space API.
 * - binder_t : user-visible binder handle (proxy or stub)
 * - attach_process : set up driver connection + event loop
 * - transact       : synchronous IPC using parcel_t
 */

typedef uint32_t handle_t;
typedef char     token_t[BINDER_TOKEN_SIZE];

/* forward declarations */
typedef struct parcel parcel_t;
struct binder;

/*
 * on_transact_fn
 *
 * Per-service transaction callback on the server side.
 *
 * Parameters:
 *   self  - local stub binder handling this transaction
 *   code  - transaction code (service-specific or TRANSACT_*)
 *   data  - request parcel
 *   reply - reply parcel (server fills; client will receive)
 *
 * Returns:
 *   0 on success, negative value on error.
 */

typedef int (*on_transact_fn)(struct binder *self,
                              uint32_t      code,
                              parcel_t     *data,
                              parcel_t     *reply);

/*
 * binder_t
 *
 * Lightweight binder object used both for:
 *   - proxy binder on the client (on_transact == NULL)
 *   - stub binder on the server (on_transact != NULL)
 */

typedef struct binder {
    token_t        token;
    handle_t       handle;
    on_transact_fn on_transact;  // stub 쪽에서만 유효, proxy는 NULL
} binder_t;

/*
 * str2tok
 *
 * Convert a C string into a token_t (service name).
 * Truncates to BINDER_TOKEN_SIZE-1 and ensures NUL-termination.
 */
void str2tok(const char *s, token_t *t);

/*
 * binder_init
 *
 * Initialize a binder_t with a token and a handle.
 * Does not register it with the kernel; used on both client/server.
 */
void binder_init(binder_t *b, const token_t t, handle_t h);

/*
 * free_binder
 *
 * Reset binder fields (handle, token, callback). Does not talk to kernel.
 */
void free_binder(binder_t *b);

/*
 * attach_process
 *
 * Entry wrapper for MiniBinder-aware processes.
 *
 * Does:
 *   - open /dev/minibinder
 *   - attach to kernel driver
 *   - mmap shared buffer
 *   - start binder thread + looper
 *   - post(entry, argv) and run event loop
 */
void attach_process(int (*entry)(int argc, char **argv),
                    int argc,
                    char **argv);

/*
 * detach_process
 *
 * Shutdown helper:
 *   - stop binder thread
 *   - stop looper
 *   - close driver fd
 */
void detach_process(void);

/*
 * binder_register
 *
 * Register a local stub binder with the system binder.
 *
 * On success:
 *   - kernel assigns a new handle
 *   - binder_t.handle is updated
 *   - binder is stored in local stub table (for incoming calls)
 */
int binder_register(binder_t *b);

/*
 * binder_unregister
 *
 * Unregister a previously registered local stub binder.
 * Removes it from both kernel table and local stub table.
 */
int binder_unregister(binder_t *b);

/*
 * bind
 *
 * Client-side lookup: acquire a remote proxy binder by token.
 *
 * On success:
 *   - out->handle is set to remote handle
 *   - out->token is copied from argument
 *   - out->on_transact is NULL (proxy, not stub)
 */
int bind(binder_t *out, const token_t *token);

/*
 * unbind
 *
 * Client-side release of a proxy binder handle.
 * This implementation is lightweight (no refcounting).
 */
int unbind(binder_t *b);

/*
 * transact
 *
 * Synchronous binder call.
 *
 * Parameters:
 *   b      - target binder (proxy or system binder)
 *   code   - transaction code
 *   data   - request parcel (heap parcel_obtain()'ed by caller)
 *   reply  - OUT, pointer to shm-backed reply parcel (zero-copy).
 *            For one-way calls, caller passes NULL.
 *
 * Returns:
 *   0 on success, negative value on error.
 */
int transact(binder_t b, int code, parcel_t *data, parcel_t **reply);

#endif /* BINDER_H */