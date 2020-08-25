/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_assert.h"

TEST rcm_assert_test_success(void)
{
  ASSERT_EQ(true, rcm_assert(true));
  PASS();
}

TEST rcm_assert_test_failure(void)
{
  ASSERT_EQ(false, rcm_assert(false));
  PASS();
}

TEST rcm_assert_test_no_handler_success(void)
{
  rcm_assert_set_handler(NULL);
  ASSERT_EQ(true, rcm_assert(true));
  PASS();
}

TEST rcm_assert_test_no_handler_failure(void)
{
  rcm_assert_set_handler(NULL);
  ASSERT_EQ(false, rcm_assert(false));
  PASS();
}

static bool rcm_assert_test_handler(const char *file, int line,
                                    const char *expr, void *data)
{
  bool *called = data;
  *called = true;
  return false;
}

TEST rcm_assert_test_handler_success(void)
{
  bool called = false;
  rcm_assert_set_handler(rcm_assert_test_handler);
  rcm_assert_set_handler_data(&called);
  ASSERT_EQ(true, rcm_assert(true));
  ASSERT_EQ(false, called);
  PASS();
}

TEST rcm_assert_test_handler_failure(void)
{
  bool called = false;
  rcm_assert_set_handler(rcm_assert_test_handler);
  rcm_assert_set_handler_data(&called);
  ASSERT_EQ(false, rcm_assert(false));
  ASSERT_EQ(true, called);
  PASS();
}

SUITE(rcm_assert_suite)
{
  RUN_TEST(rcm_assert_test_success);
  RUN_TEST(rcm_assert_test_failure);
  RUN_TEST(rcm_assert_test_no_handler_success);
  RUN_TEST(rcm_assert_test_no_handler_failure);
  RUN_TEST(rcm_assert_test_handler_success);
  RUN_TEST(rcm_assert_test_handler_failure);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_assert_suite);
  GREATEST_MAIN_END();
}
