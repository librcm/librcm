/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#define RCM_API static

#include "rcm_errbuf.c"

TEST rcm_errbuf_test(void)
{
  char err[RCM_ERRBUF_SIZE];
  rcm_errbuf_set(err, "test %d", 123);
  ASSERT_MEM_EQ("test 123", err, 8);
  PASS();
}

SUITE(rcm_errbuf_suite)
{
  RUN_TEST(rcm_errbuf_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_errbuf_suite);
  GREATEST_MAIN_END();
}
