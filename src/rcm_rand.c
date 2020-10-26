/* SPDX-License-Identifier: Unlicense OR MIT */

#include <stdio.h>
#ifdef _WIN32
#define _WINSOCKAPI_ /* stops windows.h including winsock.h */
#include <windows.h>

#include <wincrypt.h>
#endif

#ifdef _MSC_VER
/* Link with Advapi32.lib */
#pragma comment(lib, "Advapi32.lib")
#endif

#include "rcm_rand.h"

#ifndef _WIN32
/* on *NIX read /dev/random */
static size_t rcm_rand_nix(unsigned char *buf, size_t len)
{
  FILE *f;
  size_t x;

  f = fopen("/dev/urandom", "rb");
  if (f == NULL) {
    f = fopen("/dev/random", "rb");
  }

  if (f == NULL) {
    return 0;
  }

  /* disable buffering */
  if (setvbuf(f, NULL, _IONBF, 0) != 0) {
    fclose(f);
    return 0;
  }

  x = fread(buf, 1, len, f);
  fclose(f);
  return x;
}
#endif /* _WIN32 */

#ifdef _WIN32
static size_t rand_win32(unsigned char *buf, size_t len)
{
  HCRYPTPROV hProv = 0;
  if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL,
                           (CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) &&
      !CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL,
                           CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET |
                               CRYPT_NEWKEYSET))
    return 0;

  if (CryptGenRandom(hProv, len, buf) == TRUE) {
    CryptReleaseContext(hProv, 0);
    return len;
  } else {
    CryptReleaseContext(hProv, 0);
    return 0;
  }
}
#endif /* _WIN32 */

static size_t rcm_rand_read(unsigned char *out, size_t len)
{
  size_t x;
#ifndef _WIN32
  x = rcm_rand_nix(out, len);
  if (x != 0) {
    return x;
  }
#else
  x = rcm_rand_win32(out, len);
  if (x != 0) {
    return x;
  }
#endif
  return 0;
}

RCM_API rcm_rand_err_t rcm_rand_bytes(unsigned char *out, size_t len)
{
  if (!rcm_assert(out)) {
    return RCM_RAND_ERR_FAILED_ASSERT;
  }
  if (rcm_rand_read(out, len) != len) {
    return RCM_RAND_ERR_RANDOM;
  }
  return RCM_RAND_OK;
}

RCM_API const char *rcm_rand_errstr(rcm_rand_err_t errnum)
{
  switch (errnum) {
  case RCM_RAND_OK:
    return "";
  case RCM_RAND_ERR_FAILED_ASSERT:
    return "rcm_rand: assertion failed";
  case RCM_RAND_ERR_RANDOM:
    return "rcm_rand: could not get enough random bytes";
  }
  return "rcm_rand: undefined errnum";
}
