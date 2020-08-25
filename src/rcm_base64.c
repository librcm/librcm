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
  return len / 3 * 4 + ((len % 3) ? 4 : 0);
}

static const char rcm_base64_alpha[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char rcm_base64_padding = '=';

RCM_API rcm_base64_err_t rcm_base64_encode(char *out, const unsigned char *in,
                                           size_t len)
{
  size_t i, o;
  if (!rcm_assert(out && in)) {
    return RCM_BASE64_ERR_FAILED_ASSERT;
  }
  if (len == 0) {
    out[0] = '\0';
    return RCM_BASE64_OK;
  }
  for (i = 0, o = 0; i < len; i++, o++) {
    switch (i % 3) {
    case 0:
      out[o] = rcm_base64_alpha[in[i] >> 2];
      break;
    case 1:
      out[o] =
          rcm_base64_alpha[((in[i - 1] & 0x03) << 4) | ((in[i] & 0xf0) >> 4)];
      break;
    case 2:
      out[o++] =
          rcm_base64_alpha[((in[i - 1] & 0x0f) << 2) | ((in[i] & 0xc0) >> 6)];
      out[o] = rcm_base64_alpha[in[i] & 0x3f];
      break;
    }
  }
  switch (len % 3) {
  case 0:
    break;
  case 1:
    out[o++] = rcm_base64_alpha[(in[i - 1] & 0x03) << 4];
    out[o++] = rcm_base64_padding;
    out[o++] = rcm_base64_padding;
    break;
  case 2:
    out[o++] = rcm_base64_alpha[(in[i - 1] & 0x0f) << 2];
    out[o++] = rcm_base64_padding;
    break;
  }
  out[o] = '\0';
  return RCM_BASE64_OK;
}

RCM_API size_t rcm_base64_decode_len(const char *in, size_t len)
{
  size_t paddings = 0;
  if (len == 0) {
    return 0;
  }
  if (len < 4) {
    return 3; /* incomplete encoding */
  }
  if (in[len - 1] == rcm_base64_padding) {
    paddings++;
  }
  if (in[len - 2] == rcm_base64_padding) {
    paddings++;
  }
  /* A len which is not evenly divisible is makes no sense, the error will be
     caught in the actual decoding. */
  return len / 4 * 3 - paddings;
}

RCM_API rcm_base64_err_t rcm_base64_decode(unsigned char *out, const char *in,
                                           size_t len)
{
  size_t i, o;
  if (rcm_assert(in)) {
    return RCM_BASE64_ERR_FAILED_ASSERT;
  }
  /* RCM_BASE64_ERR_ILLEGAL_CHAR */
  for (i = 0, o = 0; i < len; i++, o++) {
  }
  return RCM_BASE64_OK;
}

RCM_API const char *rcm_base64_errstr(rcm_base64_err_t errnum)
{
  switch (errnum) {
  case RCM_BASE64_OK:
    return "";
  case RCM_BASE64_ERR_FAILED_ASSERT:
    return "rcm_base64: assertion failed";
  case RCM_BASE64_ERR_ILLEGAL_CHAR:
    return "rcm_base64: illegal character";
  }
  return "rcm_base64: undefined errnum";
}
