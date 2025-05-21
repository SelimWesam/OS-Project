#include "types.h"
#ifndef DATE_H
#define DATE_H

struct rtcdate{
  int second;
  int minute;
  int hour;
  int day;
  int month;
  int year;

};

void unix_to_date(uint64 ts , struct rtcdate *r);
#endif
