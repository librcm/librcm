/* SPDX-License-Identifier: Unlicense OR MIT */

#include <assert.h>

#include "rcm_base64buf.h"

RCM_API rcm_base64buf_err_t rcm_base64buf_encode(char **out,
                                                 const unsigned char *in,
                                                 size_t len, char *err)
{
  char *enc;
  int rc;
  if (!rcm_assert(out && in)) {
    rcm_errbuf_set_str(err,
                       rcm_base64buf_errstr(RCM_BASE64BUF_ERR_FAILED_ASSERT));
    return RCM_BASE64BUF_ERR_FAILED_ASSERT;
  }
  if (!(enc = rcm_mem_malloc(rcm_base64_encode_len(len) + 1))) {
    rcm_errbuf_set_str(err, rcm_base64buf_errstr(RCM_BASE64BUF_ERR_NOMEM));
    return RCM_BASE64BUF_ERR_NOMEM;
  }
  if ((rc = rcm_base64_encode(enc, in, len))) {
    rcm_errbuf_set_str(err, rcm_base64buf_errstr(rc));
    rcm_mem_free(enc);
    return rc;
  }
  assert(out); /* scan-build, already checked above */
  *out = enc;
  return RCM_BASE64BUF_OK;
}

RCM_API rcm_base64buf_err_t rcm_base64buf_decode(unsigned char **out,
                                                 size_t *outlen, const char *in,
                                                 size_t len, char *err)
{
  unsigned char *dec;
  size_t declen;
  rcm_base64_err_t rc;
  if (!rcm_assert(out && in)) {
    rcm_errbuf_set_str(err,
                       rcm_base64buf_errstr(RCM_BASE64BUF_ERR_FAILED_ASSERT));
    return RCM_BASE64BUF_ERR_FAILED_ASSERT;
  }
  if ((rc = rcm_base64_decode_len(&declen, in, len))) {
    rcm_errbuf_set_str(err, rcm_base64_errstr(rc));
    return (rcm_base64buf_err_t)rc;
  }
  if (!(dec = rcm_mem_malloc(declen))) {
    rcm_errbuf_set_str(err, rcm_base64buf_errstr(RCM_BASE64BUF_ERR_NOMEM));
    return RCM_BASE64BUF_ERR_NOMEM;
  }
  if ((rc = rcm_base64_decode(dec, in, len))) {
    rcm_mem_free(dec);
    return (rcm_base64buf_err_t)rc;
  }
  if (outlen) {
    *outlen = declen;
  }
  assert(out); /* scan-build, already checked above */
  *out = dec;
  return RCM_BASE64BUF_OK;
}

RCM_API const char *rcm_base64buf_errstr(rcm_base64buf_err_t errnum)
{
  switch (errnum) {
  case RCM_BASE64BUF_OK:
    return "";
  case RCM_BASE64BUF_ERR_NOMEM:
    return "rcm_base64buf: out of memory";
  case RCM_BASE64BUF_ERR_FAILED_ASSERT:
    return "rcm_base64buf: assertion failed";
  case RCM_BASE64BUF_ERR_ILLEGAL_LENGTH:
    return rcm_base64_errstr((rcm_base64_err_t)errnum);
  case RCM_BASE64BUF_ERR_ILLEGAL_CHAR:
    return rcm_base64_errstr((rcm_base64_err_t)errnum);
  }
  return "rcm_base64buf: undefined errnum";
}
