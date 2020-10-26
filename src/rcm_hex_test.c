/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_hex.h"

TEST rcm_hex_test_parse_byte(void)
{
  unsigned char byte;
  int rc;
  ASSERT(!rcm_hex_parse_byte(&byte, "00"));
  ASSERT(byte == 0);
  ASSERT(!rcm_hex_parse_byte(&byte, "01"));
  ASSERT(byte == 1);
  ASSERT(!rcm_hex_parse_byte(&byte, "02"));
  ASSERT(byte == 2);
  ASSERT(!rcm_hex_parse_byte(&byte, "03"));
  ASSERT(byte == 3);
  ASSERT(!rcm_hex_parse_byte(&byte, "04"));
  ASSERT(byte == 4);
  ASSERT(!rcm_hex_parse_byte(&byte, "05"));
  ASSERT(byte == 5);
  ASSERT(!rcm_hex_parse_byte(&byte, "06"));
  ASSERT(byte == 6);
  ASSERT(!rcm_hex_parse_byte(&byte, "07"));
  ASSERT(byte == 7);
  ASSERT(!rcm_hex_parse_byte(&byte, "08"));
  ASSERT(byte == 8);
  ASSERT(!rcm_hex_parse_byte(&byte, "09"));
  ASSERT(byte == 9);
  ASSERT(!rcm_hex_parse_byte(&byte, "0a"));
  ASSERT(byte == 10);
  ASSERT(!rcm_hex_parse_byte(&byte, "0b"));
  ASSERT(byte == 11);
  ASSERT(!rcm_hex_parse_byte(&byte, "0c"));
  ASSERT(byte == 12);
  ASSERT(!rcm_hex_parse_byte(&byte, "0d"));
  ASSERT(byte == 13);
  ASSERT(!rcm_hex_parse_byte(&byte, "0e"));
  ASSERT(byte == 14);
  ASSERT(!rcm_hex_parse_byte(&byte, "0f"));
  ASSERT(byte == 15);
  ASSERT(!rcm_hex_parse_byte(&byte, "ff"));
  ASSERT(byte == 255);
  ASSERT(!rcm_hex_parse_byte(&byte, "FF"));
  ASSERT(byte == 255);
  ASSERT((rc = rcm_hex_parse_byte(&byte, "gg")));
  ASSERT(rc == RCM_HEX_ERR_PARSE);
  ASSERT((rc = rcm_hex_parse_byte(&byte, "0g")));
  ASSERT(rc == RCM_HEX_ERR_PARSE);
  ASSERT((rc = rcm_hex_parse_byte(NULL, "00")));
  ASSERT(rc == RCM_HEX_ERR_FAILED_ASSERT);
  PASS();
}

TEST rcm_hex_test_parse_str(void)
{
  unsigned char out[2];
  int rc;
  ASSERT((rc = rcm_hex_parse_str(NULL, "00", 2)));
  ASSERT(rc == RCM_HEX_ERR_FAILED_ASSERT);
  ASSERT((rc = rcm_hex_parse_str(out, "00X0", 4)));
  ASSERT(rc == RCM_HEX_ERR_PARSE);
  ASSERT(!rcm_hex_parse_str(out, "1020", 4));
  ASSERT(out[0] == 16);
  ASSERT(out[1] == 32);
  PASS();
}

SUITE(rcm_hex_suite)
{
  RUN_TEST(rcm_hex_test_parse_byte);
  RUN_TEST(rcm_hex_test_parse_str);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_hex_suite);
  GREATEST_MAIN_END();
}
