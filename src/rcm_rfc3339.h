#ifndef RCM_RFC3339_H
#define RCM_RFC3339_H

#define RCM_RFC3339_BUFSIZE 21

/* RFC3339 type (internal). */
typedef struct rcm_rfc3339_t {
  int year; /* private */
  int mon;  /* private */
  int day;  /* private */
  int hour; /* private */
  int min;  /* private */
  int sec;  /* private */
} rcm_rfc3339_t;

/* Parse time value in RFC3339 format.
   Return -1, if the parsing failed. 0, otherwise.
   In case of error, the error message is written to err. */
RCM_API int rcm_rfc3339_parse(rcm_rfc3339_t *time, const char *value,
                              char *err);

/* Return the current time (now) in UTC. */
RCM_API rcm_rfc3339_t rcm_rfc3339_now(void);

/* Format time as RFC3339 in UTC to out buffer (\0-terminated).
   Out must have at least size RCM_RFC3339_BUFSIZE! */
RCM_API char *rcm_rfc3339_format(char *out, rcm_rfc3339_t time);

/* Return true if ltime is after rtime, false otherwise. */
RCM_API bool rcm_rfc3339_after(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime);

/* Return true if ltime is before rtime, false otherwise. */
RCM_API bool rcm_rfc3339_before(rcm_rfc3339_t ltime, rcm_rfc3339_t rtime);

/* Convert from time_t to rfc3339_t. */
RCM_API rcm_rfc3339_t rcm_rfc3339_from_time_t(time_t time);

#endif
