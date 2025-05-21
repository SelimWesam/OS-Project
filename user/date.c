
  #include "kernel/types.h"
  #include "kernel/fcntl.h"
  #include "user/user.h"
  int
  main(int argc, char *argv[])
  {
    struct datetime r;
  if (datetime(&r) == 0) {
    printf("current datetime: year=%d month=%d day=%d hour=%d minute=%d second=%d\n",
      r.year, r.month, r.day, r.hour, r.minute, r.second);

  } else {
    printf("Failed to get datetime\n");
  }
    exit(0);
  }
