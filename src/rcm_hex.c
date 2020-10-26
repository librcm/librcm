/* SPDX-License-Identifier: Unlicense OR MIT */

#include "rcm_hex.h"

static int rcm_hex_parse_num(unsigned char *byte, const char bytechar)
{
  switch (bytechar) {
  case '0':
    *byte = 0;
    return 0;
  case '1':
    *byte = 1;
    return 0;
  case '2':
    *byte = 2;
    return 0;
  case '3':
    *byte = 3;
    return 0;
  case '4':
    *byte = 4;
    return 0;
  case '5':
    *byte = 5;
    return 0;
  case '6':
    *byte = 6;
    return 0;
  case '7':
    *byte = 7;
    return 0;
  case '8':
    *byte = 8;
    return 0;
  case '9':
    *byte = 9;
    return 0;
  case 'a':
  case 'A':
    *byte = 10;
    return 0;
  case 'b':
  case 'B':
    *byte = 11;
    return 0;
  case 'c':
  case 'C':
    *byte = 12;
    return 0;
  case 'd':
  case 'D':
    *byte = 13;
    return 0;
  case 'e':
  case 'E':
    *byte = 14;
    return 0;
  case 'f':
  case 'F':
    *byte = 15;
    return 0;
  default:
    return -1;
  }
}

RCM_API int rcm_hex_parse_byte(unsigned char *byte, const char *bytestr)
{
  unsigned char high;
  if (!rcm_assert(byte && bytestr)) {
    return RCM_HEX_ERR_FAILED_ASSERT;
  }
  if (rcm_hex_parse_num(&high, bytestr[0])) {
    return RCM_HEX_ERR_PARSE;
  }
  if (rcm_hex_parse_num(byte, bytestr[1])) {
    return RCM_HEX_ERR_PARSE;
  }
  *byte = high * 16 + *byte;
  return RCM_HEX_OK;
}

RCM_API int rcm_hex_parse_str(unsigned char *out, const char *hex, size_t len)
{
  size_t i, outlen;
  if (!rcm_assert(out && hex && len % 2 == 0)) {
    return RCM_HEX_ERR_FAILED_ASSERT;
  }
  outlen = len / 2;
  for (i = 0; i < outlen; i++) {
    int rc;
    if ((rc = rcm_hex_parse_byte(out + i, hex + i * 2))) {
      return rc;
    }
  }
  return RCM_HEX_OK;
}
