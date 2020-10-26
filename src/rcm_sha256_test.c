/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_sha256.h"

TEST rcm_sha256_test(void)
{
  /* ASSERT_EQ(23, rcm_mod_bar()); */
  PASS();
}

SUITE(rcm_sha256_suite)
{
  RUN_TEST(rcm_sha256_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_sha256_suite);
  GREATEST_MAIN_END();
}
