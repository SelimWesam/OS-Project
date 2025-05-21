#include "types.h"
#include "date.h"

static int is_leap(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void unix_to_date(uint64 ts, struct rtcdate *r) {
  static const int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};

  int secs = ts % 60;
  int mins = (ts / 60) % 60;
  int hrs  = (ts / 3600) % 24;
  int days = ts / 86400;

  int year = 1970;
  while (1) {
    int days_in_year = is_leap(year) ? 366 : 365;
    if (days >= days_in_year) {
      days -= days_in_year;
      year++;
    } else break;
  }

  int month = 0;
  while (1) {
    int dim = days_in_month[month];
    if (month == 1 && is_leap(year)) dim = 29;
    if (days >= dim) {
      days -= dim;
      month++;
    } else break;
  }

  r->year = year;
  r->month = month + 1;
  r->day = days + 1;
  r->hour = hrs;
  r->minute = mins;
  r->second = secs;
}
