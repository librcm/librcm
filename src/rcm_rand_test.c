/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_errbuf.h"
#include "rcm_mfatest.h"
#include "rcm_rand.h"

static int rcm_rand_test_internal(char *err)
{
  unsigned char buf1[32], buf2[32];
  int rc;
  if ((rc = rcm_rand_bytes(buf1, 32))) {
    return rc;
  }
  if ((rc = rcm_rand_bytes(buf2, 32))) {
    return rc;
  }
  if (!memcmp(buf1, buf2, 32)) {
    rcm_errbuf_set(err, "random buffer differ");
    rc = -1;
  }
  return rc;
}

TEST rcm_rand_test(void)
{
  return rcm_mfatest_wrap(rcm_rand_test_internal);
}

TEST rcm_rand_test_errstr(void)
{
  ASSERT_STR_EQ("", rcm_rand_errstr(RCM_RAND_OK));
  ASSERT_STR_EQ("rcm_rand: undefined errnum", rcm_rand_errstr(-1));
  ASSERT_STR_EQ("rcm_rand: assertion failed",
                rcm_rand_errstr(RCM_RAND_ERR_FAILED_ASSERT));
  ASSERT_STR_EQ("rcm_rand: could not get enough random bytes",
                rcm_rand_errstr(RCM_RAND_ERR_RANDOM));
  PASS();
}

SUITE(rcm_rand_suite)
{
  RUN_TEST(rcm_rand_test);
  RUN_TEST(rcm_rand_test_errstr);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_rand_suite);
  GREATEST_MAIN_END();
}
