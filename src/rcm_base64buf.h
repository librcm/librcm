/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_BASE64BUF_H
#define RCM_BASE64BUF_H

/* Documentation: https://librcm.org/#rcm_base64buf.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Base64 buffer:

This module implements high-level functions for the base64 standard encoding
with padding as described in RFC 4648. Use the rcm_base64 module for
lower-level functions (without memory allocation and error buffers).
*/

#include "rcm_assert.h"
#include "rcm_base64.h"
#include "rcm_errbuf.h"
#include "rcm_mem.h"

/* The base64buf error type. */
typedef enum {
  RCM_BASE64BUF_OK = 0,
  RCM_BASE64BUF_ERR_NOMEM = -2,
  RCM_BASE64BUF_ERR_FAILED_ASSERT = -4,
  RCM_BASE64BUF_ERR_ILLEGAL_LENGTH = -10,
  RCM_BASE64BUF_ERR_ILLEGAL_CHAR = -11
} rcm_base64buf_err_t;

/* Encodes the base64 encoding of the input buffer in of lenght len and writes
   it as a null-terminated string to out. */
RCM_API rcm_base64buf_err_t rcm_base64buf_encode(char **out,
                                                 const unsigned char *in,
                                                 size_t len, char *err);

/* Decodes the base64 encoded input in of length len and writes it to the
   freshly allocated out buffer. If outlen is not NULL, the length of the output
   buffer is written to it. In case of error out remains unmodified, an error
   code is returned, and an error message is written to err. */
RCM_API rcm_base64buf_err_t rcm_base64buf_decode(unsigned char **out,
                                                 size_t *outlen, const char *in,
                                                 size_t len, char *err);

/* Return an error string describing the given error number errnum.
   For `RCM_BASE64BUF_OK` an empty string is returned ("", not `NULL`). */
RCM_API const char *rcm_base64buf_errstr(rcm_base64buf_err_t errnum);

#endif /* RCM_BASE64BUF_H */
