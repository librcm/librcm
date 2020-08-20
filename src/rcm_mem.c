/* SPDX-License-Identifier: Unlicense OR MIT */

#include "rcm_mem.h"

RCM_API void *rcm_mem_malloc(size_t size)
{
  return malloc(size);
}

RCM_API void rcm_mem_free(void *ptr)
{
  if (!ptr) {
    return;
  }
  free(ptr);
}
