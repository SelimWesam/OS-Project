
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user.h"

#define MAX_PROCS 64

int main()
{
    struct pstat procs[MAX_PROCS];

    if(getptable(MAX_PROCS, procs) == 0) {
        printf("Failed to get process table\n");
        exit(1);
    }

    printf("PID\tPPID\tSTATE\t\tNAME\tSIZE\n");
    for(int i = 0; i < MAX_PROCS; i++) {
      if(procs[i].pid == 0) {
          continue;
      }

      printf("%d\t%d\t%s\t%s\t%u\n",
             procs[i].pid, procs[i].ppid, procs[i].state,
             procs[i].name, procs[i].sz);
  }


    exit(0);
}
