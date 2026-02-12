# 루트 디렉토리용 Makefile

APP_DIR   := apps
LIB_DIR   := binder/lib
KERN_DIR  := binder/kernel
BUILD_DIR := .build

# 현재 커널 빌드 디렉토리 (필요하면 과제에서 고정 값으로 안내해도 됨)
KDIR ?= /lib/modules/$(shell uname -r)/build

CC      := gcc
CFLAGS  := -Wall -Wextra -g -I$(LIB_DIR)
LDFLAGS := -lpthread

# 라이브러리 소스 및 오브젝트
LIB_SRCS := $(LIB_DIR)/binder.c \
            $(LIB_DIR)/looper.c \
            $(LIB_DIR)/parcel.c

LIB_OBJS := $(patsubst $(LIB_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRCS))

# 앱 타겟 이름
APPS := client server

APP_BINS := $(addprefix $(BUILD_DIR)/,$(APPS))

# 커널 모듈 이름
KMOD := $(BUILD_DIR)/minibinder.ko

.PHONY: all apps kernel clean

all: $(APP_BINS) $(KMOD)

apps: $(APP_BINS)

kernel: $(KMOD)

# .build 디렉토리 생성
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 라이브러리 오브젝트 빌드
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 앱 바이너리 빌드 (client, server)
$(BUILD_DIR)/%: $(APP_DIR)/%.c $(LIB_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LIB_OBJS) -o $@ $(LDFLAGS)

# 커널 모듈 빌드
$(KMOD): | $(BUILD_DIR)
	$(MAKE) -C $(KDIR) M=$(PWD)/$(KERN_DIR) modules
	cp $(KERN_DIR)/minibinder.ko $(KMOD)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD)/$(KERN_DIR) clean || true
	rm -rf $(BUILD_DIR)