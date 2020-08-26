/* SPDX-License-Identifier: Unlicense OR MIT */

#include <assert.h>

#include "rcm_assert.h"
#include "rcm_errbuf.h"
#include "rcm_mem.h"
#include "rcm_mfatest.h"

enum greatest_test_res rcm_mfatest_wrap(rcm_mfatest_func_t test_func)
{
  return rcm_mfatest_wrap_err(test_func, 0);
}

enum greatest_test_res rcm_mfatest_wrap_err(rcm_mfatest_func_t test_func,
                                            int rval)
{
#ifndef NDEBUG
  int i, mallocs, /* io_ops, */ asserts, old_err;
#endif
  char err[RCM_ERRBUF_SIZE];
  int rc;
#ifdef NDEBUG
  rc = test_func(err);
  if (rc != rval) {
    fprintf(stderr, "error: %s\n", err);
  }
  ASSERT_EQ(rval, rc);
#else
  assert(test_func);
  rcm_mem_num_of_allocs();
  /* file_num_of_ops(); */
  rcm_assert_num_of_asserts();
  old_err = test_func(err);
  mallocs = rcm_mem_num_of_allocs();
  /* io_ops = file_num_of_ops(); */
  asserts = rcm_assert_num_of_asserts();
  /* ASSERT(!file_num_of_open_fps()); */

#ifdef MFTEST_DEBUG
  printf("num_of_mallocs=%d, num_of_io_ops=%d, asserts=%d\n", mallocs, io_ops,
         asserts);
#endif

  /* abort malloc # */
  for (i = 0; i < mallocs; i++) {
    rcm_mem_reset_abort();
    rcm_mem_abort(i + 1);
#ifdef MFTEST_DEBUG
    printf("abort malloc %d\n", i + 1);
#endif
    rc = test_func(err);
    ASSERT_EQ(-2, rc);
    rcm_mem_num_of_allocs();
    /* file_num_of_ops(); */
    rcm_assert_num_of_asserts();
    /* ASSERT(!file_num_of_open_fps()); */
  }
  rcm_mem_reset_abort();

  /* abort IO op # */
#if 0
  for (i = 0; i < io_ops; i++) {
    file_abort(i + 1);
#ifdef MFTEST_DEBUG
    printf("abort IO op %d\n", i + 1);
#endif
    rc = test_func(err);
    ASSERT_EQ(-3, rc);
    rcm_mem_num_of_allocs();
    file_num_of_ops();
    rcm_assert_num_of_asserts();
    ASSERT(!file_num_of_open_fps());
  }
  rcm_mem_reset_abort();
  file_reset_abort();
#endif

  /* abort assertion */
  for (i = 0; i < asserts; i++) {
    rcm_assert_reset_abort();
    rcm_assert_abort(i + 1);
#ifdef MFTEST_DEBUG
    printf("abort assertion %d\n", i + 1);
#endif
    rc = test_func(err);
    ASSERT_EQ(RCM_ASSERT_ERR_FAILED_ASSERT, rc);
    rcm_mem_num_of_allocs();
    /* file_num_of_ops(); */
    rcm_assert_num_of_asserts();
    /* ASSERT(!file_num_of_open_fps()); */
  }
  rcm_mem_reset_abort();
  /* file_reset_abort(); */
  rcm_assert_reset_abort();

  /* call again and make sure the error code is the same */
  rc = test_func(err);
  ASSERT_EQ(old_err, rc);
  rcm_mem_num_of_allocs();
  /* file_num_of_ops(); */
  rcm_assert_num_of_asserts();
  /* ASSERT(!file_num_of_open_fps()); */

  /* at last make sure return value is as exected */
#ifdef MFTEST_DEBUG
  if (rc)
    puts(err);
  printf("rval=%d, rc=%d\n", rval, rc);
#endif
  ASSERT_EQ(rval, rc);
#endif

  PASS();
}
