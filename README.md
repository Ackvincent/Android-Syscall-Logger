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
3. Reset your sys_call_table address by reading /proc/kallsyms, if it shows 0 to you. [echo 0 > /proc/sys/kernel/kptr_restrict] should reveal their true address instead of 0.
4. ![6](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/6.png)
5. Build kernel module with "make" command
6. push kernel module to a certain directory at your phone.
7. ![2](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/2.png)
8. Initialize your module immediately by using [insmod xxxx.ko]
9. ![3](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/3.png)
10. Starting monitoring your log from kernel by using [dmesg -w | grep "myLog"]
11. ![4](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/4.png)
12. Enjoy your pleasure.
13. ![5](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/5.png)

