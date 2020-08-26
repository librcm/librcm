/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_MFATEST_H
#define RCM_MFATEST_H

/* Documentation: https://librcm.org/#rcm_mfatest.h */

#ifndef RCM_API
#define RCM_API extern
#endif

#include "greatest.h"

typedef int(mftest_func_t)(char *err);

/** Memory/file/assertion test wrapper function. Just wrap the test func with it. */
enum greatest_test_res rcm_mfatest_wrap(mftest_func_t test_func);

/** Memory/file/assertion test wrapper function with expected error value rval. */
enum greatest_test_res rcm_mfatest_wrap_err(mftest_func_t test_func, int rval);

#endif /* RCM_MFATEST_H */
