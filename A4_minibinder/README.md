# Mini Binder 분석

본 코드는 과제의 예제 코드로 주어진 `Mini Binder`의 코드이며 본 과제는 코드의 문제점을 찾고 RPC 요청을 추적, IPC를 처리하기위한 커널의 동작 과정을 분석한다.

> [!Warning]
> - 해당 프로젝트 실습 시 반드시 VM을 사용하여 수행할 것.
>   - 커널을 다루는 프로젝트임과 동시에 안정성이 확보되지 않음.
>   - WSL, Docker 등의 환경에서는 커널 모듈 개발이 어려움. 

> [!NOTE]
> - 과제에 대한 자세한 설명은 `Assignment4.pdf`를 참고.
> - 과제 풀이에 대한 레포트는 `report.pdf`를 확인.

### report.pdf 목차
**R0. 코드 실행**
1. 실행 결과
2. 실행 중 문제점 분석

**R1. 코드 분석**
1. 호출 스택 분석
2. IPC 관점에서의 매커니즘 분석

**R2. Discussion**



## 코드 실행 방법
### 1. 디렉토리 구조

```text
.
├── Makefile
├── binder
│   ├── kernel/   # 커널 모듈 소스 (core.c 등)
│   └── lib/      # 유저 공간 라이브러리 (binder.c, parcel.c, looper.c, *.h)
└── apps
    ├── client.c  # 클라이언트 예제
    └── server.c  # 서버 예제
```

빌드 결과물은 `./.build/` 아래에 생성된다.

### 2. 빌드

```bash
make
```

프로젝트 루트(현재 디렉토리)에서 해당 명령으로 빌드 가능하며, 아래와 같은 프로그램이 생성된다.

- 커널 모듈: `./.build/minibinder.ko`
- 서버 프로그램: `./.build/server`
- 클라이언트 프로그램: `./.build/client`


### 3. 커널 모듈 로드, 디바이스 노드 확인

커널에 MiniBinder 모듈을 올린다.

```bash
sudo insmod .build/minibinder.ko
```

아래 코드로 로드 성공여부를 확인한다.

```bash
sudo insmod .build/minibinder.ko
```

해당 모듈은 `/dev/minibinder` 캐릭터 디바이스를 생성한다.

```bash
ls -l /dev/minibinder
```

위의 명령으로 minibinder 디바이스의 존재 유무를 확인한다.

### 4. 실행

서버 실행 후 클라이언트 실행

```bash
sudo ./.build/server
```

```bash
sudo ./.build/client
```

> [!NOTE]
> 서버가 켜져있지 않은 상태로 클라이언트 실행시 트랜잭션 처리가 되지 않거나 블록될 수 있음.

클라이언트 내부 동작:

1. `/dev/minibinder`에 attach
2. 시스템 바인더에 `"caculate_service"`를 BIND 요청
3. 얻은 binder를 통해 `TRANSACTION_ADD`, `SUB`, `MUL` 등의 RPC를 전송
4. 서버에서 계산한 결과를 받아 출력

### 5. 커널 로그 확인

```bash
sudo dmesg | tail
```

예상되는 메시지 예시:

- `minibinder: ATTACH -> handle=1`
- `SEND: handle=... code=...`
- `RECV: handle=... code=... offset=...`

### 6. 커널 모듈 언로드

커널로그 확인하여 모듈이 제거되었는지 확인할 것.

```bash
sudo rmmod minibinder
sudo dmesg | tail
```

> [!NOTE]
> 빌드 산출물 clean 시 커널 모듈을 우선 제거하는 것을 권장함.