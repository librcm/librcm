/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_ASSERT_H
#define RCM_ASSERT_H

/* Documentation: https://librcm.org/#rcm_assert.h */

#ifndef RCM_API
#define RCM_API extern
#endif

#include <stdbool.h>

typedef enum {
  RCM_ASSERT_OK = 0,
  RCM_ASSERT_ERR_FAILED_ASSERT = -4
} rcm_assert_err_t;

/* Robust assertion macro. It is inspired by the "JPL Institutional Coding
   Standard for the C Programming Language", Rule 16,
   (see https://lars-lab.jpl.nasa.gov/JPL_Coding_Standard_C.pdf).

   Recommended use:

   [source,c]
   ----
   if (!rcm_assert(p >= 0)) {
     return RCM_ASSERT_ERR_FAILED;
   }
   ----

   Compile with -DNDEBUG for production and without for development and
   testing. */
#define rcm_assert(e) rcm_assert_handle((e), __FILE__, __LINE__, #e)

/* An assertion handler. Should return false to mark the assertion as failed.
   Should return true to ignore failed assertions completely. */
typedef bool rcm_assert_handler_func(const char *file, int line,
                                     const char *expr, void *data);

/* Set the assertion handler. The default is rcm_assert_default_handler. Set to
   NULL to disable logging of assertions. */
RCM_API void rcm_assert_set_handler(rcm_assert_handler_func *handler);

/* Set data pointer passed to the assertion handler. */
RCM_API void rcm_assert_set_handler_data(void *data);

/* The default assert handler. Prints the failed assertion to stderr and then
   calls abort(3) if not compiled with NDEBUG. */
RCM_API bool rcm_assert_default_handler(const char *file, int line,
                                        const char *expr, void *data);

/* Internal function. It calls the rcm_assert_handler, if not NULL and
   assert_result is false. */
RCM_API bool rcm_assert_handle(bool assert_result, const char *file, int line,
                               const char *expr);

/* Reset number of assertions. Debug only. */
#ifndef NDEBUG
RCM_API int rcm_assert_num_of_asserts(void);
#endif

/* Abort assertion n. Debug only. */
#ifndef NDEBUG
RCM_API void rcm_assert_abort(int n);
#endif

/* Reset assertion abortion. Debug only. */
#ifndef NDEBUG
RCM_API void rcm_assert_reset_abort(void);
#endif

#endif /* RCM_ASSERT_H */
