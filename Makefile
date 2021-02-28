KERNEL_DIR=~/aosp810r1/kernel/msm

TOOLCHAIN=~/aosp810r1/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-

obj-m += AndroidSyscallLogger.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) ARCH=arm64 CROSS_COMPILE=$(TOOLCHAIN) modules

clean:
	make -C $(KERNEL_DIR) M=$(shell pwd) clean
