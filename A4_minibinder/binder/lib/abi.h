#ifndef BINDER_ABI_H
#define BINDER_ABI_H

/*
 * MiniBinder ABI shared between user-space and kernel.
 * This header defines the minimal protocol: handles, codes, and ioctl shapes.
 */

#ifdef __KERNEL__
    #include <linux/types.h>
    #include <linux/ioctl.h>
    typedef   __u64     u64;
    typedef   __u32     u32;
    typedef   __u8      u8;
#else
    #include <stdint.h>
    #include <sys/ioctl.h>
    typedef   uint64_t  u64;
    typedef   uint32_t  u32;
    typedef   uint8_t   u8;
#endif

/* basic layout / limits shared by lib & kernel */
#define BINDER_IOC_MAGIC      'b'
#define BINDER_PAYLOAD_SIZE   2048 
#define BINDER_TOKEN_SIZE 32
#define DEVICE_NAME "minibinder"
#define DEVICE_PATH "/dev/minibinder"
#define CLASS_NAME  "minibinder_class"
#define BINDER_BUFFER_SIZE 65536
#define BINDER_PAYLOAD_SIZE 2048 

/*
 * struct binder_ipc
 *
 * Generic IPC envelope exchanged via ioctl.
 * - For SEND:
 *     data_ptr : user-space pointer to parcel_t (request or reply)
 *     handle   : target binder handle (SYSTEM_HANDLE or service handle)
 *     code     : transaction code (REGISTER/BIND/… or app-specific)
 *     offset   : (out) shm offset where kernel placed reply parcel
 *
 * - For RECV:
 *     data_ptr : unused by this assignment (kernel uses shared buffer)
 *     handle   : (out) target handle for this request
 *     code     : (out) transaction code
 *     offset   : (out) shm offset of request parcel
 */

struct binder_ipc {
    u64 data_ptr;
    u32 handle;
    u32 code;
    u32 offset;
};

/* system-level transaction codes handled by the "system binder" (handle = 1) */
#define   TRANSACT_REGISTER    1
#define   TRANSACT_UNREGISTER  2
#define   TRANSACT_BIND        3
#define   TRANSACT_UNBIND      4

/*
 * ioctl interface
 *
 *  BINDER_IOC_ATTACH
 *    - user → kernel
 *    - arg: u32* (out)
 *    - returns initial "system" binder handle
 *
 *  BINDER_IOC_TRANS_SEND
 *    - from user to kernel
 *    - arg: struct binder_ipc* (in/out)
 *    - used both for:
 *        * client request send
 *        * server reply send
 *        * system binder meta ops (REGISTER/BIND/…)
 *
 *  BINDER_IOC_TRANS_RECV
 *    - from kernel to user
 *    - arg: struct binder_ipc* (out)
 *    - server side: blocks until next request is available
 *      and fills handle/code/offset for that request
 */

#define   BINDER_IOC_ATTACH     _IOWR(BINDER_IOC_MAGIC, 0x01, u32)
#define   BINDER_IOC_TRANS_SEND _IOWR(BINDER_IOC_MAGIC, 0x02, struct binder_ipc)
#define   BINDER_IOC_TRANS_RECV _IOWR(BINDER_IOC_MAGIC, 0x03, struct binder_ipc)

#endif
