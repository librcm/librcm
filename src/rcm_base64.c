/* SPDX-License-Identifier: Unlicense OR MIT */

#include "rcm_base64.h"

RCM_API size_t rcm_base64_encode_len(size_t len)
{
  if (len == 0) {
    return 0;
  }
  /* The encoded length is either 0 or a multiple of 4.
     If the input len is divisible by 3 without remainder, just mutiply the
     result with 4 (no padding). Otherwise add 4 for the remainder (with
     padding). */
  return len / 3 * 4 + (len % 3) ? 4 : 0;
}

RCM_API rcm_base64_err_t rcm_base64_encode(char *out, const unsigned char *in,
                                           size_t len)
{
  if (rcm_assert(out && in)) {
    return RCM_BASE64_ERR_ASSERT_FAILED;
  }
  /* TODO */
  return RCM_BASE64_OK;
}

RCM_API size_t rcm_base64_decode_len(size_t len)
{
  if (len == 0) {
    return 0;
  }
  /* A len which is not evenly divisible is makes no sense, the error will be
     caught in the actual decoding. */
  return len / 4 * 3;
}

RCM_API rcm_base64_err_t rcm_base64_decode(unsigned char *out, const char *in,
                                           size_t len)
{
  if (rcm_assert(in)) {
    return RCM_BASE64_ERR_ASSERT_FAILED;
  }
  /* TODO */
  return RCM_BASE64_OK;
}

RCM_API const char *rcm_base64_errstr(rcm_base64_err_t errnum)
{
  switch (errnum) {
  case RCM_BASE64_OK:
    return "";
  case RCM_BASE64_ERR_ASSERT_FAILED:
    return "rcm_base64: assertion failed";
  case RCM_BASE64_ERR_ILLEGAL_CHAR:
    return "rcm_base64: illegal character";
  }
  return "rcm_base64: undefined errnum";
}
