/* SPDX-License-Identifier: Unlicense OR MIT */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rcm_errbuf.h"
#include "rcm_rfc3339.h"

RCM_API int rcm_rfc3339_parse(rcm_rfc3339_t *time, const char *value, char *err)
{
  unsigned long year, month, day, hour, minute, second;
  char *endptr;
  int ret = 0;

  assert(time && value);

  /* try to parse value as YYYY-MM-DDTHH:MM:SSZ (as in ISO 8601 / RFC 3339) */
  if (strlen(value) != 20 || value[4] != '-' || value[7] != '-' ||
      value[10] != 'T' || value[19] != 'Z' ||
      strpbrk(value, "0123456789") != value ||
      strpbrk(value + 5, "0123456789") != value + 5 ||
      strpbrk(value + 8, "0123456789") != value + 8 ||
      strpbrk(value + 11, "0123456789") != value + 11 ||
      strpbrk(value + 14, "0123456789") != value + 14 ||
      strpbrk(value + 17, "0123456789") != value + 17) {
    rcm_errbuf_set(err,
                   "rcm_rfc3339: ill-formed value (must have format "
                   "\"YYYY-MM-DDTHH:MM:SSZ\"): %s",
                   value);
    ret = -1;
  }
  /* parse year */
  if (!ret) {
    year = strtoul(value, &endptr, 10);
    if (endptr != value + 4) {
      rcm_errbuf_set(err, "rcm_rfc3339: year could not be parsed: %s", value);
      ret = -1;
    }
  }
  if (!ret) {
    if (year < 1900 || year > 2525) {
      rcm_errbuf_set(
          err, "rcm_rfc3339: year must be in the range [1900,2525]: %s", value);
      ret = -1;
    }
  }

  /* parse month */
  if (!ret) {
    month = strtoul(value + 5, &endptr, 10);
    if (endptr != value + 7) {
      rcm_errbuf_set(err, "rcm_rfc3339: month could not be parsed: %s", value);
      ret = -1;
    }
  }
  if (!ret) {
    if (month < 1 || month > 12) {
      rcm_errbuf_set(err, "rcm_rfc3339: month must be in the range [1,12]: %s",
                     value);
      ret = -1;
    }
  }
  /* parse day */
  if (!ret) {
    day = strtoul(value + 8, &endptr, 10);
    if (endptr != value + 10) {
      rcm_errbuf_set(err, "rcm_rfc3339: day could not be parsed: %s", value);
      ret = -1;
    }
  }
  if (!ret) {
    if (day < 1 || day > 31) {
      rcm_errbuf_set(err, "rcm_rfc3339: day must be in the range [1,31]: %s",
                     value);
      ret = -1;
    }
  }
  /* parse hour */
  if (!ret) {
    hour = strtoul(value + 11, &endptr, 10);
    if (endptr != value + 13) {
      rcm_errbuf_set(err, "rcm_rfc3339: hour could not be parsed: %s", value);
      ret = -1;
    }
  }
  if (!ret) {
    if (hour > 23) {
      rcm_errbuf_set(err, "rcm_rfc3339: hour must be in the range [0,23]: %s",
                     value);
      ret = -1;
    }
  }
  /* parse minute */
  if (!ret) {
    minute = strtoul(value + 14, &endptr, 10);
    if (endptr != value + 16) {
      rcm_errbuf_set(err, "rcm_rfc3339: minute could not be parsed: %s", value);
      ret = -1;
    }
  }
  if (!ret) {
    if (minute > 59) {
      rcm_errbuf_set(err, "rcm_rfc3339: minute must be in the range [0,59]: %s",
                     value);
      ret = -1;
    }
  }
  /* parse second */
  if (!ret) {
    second = strtoul(value + 17, &endptr, 10);
    if (endptr != value + 19) {
      rcm_errbuf_set(err, "rcm_rfc3339: second could not be parsed: %s", value);
      ret = -1;
    }
  }
  if (!ret) {
    if (second > 59) {
      rcm_errbuf_set(err, "rcm_rfc3339: second must be in the range [0,59]: %s",
                     value);
      ret = -1;
    }
  }
  /* convert to time_t */
  if (!ret) {
    struct tm tm;
    time_t t;
    memset(&tm, 0, sizeof tm);
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    t = mktime(&tm);
    if (t == (time_t)-1) {
      rcm_errbuf_set(err,
                     "rcm_rfc3339: could not convert value to time (must have "
                     "format \"YYYY-MM-DDTHH:MM:SSZ\"): %s",
                     value);
      ret = -1;
    } else if (time) {
      time->year = year;
      time->mon = month;
      time->day = day;
      time->hour = hour;
      time->min = minute;
      time->sec = second;
    }
  }
  return ret;
}

RCM_API int rcm_rfc3339_now(rcm_rfc3339_t *t, char *err)
{
  struct tm *tm;
  time_t now;
  assert(t);
  if (time(&now) == (time_t)-1) {
    rcm_errbuf_set(err, "rcm_rfc3339: cannot get time: %s", strerror(errno));
    return -1;
  }
  tm = gmtime(&now);
  t->sec = tm->tm_sec;
  t->min = tm->tm_min;
  t->hour = tm->tm_hour;
  t->day = tm->tm_mday;
  t->mon = tm->tm_mon + 1;
  t->year = tm->tm_year + 1900;
  return 0;
}

RCM_API void rcm_rfc3339_format(char *out, rcm_rfc3339_t time)
{
  assert(out);
  snprintf(out, RCM_RFC3339_BUFSIZE, "%04u-%02u-%02uT%02u:%02u:%02uZ",
           time.year, time.mon, time.day, time.hour, time.min, time.sec);
}

RCM_API bool rcm_rfc3339_after(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime)
{
  if (ltime.year > rtime.year) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon > rtime.mon) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day > rtime.day) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day == rtime.day && ltime.hour > rtime.hour) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day == rtime.day && ltime.hour == rtime.hour &&
      ltime.min > rtime.min) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day == rtime.day && ltime.hour == rtime.hour &&
      ltime.min == rtime.min && ltime.sec > rtime.sec) {
    return true;
  }
  return false;
}

RCM_API bool rcm_rfc3339_before(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime)
{
  if (ltime.year < rtime.year) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon < rtime.mon) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day < rtime.day) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day == rtime.day && ltime.hour < rtime.hour) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day == rtime.day && ltime.hour == rtime.hour &&
      ltime.min < rtime.min) {
    return true;
  }
  if (ltime.year == rtime.year && ltime.mon == rtime.mon &&
      ltime.day == rtime.day && ltime.hour == rtime.hour &&
      ltime.min == rtime.min && ltime.sec < rtime.sec) {
    return true;
  }
  return false;
}

RCM_API rcm_rfc3339_t rcm_rfc3339_from_time_t(time_t time)
{
  rcm_rfc3339_t t;
  struct tm *tm;
  tm = gmtime(&time);
  t.sec = tm->tm_sec;
  t.min = tm->tm_min;
  t.hour = tm->tm_hour;
  t.day = tm->tm_mday;
  t.mon = tm->tm_mon + 1;
  t.year = tm->tm_year + 1900;
  return t;
}
