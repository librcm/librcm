/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#define RCM_API static

#include "rcm_errbuf.c"
#include "rcm_rfc3339.c"

#define RCM_RFC3339_EXAMPLE "2525-12-31T23:59:59Z"

TEST rcm_rfc3339_test_indefinite(void)
{
  rcm_rfc3339_t example = { 0 }, now;
  char str[RCM_RFC3339_BUFSIZE];
  char err[RCM_ERRBUF_SIZE];
  ASSERT(!rcm_rfc3339_parse(&example, RCM_RFC3339_EXAMPLE, err));
  ASSERT(!rcm_rfc3339_now(&now, err));
  ASSERT(rcm_rfc3339_after(example, now));
  ASSERT(!rcm_rfc3339_after(now, example));
  ASSERT(rcm_rfc3339_before(now, example));
  ASSERT(!rcm_rfc3339_before(example, now));
  rcm_rfc3339_format(str, example);
  ASSERT_STR_EQ(RCM_RFC3339_EXAMPLE, str);
  PASS();
}

TEST rcm_rfc3339_test_points(void)
{
  rcm_rfc3339_t start_of_day;
  rcm_rfc3339_t one_sec_before;
  rcm_rfc3339_t midnight;
  char err[RCM_ERRBUF_SIZE];
  ASSERT(!rcm_rfc3339_parse(&start_of_day, "2020-12-31T00:00:00Z", err));
  ASSERT(!rcm_rfc3339_parse(&one_sec_before, "2020-12-31T23:59:59Z", err));
  ASSERT(!rcm_rfc3339_parse(&midnight, "2021-01-01T00:00:00Z", err));
  ASSERT(rcm_rfc3339_before(start_of_day, one_sec_before));
  ASSERT(!rcm_rfc3339_after(start_of_day, one_sec_before));
  ASSERT(rcm_rfc3339_before(one_sec_before, midnight));
  ASSERT(!rcm_rfc3339_after(one_sec_before, midnight));
  PASS();
}

TEST rcm_rfc3339_test_from_time_t(void)
{
  rcm_rfc3339_from_time_t(time(NULL));
  PASS();
}

SUITE(rcm_rfc3339_suite)
{
  RUN_TEST(rcm_rfc3339_test_indefinite);
  RUN_TEST(rcm_rfc3339_test_points);
  RUN_TEST(rcm_rfc3339_test_from_time_t);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_rfc3339_suite);
  GREATEST_MAIN_END();
}
