/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_MEM_H
#define RCM_MEM_H

/* Documentation: https://librcm.org/#rcm_mem.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Memory allocator:

This modules defines the memory allocator for _reusable C modules_.
*/

#include <stdlib.h>

/* Allocates size bytes and returns a pointer to the allocated memory.
   The memory is not initialized. */
RCM_API void *rcm_mem_malloc(size_t size);

/* Frees the memory space pointed to by ptr. */
RCM_API void rcm_mem_free(void *ptr);

/* Reset number of allocations. Debug only.
   Returns the total number of allocations so far. */
#ifndef NDEBUG
RCM_API int rcm_mem_num_of_allocs(void);
#endif

/*  Abort memory allocation. Debug only.
    Parameter n is the memory allocation to abort. */
#ifndef NDEBUG
RCM_API void rcm_mem_abort(int n);
#endif

/* Reset memory allocation abortion. Debug only. */
#ifndef NDEBUG
RCM_API void rcm_mem_reset_abort(void);
#endif

#endif /* RCM_MEM_H */
