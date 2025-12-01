#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/abi.h"

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s print             # _IO   : 커널에 Hello 메시지 요청\n"
        "  %s get               # _IOR  : 커널에서 id 읽기\n"
        "  %s set <id>          # _IOW  : 커널의 id 설정\n"
        "  %s swap <id>         # _IOWR : 커널과 id data swap; 커널에기존 id 읽은 후 커널의 새 id 설정\n",
        prog, prog, prog, prog);
}

static int open_device(void) {
    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("open");
        fprintf(stderr,
                "hint: 커널 모듈이 로드되어 있고 %s 이 존재하는지 확인하세요.\n",
                DEVICE_PATH);
    }
    return fd;
}

int main(int argc, char *argv[]) {
    int fd;
    if (argc < 2) { usage(argv[0]); return EXIT_FAILURE; }

    fd = open_device();
    if (fd < 0) return EXIT_FAILURE;

    if (strcmp(argv[1], "print") == 0) {

        // 커널에 저장된 current_id를 커널 로그에 출력
        if (ioctl(fd, IOCTL_PRINT) == -1) {
            perror("ioctl");
            close(fd);
            return EXIT_FAILURE;
        }
        printf("Done. dmesg 에서 커널 로그를 확인해보세요.\n");


    } else if (strcmp(argv[1], "get") == 0) {

        // 커널의 저장된 current_id를 result 변수로 가져온 후 result를 출력
        int result = 0;
        if (ioctl(fd, IOCTL_GET_ID, &result) == -1) {
            perror("ioctl");
            close(fd);
            return EXIT_FAILURE;
        }
        printf("Done. 현재 current_id: %d\n", result);

    } else if (strcmp(argv[1], "set") == 0) {

        // 커널의 current_id를 입력받은 argv 데이터로 설정.
        if (argc < 3) {
            fprintf(stderr, "arg: 변경할 set data를 두번째 인자로 입력해주세요.\n");
            return EXIT_FAILURE;
        }
        int input = atoi(argv[2]);  // int로 변환
        if (ioctl(fd, IOCTL_SET_ID, &input) == -1) {
            perror("ioctl");
            close(fd);
            return EXIT_FAILURE;
        }
        printf("Done. current_id가 %d로 변경되었습니다. dmesg 에서 커널 로그를 확인해보세요. get명령으로도 확인 가능합니다.\n", input);


    } else if (strcmp(argv[1], "swap") == 0) {

        // 커널의 current_id와 user 의 input의 값을 swap
        if (argc < 3) {
            fprintf(stderr, "arg: 변경할 set data를 두번째 인자로 입력해주세요.\n");
            return EXIT_FAILURE;
        }
        int input = atoi(argv[2]);  // int로 변환
        if (ioctl(fd, IOCTL_SWAP_ID, &input) == -1) {
            perror("ioctl");
            close(fd);
            return EXIT_FAILURE;
        }
        // 커널의 과거의 값 출력
        printf("Done. 커널의 current_id는 %d였습니다. 현재 커널의 id는 %s로 변경되었습니다. dmesg로 로그를 확인해보세요.\n", input, argv[2]);

    } else {
        usage(argv[0]);
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return EXIT_SUCCESS;
}