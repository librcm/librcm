/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_membuf.h"
#include "rcm_mfatest.h"

static int rcm_membuf_test_internal(char *err)
{
  void *buf;
  int rc;
  if ((rc = rcm_membuf_new(&buf, 1024, err))) {
    return rc;
  }
  rcm_membuf_free(buf);
  return rc;
}

TEST rcm_membuf_test(void)
{
  return rcm_mfatest_wrap(rcm_membuf_test_internal);
}

TEST rcm_membuf_test_errstr(void)
{
  ASSERT_STR_EQ("", rcm_membuf_errstr(RCM_MEMBUF_OK));
  ASSERT_STR_EQ("rcm_membuf: undefined errnum", rcm_membuf_errstr(-1));
  PASS();
}

SUITE(rcm_membuf_suite)
{
  RUN_TEST(rcm_membuf_test);
  RUN_TEST(rcm_membuf_test_errstr);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_membuf_suite);
  GREATEST_MAIN_END();
}
