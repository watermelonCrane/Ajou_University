#ifndef HELLO_KERNEL_ABI_H
#define HELLO_KERNEL_ABI_H

#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h> 
#endif

#define DEVICE_NAME "hello_kernel"
#define DEVICE_PATH "/dev/hello_kernel"
#define CLASS_NAME  "hello"

#define IOCTL_MAGIC 'h'

#define IOCTL_PRINT _IO(IOCTL_MAGIC, 1)
#define IOCTL_GET_ID _IOR(IOCTL_MAGIC, 2, int)
#define IOCTL_SET_ID _IOW(IOCTL_MAGIC, 3, int)
#define IOCTL_SWAP_ID _IOWR(IOCTL_MAGIC, 4, int)  // user 에게 기존 저장된 ID를 반환, user는 새 ID를 SET

#endif 