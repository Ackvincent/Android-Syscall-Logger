Android-Syscall-Logger
---

​	A kernel module that hook some of your system call on your Android Device by rewriting  syscall table.

Prerequisite
---

- pixel 1
- android-8.1.0_r1 == OPM1.171019.011
- Root Access
- Set CONFIG_DEBUG_RODATA to false so you are allowable to rewrite the syscall table.

## Usage

1. Adjust a little bit your Makefile
2. ![1](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/1.png)
3. You first reset your sys_call_table address by reading /proc/kallsyms, if it shows 0 to you. [echo 0 > /proc/sys/kernel/kptr_restrict] should reveal their true address instead of 0.
4. ![6](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/6.png)
5. Run make to compile the code. Which it should create a file that ends with .ko, that's your kernel module.
6. push kernel module to a certain directory at your phone.
7. ![2](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/2.png)
8. Initialize your module immediately by using [insmod xxxx.ko]
9. ![3](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/3.png)
10. Starting monitoring your log from kernel by using [dmesg -w | grep "myLog"]
11. ![4](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/4.png)
12. Enjoy your pleasure.
13. ![5](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/5.png)

## FAQ

1. How should I reconfig my kernel?
- Change Directory to your kernel. Use the following command below.
- ************************************************************************************************
- export ARCH=arm64 &&
- export PATH=~/aosp810r1/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin:$PATH &&
- export CROSS_COMPILE=aarch64-linux-android- &&
- make menuconfig
- ************************************************************************************************
- A Gui based menu will pop up on you screen. I recommend you use the following setings as I do.
- CONFIG_MODULES=Y
- CONFIG_STRICT_MEMORY_RWX=N / CONFIG_DEBUG_RODATA=N
- CONFIG_DEVMEM=Y
- CONFIG_DEVKMEM=Y
- CONFIG_KALLSYMS=Y
- CONFIG_KALLSYMS_ALL=Y
- CONFIG_HAVE_KPROBES=Y
- CONFIG_HAVE_KRETPROBES=Y
- CONFIG_HAVE_FUNCTION_TRACER=Y
- CONFIG_HAVE_FUNCTION_GRAPH_TRACER=Y
- CONFIG_TRACING=Y
- CONFIG_FTRACE=Y
- ************************************************************************************************
- Once you finish your editing, run make command again which would create a kernel Image and then flash it to your device. 
- Like this: fastboot flash boot ~/kernel/msm/arch/arm64/boot/Image.lz4-dtb

