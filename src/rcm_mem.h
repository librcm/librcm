/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_MEM_H
#define RCM_MEM_H

/* Documentation: https://librcm.org/#rcm_mem.h */

#ifndef RCM_API
#define RCM_API extern
#endif

#include <stdlib.h>

RCM_API void *rcm_mem_malloc(size_t size);

RCM_API void rcm_mem_free(void *ptr);

#endif /* RCM_MEM_H */
