/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_HEX_H
#define RCM_HEX_H

/* Documentation: https://librcm.org/#rcm_hex.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Hex module:

Hex parsing functions.
*/

#include <stddef.h>

#include "rcm_assert.h"

/* The hex error type. */
typedef enum {
  RCM_HEX_OK = 0,
  RCM_HEX_ERR_FAILED_ASSERT = -4,
  RCM_HEX_ERR_PARSE = -15
} rcm_hex_err_t;

/* Parse two-character hex bytestr and store the result in a single byte. */
RCM_API rcm_hex_err_t rcm_hex_parse_byte(unsigned char *byte,
                                         const char *bytestr);

/* Parse hex string of given len and store the resulting bytes in out.
   The output buffer out must have at least the length len/2! */
RCM_API rcm_hex_err_t rcm_hex_parse_str(unsigned char *out, const char *hex,
                                        size_t len);

#endif /* RCM_HEX_H */
