#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//lab 2.1 sys_trace
uint64 sys_trace(void)
{
  int mask;
  //使用argint函数从用户进程的参数中获取整数值。
  //这个函数是xv6中的一个帮助函数，用于从用户空间获取参数。
  //argint的第一个参数是参数索引（从0开始），第二个参数是用于存储值的指针。
  //如果获取参数失败（返回值小于0），则返回-1表示出错
  if(argint(0, &mask) < 0)
    return -1;
  myproc()->tracemask = mask;
  return 0;
}

//Lab 2.2
uint64 sys_sysinfo(void)
{
  uint64 addr;
  if(argaddr(0, &addr)<0){
    return -1;
  }

  struct sysinfo info;
  info.freemem = getfreemem();
  info.nproc = getnproc();
  
  if(copyout(myproc()->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;

  return 0;
}