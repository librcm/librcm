/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_mfatest.h"
#include "rcm_rfc3339.h"

#define RCM_RFC3339_EXAMPLE "2525-12-31T23:59:59Z"

static int rcm_rfc3339_test_indefinite_internal(char *err)
{
  rcm_rfc3339_t example = { 0 }, now;
  char str[RCM_RFC3339_BUFSIZE];
  int rc;
  if ((rc = rcm_rfc3339_parse(&example, RCM_RFC3339_EXAMPLE, err))) {
    return rc;
  }
  if ((rc = rcm_rfc3339_now(&now, err))) {
    return rc;
  }
  if (!rcm_rfc3339_after(example, now)) {
    rcm_errbuf_set(err, "!rcm_rfc3339_after(example, now)");
    return -1;
  }
  if (rcm_rfc3339_after(now, example)) {
    rcm_errbuf_set(err, "rcm_rfc3339_after(now, example)");
    return -1;
  }
  if (!rcm_rfc3339_before(now, example)) {
    rcm_errbuf_set(err, "!rcm_rfc3339_before(now, example)");
    return -1;
  }
  if (rcm_rfc3339_before(example, now)) {
    rcm_errbuf_set(err, "rcm_rfc3339_before(example, now)");
    return -1;
  }
  if ((rc = rcm_rfc3339_format(str, example, err))) {
    return rc;
  }
  if (strcmp(RCM_RFC3339_EXAMPLE, str)) {
    rcm_errbuf_set(err, "strcmp(RCM_RFC3339_EXAMPLE, str)");
    return -1;
  }
  return rc;
}

TEST rcm_rfc3339_test_indefinite(void)
{
  return rcm_mfatest_wrap(rcm_rfc3339_test_indefinite_internal);
}

static int rcm_rfc3339_test_points_internal(char *err)
{
  rcm_rfc3339_t start_of_day;
  rcm_rfc3339_t one_sec_before;
  rcm_rfc3339_t midnight;
  int rc;
  if ((rc = rcm_rfc3339_parse(&start_of_day, "2020-12-31T00:00:00Z", err))) {
    return rc;
  }
  if ((rc = rcm_rfc3339_parse(&one_sec_before, "2020-12-31T23:59:59Z", err))) {
    return rc;
  }
  if ((rc = rcm_rfc3339_parse(&midnight, "2021-01-01T00:00:00Z", err))) {
    return rc;
  }
  if (!rcm_rfc3339_before(start_of_day, one_sec_before)) {
    rcm_errbuf_set(err, "!rcm_rfc3339_before(start_of_day, one_sec_before)");
    return -1;
  }
  if (rcm_rfc3339_after(start_of_day, one_sec_before)) {
    rcm_errbuf_set(err, "rcm_rfc3339_after(start_of_day, one_sec_before)");
    return -1;
  }
  if (!rcm_rfc3339_before(one_sec_before, midnight)) {
    rcm_errbuf_set(err, "rcm_rfc3339_before(one_sec_before, midnight)");
    return -1;
  }
  if (rcm_rfc3339_after(one_sec_before, midnight)) {
    rcm_errbuf_set(err, "rcm_rfc3339_after(one_sec_before, midnight)");
    return -1;
  }
  return rc;
}

TEST rcm_rfc3339_test_points(void)
{
  return rcm_mfatest_wrap(rcm_rfc3339_test_points_internal);
}

TEST rcm_rfc3339_test_from_time_t(void)
{
  rcm_rfc3339_from_time_t(time(NULL));
  PASS();
}

TEST rcm_rfc3339_test_errstr(void)
{
  ASSERT_STR_EQ("", rcm_rfc3339_errstr(RCM_RFC3339_OK));
  PASS();
}

SUITE(rcm_rfc3339_suite)
{
  RUN_TEST(rcm_rfc3339_test_indefinite);
  RUN_TEST(rcm_rfc3339_test_points);
  RUN_TEST(rcm_rfc3339_test_from_time_t);
  RUN_TEST(rcm_rfc3339_test_errstr);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_rfc3339_suite);
  GREATEST_MAIN_END();
}
