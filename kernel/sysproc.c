#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
/* The following code is added/modified by Christian Flores cdf200003, Yash Dalvi ypd170000
** This code includes the processor stats structure into sysproc.c
*/
#include "pstat.h"
/* End of code added/modified */


/* The following code is added/modified by Christian Flores cdf200003, Yash Dalvi ypd170000
** This code implements the getpinfo and settickets system calls
** to get each processes' information and to set the number of tickets for a process
*/
int sys_getpinfo(void){
	struct pstat *pst;
  // Retrieve pointer argument from the user system call and return 0 if it's invalid
	if(argptr(0, (void*)&pst, sizeof(struct pstat)) < 0)
		return -1;	
  if(pst == NULL){
		return -1;
	}
  /* Pass the resulting process structure pointer into getpinfo which implements the calculation
  ** of getpinfo in proc.c
  */
	getpinfo(pst);
	return 0;
}

int sys_settickets(void){
	int ticketsNum;
  // Retrieve int argument from the system call and return 0 if it's invalid
	if(argint(0, &ticketsNum) < 0){
		return -1;
	}
	if(ticketsNum <= 0)
		return -1;
  // Set the current process's ticket count to the int argument from the user
	proc->ticket = ticketsNum;
	return 0;
}
/* End of code added/modified */
	
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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
