#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#include "../include/abi.h"

#define STUDENT_ID 202224210 // Don't modify

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jaehak Kim");
MODULE_DESCRIPTION("Hello Kernel ioctl example (IO/IOR/IOW/IOWR + ABI)");
MODULE_VERSION("0.1");

static dev_t _dev;
static struct cdev _cdev;
static struct class *_class;

static int  hello_open   (struct inode *inode, struct file *file);
static int  hello_release(struct inode *inode, struct file *file);
static long hello_ioctl  (struct file *file, unsigned int cmd, unsigned long arg);

static int  __init hello_init(void);
static void __exit hello_exit(void);

static const struct file_operations _fops = {
    .owner          = THIS_MODULE,
    .open           = hello_open,
    .release        = hello_release,
    .unlocked_ioctl = hello_ioctl,
};

module_init(hello_init);
module_exit(hello_exit);

static int current_id = STUDENT_ID;

static int hello_open(struct inode *inode, struct file *file) {
    pr_info("hello_kernel: device opened\n");
    return 0;
}

static int hello_release(struct inode *inode, struct file *file) {
    pr_info("hello_kernel: device closed\n");
    return 0;
}

static long hello_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {

    case IOCTL_PRINT:    /* _IO */
        // 커널 로그에 current_id 값 출력
        pr_info("hello_kernel: current_student_ID is %d", current_id);

        return 0;

    case IOCTL_GET_ID:   /* _IOR */
        // user에게 current_id값 전달
        if (copy_to_user((int __user *)arg, &current_id, sizeof(current_id))) {
            return -EFAULT;
        }
        pr_info("hello_kernel: read_student_ID %d", current_id);    // 커널로그에 current_id값이 read되었다느 로그를 출력

        return 0;

    case IOCTL_SET_ID:   /* _IOW */
        // user에게 전달 받은 값을 커널의 current_id로 설정
        int prev = current_id;
        if (copy_from_user(&current_id, (int __user *)arg, sizeof(current_id))) {
            return -EFAULT;
        }
        pr_info("hello_kernel: change_student_ID %d -> %d", prev, current_id); // 커널로그에 current_id변경 로그 기록

        
        return 0;

    case IOCTL_SWAP_ID : /* _IOWR */

        // user와 커널의 데이터 swap. user가 전달한 포인터의 값을 커널의 current_id의 값으로 설정하고 user 가 전달한 포인터의 값을 기존의 current_id로 설정한다.
        int tmp = current_id;
        if (copy_from_user(&current_id, (int __user *)arg, sizeof(current_id))) {
            return -EFAULT;
        }
        if (copy_to_user((int __user *)arg, &tmp, sizeof(tmp))) {
            return -EFAULT;
        }
        pr_info("hello_kernel: swaped_student_ID is %d", current_id);   // 현재 변경된 current_id 출력

        return 0;

    default:

        return -ENOTTY; /* 지원하지 않는 ioctl */

    }
}

static int __init hello_init(void) {

    // tutorial코드를 참고하여 init코드 작성
    int ret;

    // allocate a single char device number
    ret = alloc_chrdev_region(&_dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("hello_kernel: failed to alloc_chrdev_region\n");    // major,minor 번호 미부여시 에러로그
        return ret;
    }

    // wire up file_operations and register cdev
    cdev_init(&_cdev, &_fops);
    _cdev.owner = THIS_MODULE;

    ret = cdev_add(&_cdev, _dev, 1);    // dev, cdev 를 커널에 등록
    if (ret < 0) {  // 커널 등록 실패시 dev 번호 반납
        pr_err("hello_kernel: failed to cdev_add\n");
        unregister_chrdev_region(_dev, 1);
        return ret;
    }

    // expose device under /dev
    _class = class_create(CLASS_NAME);
    if (IS_ERR(_class)) {       // class_create 실패시 에러 리턴
        pr_err("hello_kernel: failed to create class\n");
        cdev_del(&_cdev);
        unregister_chrdev_region(_dev, 1);
        return PTR_ERR(_class);
    }

    // 디바이스 생성
    if (IS_ERR(device_create(
        _class, NULL, _dev, NULL, DEVICE_NAME
    ))) {
        pr_err("hello_kernel: failed to create device\n");
        class_destroy(_class);
        cdev_del(&_cdev);
        unregister_chrdev_region(_dev, 1);
        return -EINVAL;
    }

    // 모든 단계 성공 시 mojor, minor번호 로그 출력
    pr_info(
        "hello_kernel: module loaded (major=%d, minor=%d)\n",
        MAJOR(_dev),
        MINOR(_dev)
    );
    return 0;


}

static void __exit hello_exit(void) {

    // to be implemented

    device_destroy(_class, _dev);   // 생성된 디바이스 제거
    class_destroy(_class);  // 디바이스 생성 클래스 제거
    cdev_del(&_cdev);   // 디바이스 번호 등록 제거
    unregister_chrdev_region(_dev, 1); // 디바이스 번호 반납

    pr_info("hello_kernel: module unloaded\n");

}
