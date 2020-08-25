/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_RFC3339_H
#define RCM_RFC3339_H

/* Documentation: https://librcm.org/#rcm_rfc3339.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
RFC3339 time:

Module to deal with time strings in RFC3339 format.
*/

#include "rcm_assert.h"
#include "rcm_errbuf.h"

/* The default RFC3339 string buffer size. */
#define RCM_RFC3339_BUFSIZE 21

/* The RFC3339 error type. */
typedef enum {
  RCM_RFC3339_OK = 0,
  RCM_RFC3339_ERR_FAILED_ASSERT = -4,
  RCM_RFC3339_ERR_PARSE = -10,
  RCM_RFC3339_ERR_TIME = -11
} rcm_rfc3339_err_t;

/* The RCF3339 time type. */
typedef struct rcm_rfc3339_t rcm_rfc3339_t;

/* Parse time `value` in RFC3339 format and store the result in `time`
   (if not `NULL`).
   Returns `RCM_RFC3339_ERR_PARSE`, if the parsing failed. 0, otherwise.
   In case of error, the error message is written to `err`. */
RCM_API rcm_rfc3339_err_t rcm_rfc3339_parse(rcm_rfc3339_t *time,
                                            const char *value, char *err);

/* Return the current time (now) in UTC and store it in `t`.
   Returns `RCM_RFC3339_ERR_TIME`, if the time could not be determined.
   0, otherwise. In case of error, the error message is written to `err`. */
RCM_API rcm_rfc3339_err_t rcm_rfc3339_now(rcm_rfc3339_t *t, char *err);

/* Format time as RFC3339 in UTC to `out` buffer (`\0`-terminated).
   The `out` buffer must have at least size `RCM_RFC3339_BUFSIZE`! */
RCM_API rcm_rfc3339_err_t rcm_rfc3339_format(char *out, rcm_rfc3339_t time,
                                             char *err);

/* Return true if `ltime` is after `rtime`, false otherwise. */
RCM_API bool rcm_rfc3339_after(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime);

/* Return true if `ltime` is before `rtime`, false otherwise. */
RCM_API bool rcm_rfc3339_before(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime);

/* Convert from `time_t` to `rfc3339_t`. */
RCM_API rcm_rfc3339_t rcm_rfc3339_from_time_t(time_t time);

/* Return an error string describing the given error number errnum.
   For `RCM_RFC3339_OK` an empty string is returned ("", not NULL). */
RCM_API const char *rcm_rfc3339_errstr(rcm_rfc3339_err_t errnum);

/* internal */
struct rcm_rfc3339_t {
  int year;
  int mon;
  int day;
  int hour;
  int min;
  int sec;
};

#endif /* RCM_RFC3339_H */
