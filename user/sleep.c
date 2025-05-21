#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{
  int sec = atoi(argv[1]);
  printf("sleeping for %d seconds \n",sec );
  sleep(sec);
  exit(0);

}
