/* SPDX-License-Identifier: Unlicense OR MIT */

#include <assert.h>
#include <stdbool.h>

#include "rcm_mem.h"

#ifndef NDEBUG
static int rcm_mem_num_of_mallocs = 0;
static int rcm_mem_abort_malloc = 0;
static bool rcm_mem_abort_in_progress = false;
#endif

static rcm_mem_malloc_func *rcm_mem_internal_malloc = malloc;
static rcm_mem_calloc_func *rcm_mem_internal_calloc = calloc;

RCM_API void *rcm_mem_malloc(size_t size)
{
  void *p;
#ifndef NDEBUG
  if (rcm_mem_abort_malloc &&
      rcm_mem_num_of_mallocs + 1 == rcm_mem_abort_malloc) {
    assert(!rcm_mem_abort_in_progress);
    rcm_mem_abort_in_progress = true;
    return NULL;
  }
#endif
  if (!(p = rcm_mem_internal_malloc(size))) {
    return NULL;
  }
#ifndef NDEBUG
  rcm_mem_num_of_mallocs++;
#endif
  return p;
}

RCM_API void *rcm_mem_calloc(size_t nmemb, size_t size)
{
  void *p;
#ifndef NDEBUG
  if (rcm_mem_abort_malloc &&
      rcm_mem_num_of_mallocs + 1 == rcm_mem_abort_malloc) {
    assert(!rcm_mem_abort_in_progress);
    rcm_mem_abort_in_progress = true;
    return NULL;
  }
#endif
  if (!(p = rcm_mem_internal_calloc(nmemb, size))) {
    return NULL;
  }
#ifndef NDEBUG
  rcm_mem_num_of_mallocs++;
#endif
  return p;
}

RCM_API void rcm_mem_free(void *ptr)
{
  if (!ptr) {
    return;
  }
  free(ptr);
}

RCM_API void rcm_mem_freecharptr(char **ptr)
{
  if (!ptr) {
    return;
  }
  rcm_mem_free(*ptr);
  *ptr = NULL;
}

RCM_API void rcm_mem_freeucharptr(unsigned char **ptr)
{
  if (!ptr) {
    return;
  }
  rcm_mem_free(*ptr);
  *ptr = NULL;
}

RCM_API void rcm_mem_set_malloc(rcm_mem_malloc_func *func)
{
  if (func) {
    rcm_mem_internal_malloc = func;
  } else {
    rcm_mem_internal_malloc = malloc;
  }
}

RCM_API void rcm_mem_set_calloc(rcm_mem_calloc_func *func)
{
  if (func) {
    rcm_mem_internal_calloc = func;
  } else {
    rcm_mem_internal_calloc = calloc;
  }
}

#ifndef NDEBUG
RCM_API int rcm_mem_num_of_allocs(void)
{
  int n = rcm_mem_num_of_mallocs;
  rcm_mem_num_of_mallocs = 0;
  return n;
}
#endif

#ifndef NDEBUG
RCM_API void rcm_mem_abort(int n)
{
  rcm_mem_abort_malloc = n;
}
#endif

#ifndef NDEBUG
RCM_API void rcm_mem_reset_abort(void)
{
  rcm_mem_abort_malloc = 0;
  rcm_mem_abort_in_progress = false;
}
#endif
