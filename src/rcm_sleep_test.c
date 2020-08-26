/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_sleep.h"

TEST rcm_sleep_test(void)
{
  rcm_sleep(0);
  rcm_sleep(1);
  PASS();
}

SUITE(rcm_sleep_suite)
{
  RUN_TEST(rcm_sleep_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_sleep_suite);
  GREATEST_MAIN_END();
}
