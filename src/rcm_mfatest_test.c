/* SPDX-License-Identifier: Unlicense OR MIT */

#include "rcm_assert.h"
#include "rcm_errbuf.h"
#include "rcm_mem.h"
#include "rcm_mfatest.h"

static int rcm_mfatest_test_internal(char *err)
{
#if 0
  FILE *fp = NULL;
#endif
  void *mem;
  int ret = 0;
  if (!rcm_assert(true)) {
    return RCM_ASSERT_ERR_FAILED_ASSERT;
  }
  if (!(mem = rcm_mem_malloc(4096))) {
    rcm_errbuf_set(err, "out of memory");
    ret = -2;
  }
#if 0
  if (!ret) {
    if (!(fp = file_fopen("/dev/null", "w", err))) {
      ret = -3;
    }
  }
  if (!ret) {
    ret = file_fclose(fp, err);
  } else {
    file_fclose(fp, NULL); /* close file no matter what */
  }
#endif
  rcm_mem_free(mem);
  return ret;
}

TEST rcm_mfatest_test(void)
{
  return rcm_mfatest_wrap(rcm_mfatest_test_internal);
}

SUITE(rcm_mfatest_suite)
{
  RUN_TEST(rcm_mfatest_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_mfatest_suite);
  GREATEST_MAIN_END();
}