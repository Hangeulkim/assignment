#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "processInfo.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

void sys_hello(void){
	return hello();	
}

void sys_hello_name(void){
	int argc;
	char **argv;
	argint(0,(void *)&argc);
	argstr(1,(void *)&argv);
	return hello_name(argc,argv);
}

void sys_get_num_proc(void){
	return get_num_proc();
}

int sys_get_max_pid(void){
	return get_max_pid();
}

int sys_get_proc_info(void){
	int pid=0;
	struct processInfo *pi;
	
	argint(0,(void*)&pid);
	argptr(1,(void*)&pi,sizeof(pi));
	
	return get_proc_info(pid,pi);
}

int sys_setprio(void){
	int n=0;
	
	argint(0,(void*)&n);
	
	return setprio(n);
}

int sys_getprio(void){
	return getprio();
}
