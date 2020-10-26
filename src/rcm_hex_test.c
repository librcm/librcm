/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_hex.h"

TEST rcm_hex_test_parse_byte(void)
{
  unsigned char byte;
  ASSERT(!rcm_hex_parse_byte(&byte, "00"));
  ASSERT(byte == 0);
  ASSERT(!rcm_hex_parse_byte(&byte, "ff"));
  ASSERT(byte == 255);
  ASSERT(!rcm_hex_parse_byte(&byte, "FF"));
  ASSERT(byte == 255);
  ASSERT(rcm_hex_parse_byte(&byte, "gg"));
  PASS();
}

SUITE(rcm_hex_suite)
{
  RUN_TEST(rcm_hex_test_parse_byte);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_hex_suite);
  GREATEST_MAIN_END();
}
