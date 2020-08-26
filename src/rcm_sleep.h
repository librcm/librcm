/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_SLEEP_H
#define RCM_SLEEP_H

/* Documentation: https://librcm.org/#rcm_sleep.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Sleep module:

Implements a portable `sleep(3)` function.
*/

/* Makes the calling thread sleep until a given number of seconds have elapsed.
   On POSIX systems a signal which is not ignored interrupts the function. */
RCM_API void rcm_sleep(unsigned int seconds);

#endif /* RCM_SLEEP_H */
