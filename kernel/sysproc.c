#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "date.h"
#include "syscall.h"
#include "sched.h"



uint64 sys_getpid(void);


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}


uint64
sys_getpid(void)
{
  return (myproc()->pid+1);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
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

  argint(0, &pid);
  return kill(pid);
}


uint64
sys_getptable(void)
{
  // Get arguments directly from trapframe
  struct proc *p = myproc();
  int nproc = argraw(0);
  uint64 addr = argraw(1);

  // Validate arguments
  if(nproc < 1 || addr == 0)
    return 0;

  // Verify the user buffer is valid
  if(walkaddr(p->pagetable, addr) == 0)
    return 0;

  return getptable(nproc, (char *)addr);
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
extern uint64 boot_epoch;

// CLOCK_FREQ is typically 10MHz in QEMU
#define CLOCK_FREQ 10000000


uint64
sys_getppid(void)
{
  return sys_getpid();
}



uint64 sys_setscheduler(void)
{
  int type;
  argint(0, &type);

  if(type < 0 || type > 2)
    return -1;

  // No global process table lock needed here
  current_scheduler = type;

  return 0;
}


uint64 sys_setpriority(void)
{
  struct proc *p = myproc();
  int pid = p->trapframe->a0;      // First argument (pid)
  int priority = p->trapframe->a1;  // Second argument (priority)

  struct proc *target;
  for(target = proc; target < &proc[NPROC]; target++) {
    acquire(&target->lock);
    if(target->pid == pid) {
      target->priority = priority;
      release(&target->lock);
      return 0;
    }
    release(&target->lock);
  }
  return -1;
}

uint64 sys_getschedmetrics(void)
{
  struct proc *p = myproc();
  uint64 addr = p->trapframe->a0;  // User-space buffer address

  struct sched_metrics {
    uint64 total_turnaround;
    uint64 total_waiting;
    uint64 process_count;
  } metrics = {0};

  uint64 now = r_time();
  struct proc *proc_iter;

  for(proc_iter = proc; proc_iter < &proc[NPROC]; proc_iter++) {
    acquire(&proc_iter->lock);
    if(proc_iter->state != UNUSED) {
      metrics.total_turnaround += now - proc_iter->creation_time;
      metrics.total_waiting += (now - proc_iter->creation_time) - proc_iter->total_runtime;
      metrics.process_count++;
    }
    release(&proc_iter->lock);
  }

  // Direct copy to user space
  if(copyout(p->pagetable, addr, (char *)&metrics, sizeof(metrics)) < 0)
    return -1;

  return 0;
}
