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

/* The memory error type. */
typedef enum { RCM_MEM_OK = 0, RCM_MEM_ERR_NOMEM = -2 } rcm_mem_err_t;

/* Memory allocator (malloc(3)-like). */
typedef void *rcm_mem_malloc_func(size_t size);

/* Allocates size bytes and returns a pointer to the allocated memory.
   The memory is not initialized. */
RCM_API void *rcm_mem_malloc(size_t size);

/* Frees the memory space pointed to by ptr. */
RCM_API void rcm_mem_free(void *ptr);

/* Free the memory space pointed to by ptr and set ptr to `NULL`. */
RCM_API void rcm_mem_freecharptr(char **ptr);

/* Free the memory space pointed to by ptr and set ptr to `NULL`. */
RCM_API void rcm_mem_freeucharptr(unsigned char **ptr);

/* Set malloc function used by this module.
   If func is `NULL` the default allocator is set. */
RCM_API void rcm_mem_set_malloc(rcm_mem_malloc_func *func);

/* Reset number of allocations. Debug only.
   Returns the total number of allocations so far. */
#ifndef NDEBUG
RCM_API int rcm_mem_num_of_allocs(void);
#endif

/* Abort memory allocation. Debug only.
   Parameter n is the memory allocation to abort. */
#ifndef NDEBUG
RCM_API void rcm_mem_abort(int n);
#endif

/* Reset memory allocation abortion. Debug only. */
#ifndef NDEBUG
RCM_API void rcm_mem_reset_abort(void);
#endif

#endif /* RCM_MEM_H */
