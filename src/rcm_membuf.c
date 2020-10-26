/* SPDX-License-Identifier: Unlicense OR MIT */

#include "rcm_membuf.h"

RCM_API rcm_membuf_err_t rcm_membuf_new(void **out, size_t size, char *err)
{
  void *buf;
  if (!rcm_assert(out)) {
    rcm_errbuf_set_str(err, rcm_membuf_errstr(RCM_MEMBUF_ERR_FAILED_ASSERT));
    return RCM_MEMBUF_ERR_FAILED_ASSERT;
  }
  if (!(buf = rcm_mem_calloc(1, size))) {
    rcm_errbuf_set_str(err, rcm_membuf_errstr(RCM_MEMBUF_ERR_NOMEM));
    return RCM_MEMBUF_ERR_NOMEM;
  }
  *out = buf;
  return RCM_MEMBUF_OK;
}

RCM_API void rcm_membuf_free(void *buf)
{
  rcm_mem_free(buf);
}

RCM_API const char *rcm_membuf_errstr(rcm_membuf_err_t errnum)
{
  switch (errnum) {
  case RCM_MEMBUF_OK:
    return "";
  case RCM_MEMBUF_ERR_NOMEM:
    return "rcm_membuf: out of memory";
  case RCM_MEMBUF_ERR_FAILED_ASSERT:
    return "rcm_membuf: assertion failed";
  }
  return "rcm_membuf: undefined errnum";
}