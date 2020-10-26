/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_base64buf.h"
#include "rcm_errbuf.h"
#include "rcm_mfatest.h"

#define RCM_BASE64BUF_TEST_CASES 7

static const char *rcm_base64buf_test_strings[] = { "",      "f",    "fo",
                                                    "foo",   "foob", "fooba",
                                                    "foobar" };

static const char *rcm_base64buf_test_results[] = {
  "", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
};

static int rcm_base64buf_test_encode_internal(char *err)
{
  char *out = NULL;
  size_t i;
  int rc = 0;

  for (i = 0; i < RCM_BASE64BUF_TEST_CASES; i++) {
    const char *str = rcm_base64buf_test_strings[i];
    puts(str);
    if ((rc = rcm_base64buf_encode(&out, (const unsigned char *)str,
                                   strlen(str), err))) {
      return rc;
    }
    if (strcmp(rcm_base64buf_test_results[i], out)) {
      rcm_errbuf_set(err, "want: %s\nhave: %s", rcm_base64buf_test_results[i],
                     out);
      rc = -1;
      goto error;
    }
    rcm_mem_freecharptr(&out);
  }
error:
  rcm_mem_free(out);
  return rc;
}

TEST rcm_base64buf_test_encode(void)
{
  return rcm_mfatest_wrap(rcm_base64buf_test_encode_internal);
}

#define RCM_BASE64BUF_TEST_WIKIPEDIA_CASES 8

static const char *rcm_base64buf_test_wikipedia_strings[] = {
  "sure.", "sure", "sur", "su", "leasure.", "easure.", "asure.", "sure."
};

static const char *rcm_base64buf_test_wikipedia_results[] = {
  "c3VyZS4=",     "c3VyZQ==",     "c3Vy",     "c3U=",
  "bGVhc3VyZS4=", "ZWFzdXJlLg==", "YXN1cmUu", "c3VyZS4="
};

static int rcm_base64buf_test_encode_wikipedia_internal(char *err)
{
  char *out = NULL;
  size_t i;
  int rc = 0;
  for (i = 0; i < RCM_BASE64BUF_TEST_WIKIPEDIA_CASES; i++) {
    const char *str = rcm_base64buf_test_wikipedia_strings[i];
    if ((rc = rcm_base64buf_encode(&out, (const unsigned char *)str,
                                   strlen(str), err))) {
      return rc;
    }
    if (strcmp(rcm_base64buf_test_wikipedia_results[i], out)) {
      rcm_errbuf_set(err, "want: %s\nhave: %s",
                     rcm_base64buf_test_wikipedia_results[i], out);
      rc = -1;
      goto error;
    }
    rcm_mem_freecharptr(&out);
  }
error:
  rcm_mem_free(out);
  return rc;
}

TEST rcm_base64buf_test_encode_wikipedia(void)
{
  return rcm_mfatest_wrap(rcm_base64buf_test_encode_wikipedia_internal);
}

static int rcm_base64buf_test_decode_internal(char *err)
{
  unsigned char *out = NULL;
  size_t i, outlen;
  int rc = 0;
  for (i = 0; i < RCM_BASE64BUF_TEST_CASES; i++) {
    const char *res = rcm_base64buf_test_results[i];
    if ((rc = rcm_base64buf_decode(&out, &outlen, res, strlen(res), err))) {
      return rc;
    }
    if (strncmp(rcm_base64buf_test_strings[i], (const char *)out, outlen)) {
      rcm_errbuf_set(err, "want: %s\nhave: %s", rcm_base64buf_test_strings[i],
                     out);
      rc = -1;
      goto error;
    }
    rcm_mem_freeucharptr(&out);
  }
error:
  rcm_mem_free(out);
  return rc;
}

TEST rcm_base64buf_test_decode(void)
{
  return rcm_mfatest_wrap(rcm_base64buf_test_decode_internal);
}

static int rcm_base64buf_test_decode_wikipedia_internal(char *err)
{
  unsigned char *out = NULL;
  size_t i, outlen;
  int rc = 0;
  for (i = 0; i < RCM_BASE64BUF_TEST_WIKIPEDIA_CASES; i++) {
    const char *res = rcm_base64buf_test_wikipedia_results[i];
    if ((rc = rcm_base64buf_decode(&out, NULL, res, strlen(res), err))) {
      return rc;
    }
    if ((rc = rcm_base64_decode_len(&outlen, res, strlen(res)))) {
      goto error;
    }
    if (strncmp(rcm_base64buf_test_wikipedia_strings[i], (const char *)out,
                outlen)) {
      rcm_errbuf_set(err, "want: %s\nhave: %s",
                     rcm_base64buf_test_wikipedia_strings[i], out);
      rc = -1;
      goto error;
    }
    rcm_mem_freeucharptr(&out);
  }
error:
  rcm_mem_free(out);
  return rc;
}

TEST rcm_base64buf_test_decode_wikipedia(void)
{
  return rcm_mfatest_wrap(rcm_base64buf_test_decode_wikipedia_internal);
}

TEST rcm_base64buf_test_errstr(void)
{
  ASSERT_STR_EQ("", rcm_base64buf_errstr(RCM_BASE64BUF_OK));
  ASSERT_STR_EQ("rcm_base64buf: undefined errnum", rcm_base64buf_errstr(-1));
  ASSERT_STR_EQ("rcm_base64: illegal length (must be multiple of 4)",
                rcm_base64buf_errstr(RCM_BASE64BUF_ERR_ILLEGAL_LENGTH));
  ASSERT_STR_EQ("rcm_base64: illegal character",
                rcm_base64buf_errstr(RCM_BASE64BUF_ERR_ILLEGAL_CHAR));
  PASS();
}

SUITE(rcm_base64buf_suite)
{
  RUN_TEST(rcm_base64buf_test_encode);
  RUN_TEST(rcm_base64buf_test_encode_wikipedia);
  RUN_TEST(rcm_base64buf_test_decode);
  RUN_TEST(rcm_base64buf_test_decode_wikipedia);
  RUN_TEST(rcm_base64buf_test_errstr);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_base64buf_suite);
  GREATEST_MAIN_END();
}
