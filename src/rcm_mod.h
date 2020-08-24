/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_MOD_H
#define RCM_MOD_H

/* Documentation: https://librcm.org/#rcm_mod.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/* Example module:

   Example module which shows how to write _reusable C modules_. */


/* Does nothing. */
RCM_API void rcm_mod_foo(void);

/* Returns 23. */
RCM_API int rcm_mod_bar(void);

#endif /* RCM_MOD_H */
