/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_pid.h"

TEST rcm_pid_test(void)
{
  int pid = rcm_pid_get();
  ASSERT(pid > 0);
  PASS();
}

SUITE(rcm_pid_suite)
{
  RUN_TEST(rcm_pid_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_pid_suite);
  GREATEST_MAIN_END();
}
