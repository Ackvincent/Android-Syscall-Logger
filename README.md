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
2. ![image-20210228131545707](https://github.com/Katana-O/Android-Syscall-Logger/blob/main/images/1.png)
3. Build kernel module with "make" command
4. push kernel module to a certain directory at your phone.
5. ![image-20210228132042785](C:\Users\ccc\AppData\Roaming\Typora\typora-user-images\image-20210228132042785.png)
6. Initialize your module immediately by using [insmod xxxx.ko]
7. ![image-20210228132143703](C:\Users\ccc\AppData\Roaming\Typora\typora-user-images\image-20210228132143703.png)
8. Starting monitoring your log from kernel by using [dmesg -w | grep "myLog"]
9. ![image-20210228132340789](C:\Users\ccc\AppData\Roaming\Typora\typora-user-images\image-20210228132340789.png)
10. Enjoy your pleasure.
11. ![image-20210228132323015](C:\Users\ccc\AppData\Roaming\Typora\typora-user-images\image-20210228132323015.png)

