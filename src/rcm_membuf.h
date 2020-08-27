/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_MOD_H
#define RCM_MOD_H

/* Documentation: https://librcm.org/#rcm_membuf.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Memory buffer:

Higher level memory allocation API which sets an error buffer if allocations
fail.
*/

#include "rcm_assert.h"
#include "rcm_errbuf.h"
#include "rcm_mem.h"

/* The membuf error type. */
typedef enum {
  RCM_MEMBUF_OK = 0,
  RCM_MEMBUF_ERR_NOMEM = -2,
  RCM_MEMBUF_ERR_FAILED_ASSERT = -4,
} rcm_membuf_err_t;

/* Allocate memory buffer out of the given size. The memory is initialized to
   zero. */
RCM_API rcm_membuf_err_t rcm_membuf_new(void **out, size_t size, char *err);

/* Free memory buffer buf. */
RCM_API void rcm_membuf_free(void *buf);

/* Return an error string describing the given error number errnum.
   For `RCM_MEMBUF_OK` an empty string is returned ("", not `NULL`). */
RCM_API const char *rcm_membuf_errstr(rcm_membuf_err_t errnum);

#endif /* RCM_MEMBUF_H */
