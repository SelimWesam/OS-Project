#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/defs.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "spinlock.h"
#include "kernel/proc.h"
#include "kernel/date.h"

extern uint64 kbd_int_count;
uint64
sys_kbdint(){
return kbd_int_count;

}

extern uint64 count_syscall;

uint64 sys_countsyscall(){
return count_syscall;

}

int
do_rand(unsigned long *ctx)
{
/*
 * Compute x = (7^5 * x) mod (2^31 - 1)
 * without overflowing 31 bits:
 *      (2^31 - 1) = 127773 * (7^5) + 2836
 * From "Random number generators: good ones are hard to find",
 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
 * October 1988, p. 1195.
 */
    long hi, lo, x;

    /* Transform to [1, 0x7ffffffe] range. */
    x = (*ctx % 0x7ffffffe) + 1;
    hi = x / 127773;
    lo = x % 127773;
    x = 16807 * lo - 2836 * hi;
    if (x < 0)
        x += 0x7fffffff;
    /* Transform to [0, 0x7ffffffd] range. */
    x--;
    *ctx = x;
    return (x);
}

unsigned long rand_next = 1;

uint64
sys_rand(void)
{
  return (do_rand(&rand_next));
}



extern uint ticks;

#define CLINT          0x02000000L


#define TICKS_PER_SECOND 10000000L

void epoch_to_rtc(uint64 epoch, struct rtcdate *r);


int sys_datetime(void) {
  uint64 user_dst;


  argaddr(0, &user_dst);  // OK to assume it's void-returning


  uint64 seconds_since_boot = ticks / TICKS_PER_SECOND;

  uint64 epoch = BOOT_EPOCH + seconds_since_boot;




  struct rtcdate r;
  epoch_to_rtc(epoch, &r);


  if (copyout(myproc()->pagetable, user_dst, (char *)&r, sizeof(r)) < 0){
    return -1;}





  return 0;
}


void epoch_to_rtc(uint64 epoch, struct rtcdate *r) {
  int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  uint64 seconds = epoch;
  r->second = seconds % 60; seconds /= 60;
  r->minute = seconds % 60; seconds /= 60;
  r->hour = (3) % 24;
  r->hour = (r->hour + 7) % 24;
    seconds /= 24;

  int days = seconds;
  int year = 1970;
  while (1) {
    int leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    int days_in_year = 365 + leap;
    if (days >= days_in_year) {
      days -= days_in_year;
      year++;
    } else break;
  }
  r->year = year;

  int month = 0;
  while (1) {
    int dim = days_in_month[month];
    if (month == 1 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
      dim++;
    if (days >= dim) {
      days -= dim;
      month++;
    } else break;
  }
  r->month = month + 1;
  r->day = days + 1;
}
