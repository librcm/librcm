/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_RAND_H
#define RCM_RAND_H

/* Documentation: https://librcm.org/#rcm_rand.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Number generator:

Portable call to the system random number generator (RNG).
*/

#include <stddef.h>

#include "rcm_assert.h"

/* The rand error type. */
typedef enum {
  RCM_RAND_OK = 0,
  RCM_RAND_ERR_FAILED_ASSERT = -4,
  RCM_RAND_ERR_RANDOM = -14
} rcm_rand_err_t;

/* Read len random bytes from the sytem RNG and write it to out.
   If not enought bytes could be read `RCM_RAND_ERR_RANDOM` is returned (and
   out can be partially written to). */
RCM_API rcm_rand_err_t rcm_rand_bytes(unsigned char *out, size_t len);

/* Return an error string describing the given error number errnum.
   For `RCM_RAND_OK` an empty string is returned ("", not `NULL`). */
RCM_API const char *rcm_rand_errstr(rcm_rand_err_t errnum);

#endif /* RCM_RAND_H */
