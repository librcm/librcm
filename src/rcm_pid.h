/* SPDX-License-Identifier: Unlicense OR MIT */

#ifndef RCM_PID_H
#define RCM_PID_H

/* Documentation: https://librcm.org/#rcm_pid.h */

#ifndef RCM_API
#define RCM_API extern
#endif

/*
Process ID:

Module to deal with process IDs (PIDs).
*/

/* Returns the process ID (PID) of the calling process. */
RCM_API int rcm_pid_get(void);

#endif /* RCM_PID_H */
