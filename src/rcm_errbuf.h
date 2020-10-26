/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_ERRBUF_H
#define RCM_ERRBUF_H

/* Documentation: https://librcm.org/#rcm_errbuf.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Error buffer:

The error buffer module.
*/

/* The default error buffer size. */
#ifndef RCM_ERRBUF_SIZE
#define RCM_ERRBUF_SIZE 1024
#endif

#ifndef _WIN32
#define RCM_ERRBUF_FORMAT_ATTRIBUTE(i, j) __attribute__((format(printf, i, j)))
#else
#define RCM_ERRBUF_FORMAT_ATTRIBUTE(i, j)
#endif

/* Sets error buffer err to format string (printf(3)-like) with arguments
   `...`. Does nothing, if err is `NULL`. */
RCM_API void rcm_errbuf_set(char *err, const char *format, ...)
    RCM_ERRBUF_FORMAT_ATTRIBUTE(2, 3);

/* Sets error buffer err to string. Does nothing, if err is `NULL`. */
RCM_API void rcm_errbuf_set_str(char *err, const char *string);

#endif /* RCM_ERRBUF_H */
