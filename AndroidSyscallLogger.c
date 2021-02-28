#include "linux/kernel.h"
#include "linux/init.h"
#include "linux/module.h"
#include "linux/moduleparam.h"
#include "asm/unistd.h"
#include "linux/slab.h"
#include "linux/sched.h"
#include "linux/uaccess.h"


void ** sys_call_table64 = (void**)0xffffffc000ff0000;

#define SURPRESS_WARNING __attribute__((unused)) 
SURPRESS_WARNING int getCurrentPid(void)
{
   int pid = get_current()->pid;
   return pid;
}

SURPRESS_WARNING int isUserPid(void)
{
   const struct cred * m_cred = current_cred();
   kuid_t uid = m_cred->uid;
   int m_uid = uid.val;
   if(m_uid > 10000)
   {
      return true;
   }
   return false;
}

SURPRESS_WARNING asmlinkage int (*old_openat64)(int dirfd, const char __user* pathname, int flags, umode_t modex);
SURPRESS_WARNING int new_openat64(int dirfd, const char __user* pathname, int flags, umode_t modex)
{
   int ret = -1;
   if(isUserPid())
   {
      char bufname[256] = {0};
      strncpy_from_user(bufname, pathname, 255);
      printk("myLog::openat64 pathname:[%s] current->pid:[%d]\n", bufname, getCurrentPid());
   }
   ret = old_openat64(dirfd, pathname, flags, modex);
   return ret;
}

//extern "C" long __ptrace(int req, pid_t pid, void* addr, void* data);
SURPRESS_WARNING asmlinkage int (*old_ptrace64)(int request, pid_t pid, void* addr, void* data);
SURPRESS_WARNING int new_ptrace64(int request, pid_t pid, void* addr, void* data){
   int ret = -1;
   if(isUserPid()){
      printk("myLog::ptrace64 request:[%d] ptrace-pid:[%d] addr:[%p] currentPid:[%d]\n", request, pid, addr, getCurrentPid());
   }
   ret = old_ptrace64(request, pid, addr, data);
   return ret;
}

//int kill(pid_t pid, int sig);
SURPRESS_WARNING asmlinkage int (*old_kill64)(pid_t pid, int sig);
SURPRESS_WARNING int new_kill64(pid_t pid, int sig){
   int ret = -1;
   if(isUserPid()){
      printk("myLog::kill64 target_pid:[%d] sig:[%d] currentPid:[%d]\n", pid, sig, getCurrentPid());
   }
   ret = old_kill64(pid, sig);
   return ret;
}

//int tkill(int tid, int sig);
SURPRESS_WARNING asmlinkage int (*old_tkill64)(int tid, int sig);
SURPRESS_WARNING int new_tkill64(int tid, int sig){
   int ret = -1;
   if(isUserPid()){
      printk("myLog::tkill64 target_tid:[%d] sig:[%d] currentPid:[%d]\n", tid, sig, getCurrentPid());
   }
   ret = old_tkill64(tid, sig);
   return ret;
}

//int tgkill(int tgid, int tid, int sig);
SURPRESS_WARNING asmlinkage int (*old_tgkill64)(int tgid, int tid, int sig);
SURPRESS_WARNING int new_tgkill64(int tgid, int tid, int sig){
   int ret = -1;
   if(isUserPid()){
      printk("myLog::tgkill64 tgid:[%d] tid:[%d] sig:[%d] currentPid:[%d]\n", tgid, tid, sig, getCurrentPid());
   }
   ret = old_tgkill64(tgid, tid, sig);
   return ret;
}


//void exit(int status);
SURPRESS_WARNING asmlinkage int (*old_exit64)(int status);
SURPRESS_WARNING int new_exit64(int status){
   int ret = -1;
   if(isUserPid()){
      printk("myLog::exit64 enter, status num:[%d] currentPid:[%d]\n", status, getCurrentPid());
   }
   ret = old_exit64(status);
   return ret;
}


//int execve(const char *pathname, char *const argv[], char *const envp[]);
SURPRESS_WARNING asmlinkage int (*old_execve64)(const char *pathname, char *const argv[], char *const envp[]);
SURPRESS_WARNING int new_execve64(const char *pathname, char *const argv[], char *const envp[]){
   int ret = -1;
   if(isUserPid()){
      char bufname[256] = {0};
      strncpy_from_user(bufname, pathname, 255);
      printk("myLog::execve64 pathname:[%s] currentPid:[%d]\n", bufname, getCurrentPid());
   }
   ret = old_execve64(pathname, argv, envp);
   return ret;
}

//int execve(const char *pathname, char *const argv[], char *const envp[]);
SURPRESS_WARNING asmlinkage int (*old_clone64)(void * a0, void * a1, void * a2, void * a3, void * a4);
SURPRESS_WARNING int new_clone64(void * a0, void * a1, void * a2, void * a3, void * a4){
   int tid = old_clone64(a0, a1, a2, a3, a4);
   if(isUserPid()){
      printk("myLog::clone64 return Tid:[%d] currentPid:[%d]\n", tid, getCurrentPid());
   }
   return tid;
}


//fork = __NR_set_tid_address -> __NR_unshare

//set_tid_address - set pointer to thread ID
SURPRESS_WARNING asmlinkage int (*old_set_tid_address)(int * tidptr);
SURPRESS_WARNING int new_set_tid_address(int * tidptr){
   int tid = old_set_tid_address(tidptr);
   if(isUserPid()){
      printk("myLog::set_tid_address64 return Tid:[%d]\n", tid);
   }
   return tid;
}

//int unshare(int flags);
SURPRESS_WARNING asmlinkage int (*old_unshare)(int flags);
SURPRESS_WARNING int new_unshare(int flags)
{
   if(isUserPid()){
      printk("myLog::unshare flags:[%d]\n", flags);
   }
   return old_unshare(flags);
}



SURPRESS_WARNING static int hook_init(void){
   printk("myLog::hook init success\n");

   old_openat64 = (void*)(sys_call_table64[__NR_openat]);
   printk("myLog::old_openat64 : %p\n", old_openat64);
   sys_call_table64[__NR_openat] = (void*)new_openat64;
   
   old_ptrace64 = (void*)(sys_call_table64[__NR_ptrace]);
   printk("myLog::old_ptrace64 : %p\n", old_ptrace64);
   sys_call_table64[__NR_ptrace] = (void*)new_ptrace64;

   old_kill64 = (void*)(sys_call_table64[__NR_kill]);
   printk("myLog::old_kill64 : %p\n", old_kill64);
   sys_call_table64[__NR_kill] = (void*)new_kill64;

   old_tkill64 = (void*)(sys_call_table64[__NR_tkill]);
   printk("myLog::old_tkill64 : %p\n", old_tkill64);
   sys_call_table64[__NR_tkill] = (void*)new_tkill64;

   old_tgkill64 =(void*)(sys_call_table64[__NR_tgkill]);
   printk("myLog::old_tgkill64 : %p\n", old_tgkill64);
   sys_call_table64[__NR_tgkill] = (void*)new_tgkill64;

   old_exit64 =(void*)(sys_call_table64[__NR_exit]);
   printk("myLog::old_exit64 : %p\n", old_exit64);
   sys_call_table64[__NR_exit] = (void*)new_exit64;

   old_execve64 =(void*)(sys_call_table64[__NR_execve]);
   printk("myLog::old_execve64 : %p\n", old_execve64);
   sys_call_table64[__NR_execve] = (void*)new_execve64;

   old_clone64 =(void*)(sys_call_table64[__NR_clone]);
   printk("myLog::old_clone64 : %p\n", old_clone64);
   sys_call_table64[__NR_clone] = (void*)new_clone64;
   
   old_set_tid_address =(void*)(sys_call_table64[__NR_set_tid_address]);
   printk("myLog::old_set_tid_address64 : %p\n", old_set_tid_address);
   sys_call_table64[__NR_set_tid_address] = (void*)new_set_tid_address;

   old_unshare =(void*)(sys_call_table64[__NR_unshare]);
   printk("myLog::old_unshare64 : %p\n", old_unshare);
   sys_call_table64[__NR_unshare] = (void*)new_unshare;
   
   
   printk("myLog::hook init end\n");
   return 0;
}


static int __init myInit(void){
   printk(KERN_ALERT "myLog::hooksyscall Loaded1\n");
   hook_init();
   return 0;
}
static void __exit myExit(void){
   printk(KERN_ALERT "myLog::hooksyscall Quited\n");
}
module_init(myInit);
module_exit(myExit);
