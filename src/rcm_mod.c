/* SPDX-License-Identifier: Unlicense OR MIT */

#include "rcm_mod.h"

static void rcm_mod_internal_foo(void)
{
  /* some internal stuff */
}

RCM_API void rcm_mod_foo(void)
{
  rcm_mod_internal_foo();
}

RCM_API int rcm_mod_bar(void)
{
  return 23;
}
