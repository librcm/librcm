/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_BASE64_H
#define RCM_BASE64_H

/* Documentation: https://librcm.org/#rcm_base64.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Base64 encoding:

This module implements low-level funtions for the base64 standard encoding
with padding as described in RFC 4648. Use the rcm_base64_buf module for
higher-level functions (with memory allocation and error buffers).
*/

#include <stddef.h>

#include "rcm_assert.h"

typedef enum {
  RCM_BASE64_OK = 0,
  RCM_BASE64_ERR_ASSERT_FAILED = -4,
  RCM_BASE64_ERR_ILLEGAL_CHAR = -5
} rcm_base64_err_t;

/* Returns the base64 encoded length of a binary input buffer of size len. */
RCM_API size_t rcm_base64_encode_len(size_t len);

/* Writes the base64 encoding of the input buffer in of lenght len to out and
   appends a `\0` character. The output buffer out has to be at least the size
   `rcm_base64_encode_len(len)+1`! */
RCM_API rcm_base64_err_t rcm_base64_encode(char *out, const unsigned char *in,
                                           size_t len);

/* Returns the decoded length of an base64 encoded input buffer of size len. */
RCM_API size_t rcm_base64_decode_len(size_t len);

/* Decodes the base64 encoded input in of length len and writes it to out (if
   not NULL). If the the output buffer out is not NULL it has to be at least of
   the size `rcm_base64_decode_len(len)`! */
RCM_API rcm_base64_err_t rcm_base64_decode(unsigned char *out, const char *in,
                                           size_t len);

/* Return an error string describing the given error number errnum.
   For RCM_BASE64_OK an empty string is returned ("", not NULL). */
RCM_API const char *rcm_base64_errstr(rcm_base64_err_t errnum);

#endif /* RCM_BASE64_H */
