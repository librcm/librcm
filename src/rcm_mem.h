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

#endif /* RCM_MEM_H */
