/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_SHA512_H
#define RCM_SHA512_H

/* Documentation: https://librcm.org/#rcm_sha512.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
SHA-512 module:

Implements the SHA-512 cryptographic hash function.
*/

/* The sha512 error type. */
typedef enum {
  RCM_SHA512_OK = 0,
  RCM_SHA512_ERR_FAILED_ASSERT = -4,
} rcm_sha512_err_t;

#include <stddef.h>

#include "rcm_assert.h"

/* The SHA-512 hash size. */
#define RCM_SHA512_SIZE 64

/* Calculate SHA-512 hash of in message of given inlen and store the result in
   out. The output buffer out must be at least `RCM_SHA512_SIZE` large! */
RCM_API rcm_sha512_err_t rcm_sha512(unsigned char *out, const unsigned char *in,
                                    size_t inlen);

#endif /* RCM_SHA512_H */
