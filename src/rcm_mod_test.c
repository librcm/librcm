/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_mod.h"

TEST rcm_mod_test_foo(void)
{
  rcm_mod_foo(); /* nothing to test */
  PASS();
}

TEST rcm_mod_test_bar(void)
{
  ASSERT_EQ(23, rcm_mod_bar());
  PASS();
}

SUITE(rcm_mod_suite)
{
  RUN_TEST(rcm_mod_test_foo);
  RUN_TEST(rcm_mod_test_bar);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_mod_suite);
  GREATEST_MAIN_END();
}
