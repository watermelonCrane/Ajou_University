#ifndef PARCEL_H
#define PARCEL_H

#include "binder.h"
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

/*
 * MiniBinder parcel
 *
 * Small fixed-size serialization buffer used as the message format
 * between client, server, and kernel.
 *
 * - pos : current read/write cursor
 * - len : valid payload length in buf[]
 * - binder_offset : offset in buf[] where a binder object starts
 *                   (single binder per parcel in this assignment, -1 if none)
 * - buf : raw bytes written by write_* / read_* helpers
 */

#define PARCEL_DEFAULT_CAPACITY (1 << 10)
#define PARCEL_POOL_SIZE        (1 << 4)

/*
 * parcel_t
 *
 * Fixed-layout parcel used in user space.
 * Kernel side has a compatible struct (parcel_abi) with the same header
 * fields and a smaller buf[], so only the common prefix is shared.
 */
typedef struct parcel {
    size_t      pos;
    size_t      len;
    ssize_t     binder_offset;
    uint8_t     buf[PARCEL_DEFAULT_CAPACITY];
} parcel_t;

/*
 * parcel_obtain
 *
 * Acquire a parcel from an internal pool or allocate a new one.
 * Returned parcel is zero-initialized and ready for writing.
 */
parcel_t* parcel_obtain(void);

/*
 * parcel_recycle
 *
 * Reset and return a parcel to the internal pool.
 * Call after finishing use of a heap parcel obtained via parcel_obtain().
 */
void parcel_recycle(parcel_t *p);

/*
 * set_pos
 *
 * Set the current cursor position inside the parcel.
 * Does not perform bounds checking.
 */
void set_pos(parcel_t *p, size_t pos);

/*
 * write_value
 *
 * Append raw bytes to the parcel at the current cursor.
 * Advances pos and updates len accordingly.
 */
void write_value(parcel_t *p, const void *v, size_t len);

/*
 * write_token
 *
 * Serialize a service token (token_t) into the parcel.
 * Typically used for REGISTER/BIND requests.
 */
void write_token(parcel_t *p, const token_t t);

/*
 * write_binder
 *
 * Serialize a binder_t into the parcel and record its offset.
 * This assignment assumes at most one binder object per parcel.
 */
void write_binder(parcel_t *p, binder_t b);

/*
 * read_value
 *
 * Read raw bytes from the parcel at the current cursor into v.
 * Advances pos by len.
 */
void read_value(parcel_t *p, void *v, size_t len);

/*
 * read_token
 *
 * Deserialize a token_t from the current cursor position.
 */
void read_token(parcel_t *p, token_t *t);

/*
 * read_binder
 *
 * Deserialize a binder_t from the current cursor position.
 */
void read_binder(parcel_t *p, binder_t *b);

/*
 * parcel_dump
 *
 * Debug helper: print parcel metadata and a hex dump of the payload.
 */
void parcel_dump(const parcel_t *p);

#endif /* PARCEL_H */