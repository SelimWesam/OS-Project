
#ifndef _PSTAT_H_
#define _PSTAT_H_

struct pstat {
  int pid;
  int ppid;
  int sz;
  char state[16];
  char name[16];
};

#endif
