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
    return 3; /* incomplete encoding, error will be caught when decoding */
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

/* generated with base64map.go */
static const unsigned char rcm_base64_map[256] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0xa,  0xb,  0xc,
  0xd,  0xe,  0xf,  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
  0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

RCM_API rcm_base64_err_t rcm_base64_decode(unsigned char *out, const char *in,
                                           size_t len)
{
  unsigned char carry = 0;
  size_t i, o;
  if (!rcm_assert(in)) {
    return RCM_BASE64_ERR_FAILED_ASSERT;
  }
  if (len == 0) {
    return RCM_BASE64_OK;
  }
  if (len % 4 != 0) {
    return RCM_BASE64_ERR_ILLEGAL_LENGTH;
  }
  for (i = 0, o = 0; i < len; i++, o++) {
    unsigned char dec;
    if ((i == len - 1 || i == len - 2) && in[i] == rcm_base64_padding) {
      if (i == len - 2 && in[i + 1] != rcm_base64_padding) {
        return RCM_BASE64_ERR_ILLEGAL_CHAR;
      }
      break;
    }
    dec = rcm_base64_map[(unsigned int)in[i]];
    if (dec == 0xff) {
      return RCM_BASE64_ERR_ILLEGAL_CHAR;
    }
    switch (i % 4) {
    case 0:
      out[o] = (unsigned char)(dec << 2);
      break;
    case 1:
      out[o - 1] |= (unsigned char)((dec & 0x30) >> 4);
      carry = (unsigned char)((dec & 0x0f) << 4);
      break;
    case 2:
      out[o - 1] = carry | dec >> 2;
      carry = (unsigned char)((dec & 0x03) << 6);
      o--;
      break;
    case 3:
      out[o] = carry | dec;
      break;
    }
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
  case RCM_BASE64_ERR_ILLEGAL_LENGTH:
    return "rcm_base64: illegal length (must be multiple of 4)";
  case RCM_BASE64_ERR_ILLEGAL_CHAR:
    return "rcm_base64: illegal character";
  }
  return "rcm_base64: undefined errnum";
}
