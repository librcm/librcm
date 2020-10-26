/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_SHA256_H
#define RCM_SHA256_H

/* Documentation: https://librcm.org/#rcm_sha256.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
SHA-256 module:

Implements the SHA-256 cryptographic hash function.
*/

/* The sha256 error type. */
typedef enum {
  RCM_SHA256_OK = 0,
  RCM_SHA256_ERR_FAILED_ASSERT = -4,
} rcm_sha256_err_t;

#include <stddef.h>

#include "rcm_assert.h"

/* The SHA-256 hash size. */
#define RCM_SHA256_SIZE 32

/* Calculate SHA-256 hash of in message of given inlen and store the result in
   out. The output buffer out must be at least `RCM_SHA256_SIZE` large! */
RCM_API rcm_sha256_err_t rcm_sha256(unsigned char *out, const unsigned char *in,
                                    size_t inlen);

#endif /* RCM_SHA256_H */
