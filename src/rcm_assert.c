/* SPDX-License-Identifier: Unlicense OR MIT */

#include "rcm_assert.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG
static int rcm_assert_num_of_assertions = 0;
static int rcm_assert_abort_assertion = 0;
static bool rcm_assert_abort_in_progress = false;
#endif

static rcm_assert_handler_func *rcm_assert_handler = rcm_assert_default_handler;

static void *rcm_assert_handler_data = NULL;

RCM_API void rcm_assert_set_handler(rcm_assert_handler_func *handler)
{
  rcm_assert_handler = handler;
}

RCM_API void rcm_assert_set_handler_data(void *data)
{
  rcm_assert_handler_data = data;
}

RCM_API bool rcm_assert_default_handler(const char *file, int line,
                                        const char *expr, void *data)
{
  fprintf(stderr, "%s,%d: assertion '%s' failed\n", file, line, expr);
#ifndef NDEBUG
  abort();
#endif
  return false;
}

RCM_API bool rcm_assert_handle(bool assert_result, const char *file, int line,
                               const char *expr)
{
  bool res;
#ifndef NDEBUG
  if (rcm_assert_abort_assertion &&
      rcm_assert_num_of_assertions + 1 == rcm_assert_abort_assertion) {
    assert(!rcm_assert_abort_in_progress);
    rcm_assert_abort_in_progress = true;
    fprintf(stderr, "%s,%d: assertion '%s' failed (simulated)\n", file, line,
            expr);
    return false;
  }
#endif
  if (assert_result) {
    res = true;
  } else if (rcm_assert_handler) {
    res = rcm_assert_handler(file, line, expr, rcm_assert_handler_data);
  } else {
    res = false;
  }
#ifndef NDEBUG
  rcm_assert_num_of_assertions++;
#endif
  return res;
}

#ifndef NDEBUG
RCM_API int rcm_assert_num_of_asserts(void)
{
  int n = rcm_assert_num_of_assertions;
  rcm_assert_num_of_assertions = 0;
  return n;
}
#endif

#ifndef NDEBUG
RCM_API void rcm_assert_abort(int n)
{
  rcm_assert_abort_assertion = n;
}
#endif

#ifndef NDEBUG
RCM_API void rcm_assert_reset_abort(void)
{
  rcm_assert_abort_assertion = 0;
  rcm_assert_abort_in_progress = false;
}
#endif
