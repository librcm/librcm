/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_RFC3339_H
#define RCM_RFC3339_H

/* Documentation: https://librcm.org/#rcm_rfc3339.h */

#define RCM_RFC3339_BUFSIZE 21

typedef struct rcm_rfc3339_t rcm_rfc3339_t;

RCM_API int rcm_rfc3339_parse(rcm_rfc3339_t *time, const char *value,
                              char *err);

RCM_API int rcm_rfc3339_now(rcm_rfc3339_t *t, char *err);

RCM_API void rcm_rfc3339_format(char *out, rcm_rfc3339_t time);

RCM_API bool rcm_rfc3339_after(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime);

RCM_API bool rcm_rfc3339_before(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime);

RCM_API rcm_rfc3339_t rcm_rfc3339_from_time_t(time_t time);

/* internal */
struct rcm_rfc3339_t {
  int year;
  int mon;
  int day;
  int hour;
  int min;
  int sec;
};

#endif
