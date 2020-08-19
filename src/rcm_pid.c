/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef _WIN32
#include <sys/types.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "rcm_pid.h"

RCM_API int rcm_pid_get(void)
{
#ifndef _WIN32
  return getpid();
#else
  return GetCurrentProcessId();
#endif
}
