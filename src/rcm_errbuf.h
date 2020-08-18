/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef ERRBUF_H
#define ERRBUF_H

/* Documentation: https://librcm.org/#rcm_errbuf.h */

#ifndef RCM_API
#define RCM_API extern
#endif

#ifndef RCM_ERRBUF_SIZE
#define RCM_ERRBUF_SIZE 1024
#endif

#ifndef _WIN32
#define RCM_ERRBUF_FORMAT_ATTRIBUTE(i, j) __attribute__((format(printf, i, j)))
#else
#define RCM_ERRBUF_FORMAT_ATTRIBUTE(i, j)
#endif

RCM_API void rcm_errbuf_set(char *err, const char *format, ...)
    RCM_ERRBUF_FORMAT_ATTRIBUTE(2, 3);

#endif
