/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_mem.h"
#include "rcm_mfatest.h"

static int rcm_mem_test_malloc_internal(char *err)
{
  void *ptr = NULL;
  if (!(ptr = rcm_mem_malloc(4096))) {
    return RCM_MEM_ERR_NOMEM;
  }
  rcm_mem_free(ptr);
  return RCM_MEM_OK;
}

TEST rcm_mem_test_malloc(void)
{
  return rcm_mfatest_wrap(rcm_mem_test_malloc_internal);
}

TEST rcm_mem_test_null(void)
{
  rcm_mem_free(NULL);
  rcm_mem_freecharptr(NULL);
  rcm_mem_freeucharptr(NULL);
  PASS();
}

static void *rcm_mem_test_malloc_noop(size_t size)
{
  return NULL;
}

TEST rcm_mem_test_custom_malloc(void)
{
  rcm_mem_set_malloc(rcm_mem_test_malloc_noop);
  ASSERT(!rcm_mem_malloc(4096));
  rcm_mem_set_malloc(NULL);
  PASS();
}

SUITE(rcm_mem_suite)
{
  RUN_TEST(rcm_mem_test_malloc);
  RUN_TEST(rcm_mem_test_null);
  RUN_TEST(rcm_mem_test_custom_malloc);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_mem_suite);
  GREATEST_MAIN_END();
}
