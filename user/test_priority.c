#include "kernel/types.h"
#include "user/user.h"

int main() {
  setscheduler(2); // Priority

  if(fork() == 0) { setpriority(getpid(), 1); sleep(50); exit(0); }
  if(fork() == 0) { setpriority(getpid(), 10); sleep(30); exit(0); }

  wait(0); wait(0);

  uint64 metrics[3];
  getschedmetrics((uint64)metrics);

  printf("Priority - Avg Turnaround: %ld, Avg Waiting: %ld\n",
         metrics[0]/metrics[2], metrics[1]/metrics[2]);

  exit(0);
}
