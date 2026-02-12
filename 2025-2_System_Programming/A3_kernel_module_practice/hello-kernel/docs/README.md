# Hello Kernel – 과제용 README 

이 리포지토리는 System Programming 과목 **Hello Kernel 과제**용 코드입니다.  
이 파일은 빌드 및 커널 모듈 로드/실행/제거 절차만 간단히 정리한 문서입니다.

> 커널 모듈은 반드시 **VM(연습용 리눅스 환경)** 에서 실습하는 것을 권장합니다.

---

## 1. 빌드

프로젝트 루트 디렉토리에서:

```bash
make
```

성공하면 다음 파일들이 생성됩니다 (예시):

- 커널 모듈: `./.build/hello_kernel.ko`
- 유저 프로그램: `./.build/hello_user`

---

## 2. 커널 모듈 로드

빌드가 끝난 뒤, 커널에 모듈을 올립니다.

```bash
sudo insmod .build/hello_kernel.ko
```

모듈이 정상적으로 로드되었는지 확인하려면:

```bash
sudo dmesg | tail
```

`module loaded` 와 비슷한 메시지가 출력되는지 확인합니다.

---

## 3. 디바이스 노드 확인

모듈이 `/dev/hello_kernel` 같은 캐릭터 디바이스를 생성했다면:

```bash
ls -l /dev/hello_kernel
```

대략 아래와 비슷한 출력이 보이면 정상입니다.

```text
crw-rw---- 1 root root <major> <minor> /dev/hello_kernel
```

(실제 디바이스 이름은 코드/ABI에 따라 다를 수 있습니다.)

---

## 4. 유저 프로그램 실행

유저 프로그램은 빌드 후 `.build/hello_user` 에 생성된다고 가정합니다.

```bash
sudo ./.build/hello_user
```

- 프로그램이 커널 모듈에 `ioctl()` 또는 기타 요청을 전송합니다.
- 자세한 동작은 과제 설명 및 소스 코드를 참고하세요.

실행 후 커널 로그를 다시 확인합니다.

```bash
sudo dmesg | tail
```

- 유저 프로그램에서 보낸 요청에 대응하는 커널 측 로그가 찍혀 있는지 확인합니다.

---

## 5. 커널 모듈 언로드

실습이 끝나면 커널에서 모듈을 제거합니다.

```bash
sudo rmmod hello_kernel
sudo dmesg | tail
```

- `module unloaded` 와 비슷한 메시지가 출력되는지 확인합니다.
- `/dev/hello_kernel` 디바이스 노드도 사라져야 합니다.

```bash
ls -l /dev/hello_kernel
# → No such file or directory
```

---

## 6. 빌드 산출물 정리

필요하다면 빌드 산출물을 모두 지울 수 있습니다.

```bash
make clean
```

- 커널 빌드 중간 파일
- `.build/` 디렉토리

가 제거됩니다. (이미 로드된 모듈은 `sudo rmmod hello_kernel`으로 먼저 내린 뒤 `make clean` 을 실행하세요.)

<!-- ## Optional. _IOWR 명세는 이 아래에 작성할 것 -->

## 7. _IOWR 구현 명세
> **개발환경:** 
> - Ubuntu 24.04.3 LTS
> - kernel: 6.14.0-36-generic

**_IOWR** 동작으로 간단한 **data swap**을 구현하였습니다. 아래의 명령으로 동작합니다.
```bash
sudo ./.build/hello_user swap <swap_data>
```
이 기능은 `set`과 `get`의 기능을 동시에 수행하는 **_IOWR** 구현입니다.
- kernel의 `current_id`를 `<swap data>`로 변경하고 이를 kernel log에 출력합니다. (set 동작과 동일합니다.)
- kernel의 변경 전 `current_id`의 값을 user에게 전달합니다. user는 이를 쉘에 print합니다. (get 동작과 동일합니다.)

이 명령은 kernel 코드에서 `IOCTL_SWAP_ID` 커멘드를 실행하게 합니다.
- kernel은 `copy_to_user`와 `copy_from_user`로 위의 기능을 수행합니다.

커멘드 정의는 `abi.h`에서 `_IOWR` 구조체로 정의되었습니다.

## 8. Makefile 수정 사항
기존 스켈레톤 코드의 `main.c` 를 과제 제출 요구사항에 맞추어 `hello_user.c`로 변경하였습니다.

이에 따라 `Makefile`을 수정하였습니다. 이외 변경사항은 없습니다.


### 본 과제의 구현에는 아래의 문서에서 도움을 받았습니다.
- _아주대학교 시스템프로그래밍 25-2학기 고영배 교수님의 강의자료_
- _<https://wikidocs.net/196800>_ - Linux 커널 모듈 프로그래밍 가이드(5.x)
