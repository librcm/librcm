/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_MFATEST_H
#define RCM_MFATEST_H

/* Documentation: https://librcm.org/#rcm_mfatest.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
MFA test module:

Memory/file/assertion test wrapper module.
To be used for unit tests written with the C testing library `greatest.h`.
*/

#include "greatest.h"

/* Test function type. */
typedef int rcm_mfatest_func(char *err);

/* Memory/file/assertion test wrapper function. The expected error value is 0.
   Just wrap the test_func with it. */
RCM_API enum greatest_test_res rcm_mfatest_wrap(rcm_mfatest_func test_func);

/* Memory/file/assertion test wrapper function with expected error value rval.
   Just wrap the test_func with it. */
RCM_API enum greatest_test_res rcm_mfatest_wrap_err(rcm_mfatest_func test_func,
                                                    int rval);

#endif /* RCM_MFATEST_H */
