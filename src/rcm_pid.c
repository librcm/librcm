/* SPDX-License-Identifier: Unlicense OR MIT */

#include <sys/types.h>
#include <unistd.h>

#include "rcm_pid.h"

RCM_API int rcm_pid_get(void)
{
#ifndef _WIN32
  return getpid();
#else
  return GetCurrentProcessId();
#endif
}
