/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_errbuf.h"

TEST rcm_errbuf_test(void)
{
  char err[RCM_ERRBUF_SIZE];
  rcm_errbuf_set(err, "test %d", 123);
  ASSERT_MEM_EQ("test 123", err, 8);
  rcm_errbuf_set_str(err, "test 456");
  ASSERT_MEM_EQ("test 456", err, 8);
  PASS();
}

TEST rcm_errbuf_test_null(void)
{
  rcm_errbuf_set(NULL, "test %d", 123);
  rcm_errbuf_set_str(NULL, "test");
  PASS();
}

SUITE(rcm_errbuf_suite)
{
  RUN_TEST(rcm_errbuf_test);
  RUN_TEST(rcm_errbuf_test_null);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_errbuf_suite);
  GREATEST_MAIN_END();
}
