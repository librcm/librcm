/* SPDX-License-Identifier: Unlicense OR MIT */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rcm_rfc3339.h"

RCM_API rcm_rfc3339_err_t rcm_rfc3339_parse(rcm_rfc3339_t *time,
                                            const char *value, char *err)
{
  unsigned long year, month, day, hour, minute, second;
  char *endptr;
  struct tm tm = { 0 };
  time_t t;

  if (!rcm_assert(value)) {
    rcm_errbuf_set(err, "%s",
                   rcm_rfc3339_errstr(RCM_RFC3339_ERR_FAILED_ASSERT));
    return RCM_RFC3339_ERR_FAILED_ASSERT;
  }

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
    return RCM_RFC3339_ERR_PARSE;
  }
  /* parse year */
  year = strtoul(value, &endptr, 10);
  if (endptr != value + 4) {
    rcm_errbuf_set(err, "rcm_rfc3339: year could not be parsed: %s", value);
    return RCM_RFC3339_ERR_PARSE;
  }
  if (year < 1900 || year > 9998) {
    rcm_errbuf_set(
        err, "rcm_rfc3339: year must be in the range [1900,9998]: %s", value);
    return RCM_RFC3339_ERR_PARSE;
  }

  /* parse month */
  month = strtoul(value + 5, &endptr, 10);
  if (endptr != value + 7) {
    rcm_errbuf_set(err, "rcm_rfc3339: month could not be parsed: %s", value);
    return RCM_RFC3339_ERR_PARSE;
  }
  if (month < 1 || month > 12) {
    rcm_errbuf_set(err, "rcm_rfc3339: month must be in the range [1,12]: %s",
                   value);
    return RCM_RFC3339_ERR_PARSE;
  }
  /* parse day */
  day = strtoul(value + 8, &endptr, 10);
  if (endptr != value + 10) {
    rcm_errbuf_set(err, "rcm_rfc3339: day could not be parsed: %s", value);
    return RCM_RFC3339_ERR_PARSE;
  }
  if (day < 1 || day > 31) {
    rcm_errbuf_set(err, "rcm_rfc3339: day must be in the range [1,31]: %s",
                   value);
    return RCM_RFC3339_ERR_PARSE;
  }
  /* parse hour */
  hour = strtoul(value + 11, &endptr, 10);
  if (endptr != value + 13) {
    rcm_errbuf_set(err, "rcm_rfc3339: hour could not be parsed: %s", value);
    return RCM_RFC3339_ERR_PARSE;
  }
  if (hour > 23) {
    rcm_errbuf_set(err, "rcm_rfc3339: hour must be in the range [0,23]: %s",
                   value);
    return RCM_RFC3339_ERR_PARSE;
  }
  /* parse minute */
  minute = strtoul(value + 14, &endptr, 10);
  if (endptr != value + 16) {
    rcm_errbuf_set(err, "rcm_rfc3339: minute could not be parsed: %s", value);
    return RCM_RFC3339_ERR_PARSE;
  }
  if (minute > 59) {
    rcm_errbuf_set(err, "rcm_rfc3339: minute must be in the range [0,59]: %s",
                   value);
    return RCM_RFC3339_ERR_PARSE;
  }
  /* parse second */
  second = strtoul(value + 17, &endptr, 10);
  if (endptr != value + 19) {
    rcm_errbuf_set(err, "rcm_rfc3339: second could not be parsed: %s", value);
    return RCM_RFC3339_ERR_PARSE;
  }
  if (second > 59) {
    rcm_errbuf_set(err, "rcm_rfc3339: second must be in the range [0,59]: %s",
                   value);
    return RCM_RFC3339_ERR_PARSE;
  }
  /* convert to time_t */
  tm.tm_year = (int)year - 1900;
  tm.tm_mon = (int)month - 1;
  tm.tm_mday = (int)day;
  tm.tm_hour = (int)hour;
  tm.tm_min = (int)minute;
  tm.tm_sec = (int)second;
  t = mktime(&tm);
  /* mktime(3) cannot overflow with the ranges we allow above */
  if (!rcm_assert(t != (time_t)-1)) {
    rcm_errbuf_set(err, "%s",
                   rcm_rfc3339_errstr(RCM_RFC3339_ERR_FAILED_ASSERT));
    return RCM_RFC3339_ERR_FAILED_ASSERT;
  }
  if (time) {
    time->year = (int)year;
    time->mon = (int)month;
    time->day = (int)day;
    time->hour = (int)hour;
    time->min = (int)minute;
    time->sec = (int)second;
  }
  return RCM_RFC3339_OK;
}

static void rcm_rfc3339_gmtime(struct tm *result, time_t t)
{
#ifndef _WIN32
  gmtime_r(&t, result);
#else
  gmtime_s(result, &t);
#endif
}

RCM_API rcm_rfc3339_err_t rcm_rfc3339_now(rcm_rfc3339_t *t, char *err)
{
  struct tm tm;
  time_t now;
  if (!rcm_assert(t)) {
    rcm_errbuf_set(err, "%s",
                   rcm_rfc3339_errstr(RCM_RFC3339_ERR_FAILED_ASSERT));
    return RCM_RFC3339_ERR_FAILED_ASSERT;
  }
  now = time(NULL);
  /* time(NULL) cannot fail */
  if (!rcm_assert(now != (time_t)-1)) {
    rcm_errbuf_set(err, "%s",
                   rcm_rfc3339_errstr(RCM_RFC3339_ERR_FAILED_ASSERT));
    return RCM_RFC3339_ERR_FAILED_ASSERT;
  }
  rcm_rfc3339_gmtime(&tm, now);
  t->sec = tm.tm_sec;
  t->min = tm.tm_min;
  t->hour = tm.tm_hour;
  t->day = tm.tm_mday;
  t->mon = tm.tm_mon + 1;
  t->year = tm.tm_year + 1900;
  return RCM_RFC3339_OK;
}

RCM_API rcm_rfc3339_err_t rcm_rfc3339_format(char *out, rcm_rfc3339_t time,
                                             char *err)
{
  if (!rcm_assert(out)) {
    rcm_errbuf_set(err, "%s",
                   rcm_rfc3339_errstr(RCM_RFC3339_ERR_FAILED_ASSERT));
    return RCM_RFC3339_ERR_FAILED_ASSERT;
  }
  snprintf(out, RCM_RFC3339_BUFSIZE, "%04u-%02u-%02uT%02u:%02u:%02uZ",
           time.year, time.mon, time.day, time.hour, time.min, time.sec);
  return RCM_RFC3339_OK;
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
  struct tm tm;
  rcm_rfc3339_gmtime(&tm, time);
  t.sec = tm.tm_sec;
  t.min = tm.tm_min;
  t.hour = tm.tm_hour;
  t.day = tm.tm_mday;
  t.mon = tm.tm_mon + 1;
  t.year = tm.tm_year + 1900;
  return t;
}

RCM_API const char *rcm_rfc3339_errstr(rcm_rfc3339_err_t errnum)
{
  switch (errnum) {
  case RCM_RFC3339_OK:
    return "";
  case RCM_RFC3339_ERR_FAILED_ASSERT:
    return "rcm_rfc3339: assertion failed";
  case RCM_RFC3339_ERR_PARSE:
    return "rcm_rfc3339: parse error";
  case RCM_RFC3339_ERR_TIME:
    return "rcm_rfc3339: cannot get time";
  }
  return "rcm_rfc3339: undefined errnum";
}
