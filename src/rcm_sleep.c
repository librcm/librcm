/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "rcm_sleep.h"

RCM_API void rcm_sleep(unsigned int seconds)
{
#ifndef _WIN32
  (void)sleep(seconds);
#else
  Sleep((DWORD)seconds * 1000);
#endif
}
