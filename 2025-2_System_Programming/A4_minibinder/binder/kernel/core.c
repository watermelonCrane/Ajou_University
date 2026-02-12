#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "../lib/abi.h" 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("System Programming TA");
MODULE_DESCRIPTION("minibinder kernel module");
MODULE_VERSION("0.1");

static dev_t         g_dev;
static struct cdev   g_cdev;
static struct class *g_class;

static u8   *g_buf         = NULL;
static size_t g_buf_size   = 0;

static const u32 SYSTEM_HANDLE = 1;

#ifndef PARCEL_DEFAULT_CAPACITY
#define PARCEL_DEFAULT_CAPACITY 64
#endif

struct parcel_abi {
    size_t  pos;
    size_t  len;
    ssize_t binder_offset;
    u8      buf[PARCEL_DEFAULT_CAPACITY];
};

#define MAX_SERVICES 16

struct svc_entry {
    char token[BINDER_TOKEN_SIZE];
    u32  handle;
};

static struct svc_entry g_svcs[MAX_SERVICES];
static u32              g_svc_count = 0;
static u32              g_next_handle = 10;  

struct mb_tx {
    u32 handle;
    u32 code;

    struct parcel_abi req;

    wait_queue_head_t wait_client;
    int replied;
    u32 reply_offset;

    int in_use;
    int delivered;
};

static struct mb_tx g_tx;
static DEFINE_MUTEX(g_tx_lock);
static DECLARE_WAIT_QUEUE_HEAD(g_wait_recv);

static int  binder_open   (struct inode *inode, struct file *file);
static int  binder_release(struct inode *inode, struct file *file);
static long binder_ioctl  (struct file *file, unsigned int cmd, unsigned long arg);
static int  binder_mmap   (struct file *file, struct vm_area_struct *vma);

static const struct file_operations binder_fops = {
    .owner          = THIS_MODULE,
    .open           = binder_open,
    .release        = binder_release,
    .unlocked_ioctl = binder_ioctl,
    .mmap           = binder_mmap,
};

static void parcel_get_token(const struct parcel_abi *p, char *out_token) {
    memset(out_token, 0, BINDER_TOKEN_SIZE);
    memcpy(out_token, p->buf, BINDER_TOKEN_SIZE - 1);
}

static long handle_register(struct binder_ipc *ipc) {
    struct parcel_abi up;
    char token[BINDER_TOKEN_SIZE];
    u32 handle;
    struct parcel_abi rep;

    if (copy_from_user(&up,
                       (void __user *)(uintptr_t)ipc->data_ptr,
                       sizeof(up)))
        return -EFAULT;

    parcel_get_token(&up, token);

    {
        int i;
        for (i = 0; i < g_svc_count; i++) {
            if (strncmp(g_svcs[i].token, token, BINDER_TOKEN_SIZE) == 0) {
                handle = g_svcs[i].handle;
                goto found;
            }
        }
    }

    if (g_svc_count >= MAX_SERVICES)
        return -ENOMEM;

    handle = g_next_handle++;
    strscpy(g_svcs[g_svc_count].token, token, BINDER_TOKEN_SIZE);
    g_svcs[g_svc_count].handle = handle;
    g_svc_count++;

found:
    memset(&rep, 0, sizeof(rep));
    rep.pos           = 0;
    rep.len           = sizeof(u32);
    rep.binder_offset = -1;
    memcpy(rep.buf, &handle, sizeof(u32));

    if (!g_buf || g_buf_size < sizeof(rep))
        return -ENOMEM;

    memcpy(g_buf, &rep, sizeof(rep));
    ipc->offset = 0;

    return 0;
}

static long handle_bind(struct binder_ipc *ipc) {
    struct parcel_abi up;
    char token[BINDER_TOKEN_SIZE];
    u32 handle = 0;
    struct parcel_abi rep;

    if (copy_from_user(&up,
                       (void __user *)(uintptr_t)ipc->data_ptr,
                       sizeof(up)))
        return -EFAULT;

    parcel_get_token(&up, token);

    {
        int i;
        for (i = 0; i < g_svc_count; i++) {
            if (strncmp(g_svcs[i].token, token, BINDER_TOKEN_SIZE) == 0) {
                handle = g_svcs[i].handle;
                break;
            }
        }
    }

    memset(&rep, 0, sizeof(rep));
    rep.pos           = 0;
    rep.len           = sizeof(u32);
    rep.binder_offset = -1;
    memcpy(rep.buf, &handle, sizeof(u32));

    if (!g_buf || g_buf_size < sizeof(rep))
        return -ENOMEM;

    memcpy(g_buf, &rep, sizeof(rep));
    ipc->offset = 0;

    return 0;
}

static long handle_unregister(struct binder_ipc *ipc) {
    struct parcel_abi up;
    u32 handle;
    int i;

    if (copy_from_user(&up,
                       (void __user *)(uintptr_t)ipc->data_ptr,
                       sizeof(up)))
        return -EFAULT;

    memcpy(&handle, up.buf, sizeof(u32));

    for (i = 0; i < g_svc_count; i++) {
        if (g_svcs[i].handle == handle) {
            g_svcs[i] = g_svcs[g_svc_count - 1];
            g_svc_count--;
            break;
        }
    }

    ipc->offset = 0;
    return 0;
}

static long handle_send_request(struct binder_ipc *ipc) {
    long ret = 0;

    if (!g_buf || g_buf_size < sizeof(struct parcel_abi))
        return -ENOMEM;

    mutex_lock(&g_tx_lock);

    if (g_tx.in_use) {
        mutex_unlock(&g_tx_lock);
        return -EBUSY;
    }

    memset(&g_tx, 0, sizeof(g_tx));
    g_tx.in_use    = 1;
    g_tx.delivered = 0;
    g_tx.handle    = ipc->handle;
    g_tx.code      = ipc->code;
    init_waitqueue_head(&g_tx.wait_client);
    g_tx.replied      = 0;
    g_tx.reply_offset = 0;

    if (copy_from_user(&g_tx.req,
                       (void __user *)(uintptr_t)ipc->data_ptr,
                       sizeof(g_tx.req))) {
        g_tx.in_use = 0;
        mutex_unlock(&g_tx_lock);
        return -EFAULT;
    }

    memcpy(g_buf, &g_tx.req, sizeof(g_tx.req));

    wake_up_interruptible(&g_wait_recv);

    mutex_unlock(&g_tx_lock);

    wait_event_interruptible(g_tx.wait_client, g_tx.replied != 0);

    mutex_lock(&g_tx_lock);
    if (!g_tx.in_use) {
        mutex_unlock(&g_tx_lock);
        return -EIO;
    }

    ipc->offset = g_tx.reply_offset;

    g_tx.in_use = 0;
    mutex_unlock(&g_tx_lock);

    return ret;
}

static long handle_send_reply(struct binder_ipc *ipc) {
    struct parcel_abi reply;

    if (!g_buf || g_buf_size < sizeof(struct parcel_abi))
        return -ENOMEM;

    mutex_lock(&g_tx_lock);
    if (!g_tx.in_use) {
        mutex_unlock(&g_tx_lock);
        return -EINVAL;
    }

    if (copy_from_user(&reply,
                       (void __user *)(uintptr_t)ipc->data_ptr,
                       sizeof(reply))) {
        mutex_unlock(&g_tx_lock);
        return -EFAULT;
    }

    if (g_buf_size < 2 * sizeof(struct parcel_abi)) {
        mutex_unlock(&g_tx_lock);
        return -ENOMEM;
    }

    g_tx.reply_offset = (u32)(g_buf_size / 2);
    memcpy(g_buf + g_tx.reply_offset, &reply, sizeof(reply));

    g_tx.replied = 1;
    wake_up_interruptible(&g_tx.wait_client);

    mutex_unlock(&g_tx_lock);
    return 0;
}

static long handle_recv(struct binder_ipc *ipc) {
    long ret = 0;

    if (!g_buf || g_buf_size < sizeof(struct parcel_abi))
        return -ENOMEM;

    ret = wait_event_interruptible(g_wait_recv,
                                   g_tx.in_use != 0 && g_tx.delivered == 0);
    if (ret)
        return ret;

    mutex_lock(&g_tx_lock);
    if (!g_tx.in_use || g_tx.delivered) {
        mutex_unlock(&g_tx_lock);
        return -EIO;
    }

    ipc->handle = g_tx.handle;
    ipc->code   = g_tx.code;
    ipc->offset = 0;

    g_tx.delivered = 1;

    mutex_unlock(&g_tx_lock);
    return ret;
}

static int binder_open(struct inode *inode, struct file *file) {
    pr_info("minibinder: device opened\n");
    return 0;
}

static int binder_release(struct inode *inode, struct file *file) {
    pr_info("minibinder: device closed\n");
    return 0;
}

static int binder_mmap(struct file *file, struct vm_area_struct *vma) {
    unsigned long size   = vma->vm_end - vma->vm_start;
    unsigned long addr   = (unsigned long)g_buf;
    unsigned long uaddr  = vma->vm_start;
    unsigned long offset = 0;
    int ret;

    if (!g_buf)
        return -ENOMEM;

    if (size > g_buf_size)
        return -EINVAL;

    vm_flags_set(vma, VM_DONTEXPAND | VM_DONTDUMP);

    while (offset < size) {
        unsigned long pfn = vmalloc_to_pfn((void *)(addr + offset));

        ret = remap_pfn_range(vma,
                              uaddr + offset,
                              pfn,
                              PAGE_SIZE,
                              vma->vm_page_prot);
        if (ret) {
            pr_err("minibinder: remap_pfn_range failed at offset=%lu\n",
                   offset);
            return -EAGAIN;
        }

        offset += PAGE_SIZE;
    }

    pr_info("minibinder: mmap %lu bytes OK\n", size);
    return 0;
}

static long binder_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct binder_ipc ipc;
    long ret = 0;

    switch (cmd) {
    case BINDER_IOC_ATTACH:
    {
        u32 __user *u_handle = (u32 __user *)arg;
        u32 h = SYSTEM_HANDLE;
        if (copy_to_user(u_handle, &h, sizeof(h)))
            return -EFAULT;
        pr_info("minibinder: ATTACH -> handle=%u\n", h);
        return 0;
    }

    case BINDER_IOC_TRANS_SEND:
        if (copy_from_user(&ipc, (void __user *)arg, sizeof(ipc)))
            return -EFAULT;
        pr_info("SEND: handle=%u code=%u\n", ipc.handle, ipc.code);

        if (ipc.handle == SYSTEM_HANDLE) {
            switch (ipc.code) {
            case TRANSACT_REGISTER:
                ret = handle_register(&ipc);
                break;
            case TRANSACT_BIND:
                ret = handle_bind(&ipc);
                break;
            case TRANSACT_UNREGISTER:
                ret = handle_unregister(&ipc);
                break;
            case TRANSACT_UNBIND:
                ipc.offset = 0;
                ret = 0;
                break;
            default:
                ret = -EINVAL;
                break;
            }

            if (ret == 0) {
                if (copy_to_user((void __user *)arg, &ipc, sizeof(ipc)))
                    return -EFAULT;
            }
            return ret;
        }

        if (!g_tx.in_use) {
            ret = handle_send_request(&ipc);
        } else {
            ret = handle_send_reply(&ipc);
        }

        if (ret == 0) {
            if (copy_to_user((void __user *)arg, &ipc, sizeof(ipc)))
                return -EFAULT;
        }
        return ret;

    case BINDER_IOC_TRANS_RECV:
    {
        memset(&ipc, 0, sizeof(ipc));

        ret = handle_recv(&ipc);
        if (ret == 0) {
            pr_info("RECV -> handle=%u code=%u offset=%u\n",
                    ipc.handle, ipc.code, ipc.offset);

            if (copy_to_user((void __user *)arg, &ipc, sizeof(ipc)))
                return -EFAULT;
        }
        return ret;
    }

    default:
        return -ENOTTY;
    }
}

static int __init binder_init(void) {
    int ret;

    ret = alloc_chrdev_region(&g_dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("minibinder: failed to alloc_chrdev_region\n");
        return ret;
    }

    cdev_init(&g_cdev, &binder_fops);
    g_cdev.owner = THIS_MODULE;

    ret = cdev_add(&g_cdev, g_dev, 1);
    if (ret < 0) {
        pr_err("minibinder: failed to cdev_add\n");
        unregister_chrdev_region(g_dev, 1);
        return ret;
    }

    g_class = class_create(CLASS_NAME);
    if (IS_ERR(g_class)) {
        pr_err("minibinder: failed to create class\n");
        cdev_del(&g_cdev);
        unregister_chrdev_region(g_dev, 1);
        return PTR_ERR(g_class);
    }

    if (IS_ERR(device_create(g_class, NULL, g_dev, NULL, DEVICE_NAME))) {
        pr_err("minibinder: failed to create device\n");
        class_destroy(g_class);
        cdev_del(&g_cdev);
        unregister_chrdev_region(g_dev, 1);
        return -EINVAL;
    }

    g_buf_size = BINDER_BUFFER_SIZE;
    g_buf = vmalloc(g_buf_size);
    if (!g_buf) {
        pr_err("minibinder: failed to vmalloc buffer\n");
        device_destroy(g_class, g_dev);
        class_destroy(g_class);
        cdev_del(&g_cdev);
        unregister_chrdev_region(g_dev, 1);
        return -ENOMEM;
    }
    memset(g_buf, 0, g_buf_size);

    memset(&g_tx, 0, sizeof(g_tx));
    mutex_init(&g_tx_lock);

    pr_info("minibinder: module loaded (major=%d, minor=%d)\n",
            MAJOR(g_dev), MINOR(g_dev));
    return 0;
}

static void __exit binder_exit(void) {
    if (g_buf)
        vfree(g_buf);

    device_destroy(g_class, g_dev);
    class_destroy(g_class);
    cdev_del(&g_cdev);
    unregister_chrdev_region(g_dev, 1);

    pr_info("minibinder: module unloaded\n");
}

module_init(binder_init);
module_exit(binder_exit);