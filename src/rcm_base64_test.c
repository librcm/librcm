/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_base64.h"
#include "rcm_errbuf.h"
#include "rcm_mfatest.h"

TEST rcm_base64_test_encode_len(void)
{
  ASSERT_EQ(0, rcm_base64_encode_len(strlen("")));
  ASSERT_EQ(4, rcm_base64_encode_len(strlen("f")));
  ASSERT_EQ(4, rcm_base64_encode_len(strlen("fo")));
  ASSERT_EQ(4, rcm_base64_encode_len(strlen("foo")));
  ASSERT_EQ(8, rcm_base64_encode_len(strlen("foob")));
  ASSERT_EQ(8, rcm_base64_encode_len(strlen("fooba")));
  ASSERT_EQ(8, rcm_base64_encode_len(strlen("foobar")));
  PASS();
}

#define RCM_BASE64_TEST_RFC3339_CASES 7

static const char *rcm_base64_test_rfc3339_strings[] = {
  "", "f", "fo", "foo", "foob", "fooba", "foobar"
};

static const char *rcm_base64_test_rfc3339_results[] = {
  "", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
};

static int rcm_base64_test_encode_rfc3339_internal(char *err)
{
  char out[9];
  size_t i;
  int rc = 0;
  for (i = 0; i < sizeof RCM_BASE64_TEST_RFC3339_CASES; i++) {
    const char *str = rcm_base64_test_rfc3339_strings[i];
    if ((rc =
             rcm_base64_encode(out, (const unsigned char *)str, strlen(str)))) {
      return rc;
    }
    if (strcmp(rcm_base64_test_rfc3339_results[i], out)) {
      rcm_errbuf_set(err, "want: %s\nhave: %s",
                     rcm_base64_test_rfc3339_results[i], out);
      return -1;
    }
  }
  return rc;
}

TEST rcm_base64_test_encode_rfc3339(void)
{
  return rcm_mfatest_wrap(rcm_base64_test_encode_rfc3339_internal);
}

#define RCM_BASE64_TEST_WIKIPEDIA_CASES 8

static const char *rcm_base64_test_wikipedia_strings[] = {
  "sure.", "sure", "sur", "su", "leasure.", "easure.", "asure.", "sure."
};

static const char *rcm_base64_test_wikipedia_results[] = {
  "c3VyZS4=",     "c3VyZQ==",     "c3Vy",     "c3U=",
  "bGVhc3VyZS4=", "ZWFzdXJlLg==", "YXN1cmUu", "c3VyZS4="
};

static int rcm_base64_test_encode_wikipedia_internal(char *err)
{
  char out[13];
  size_t i;
  int rc = 0;
  for (i = 0; i < sizeof RCM_BASE64_TEST_WIKIPEDIA_CASES; i++) {
    const char *str = rcm_base64_test_wikipedia_strings[i];
    if ((rc =
             rcm_base64_encode(out, (const unsigned char *)str, strlen(str)))) {
      return rc;
    }
    if (strcmp(rcm_base64_test_wikipedia_results[i], out)) {
      rcm_errbuf_set(err, "want: %s\nhave: %s",
                     rcm_base64_test_wikipedia_results[i], out);
      return -1;
    }
  }
  return rc;
}

TEST rcm_base64_test_encode_wikipedia(void)
{
  return rcm_mfatest_wrap(rcm_base64_test_encode_wikipedia_internal);
}

TEST rcm_base64_test_decode_len(void)
{
  ASSERT_EQ(0, rcm_base64_decode_len("", strlen("")));
  ASSERT_EQ(1, rcm_base64_decode_len("Zg==", strlen("Zg==")));
  ASSERT_EQ(2, rcm_base64_decode_len("Zm8=", strlen("Zm8=")));
  ASSERT_EQ(3, rcm_base64_decode_len("Zm9v", strlen("Zm9v")));
  ASSERT_EQ(4, rcm_base64_decode_len("Zm9vYg==", strlen("Zm9vYg==")));
  ASSERT_EQ(5, rcm_base64_decode_len("Zm9vYmE=", strlen("Zm9vYmE=")));
  ASSERT_EQ(6, rcm_base64_decode_len("Zm9vYmFy", strlen("Zm9vYmFy")));

  /* illegal encoding */
  ASSERT_EQ(3, rcm_base64_decode_len("xxx", strlen("xxx")));
  PASS();
}

static int rcm_base64_test_decode_rfc3339_internal(char *err)
{
  unsigned char out[5];
  size_t i;
  int rc = 0;
  for (i = 0; i < sizeof RCM_BASE64_TEST_RFC3339_CASES; i++) {
    const char *res = rcm_base64_test_rfc3339_results[i];
    if ((rc = rcm_base64_decode(out, res, strlen(res)))) {
      return rc;
    }
    if (strncmp(rcm_base64_test_rfc3339_strings[i], (const char *)out,
                rcm_base64_decode_len(res, strlen(res)))) {
      rcm_errbuf_set(err, "want: %s\nhave: %s",
                     rcm_base64_test_rfc3339_strings[i], out);
      return -1;
    }
  }
  return rc;
}

TEST rcm_base64_test_decode_rfc3339(void)
{
  return rcm_mfatest_wrap(rcm_base64_test_decode_rfc3339_internal);
}

static int rcm_base64_test_decode_wikipedia_internal(char *err)
{
  unsigned char out[9];
  size_t i;
  int rc = 0;
  for (i = 0; i < sizeof RCM_BASE64_TEST_WIKIPEDIA_CASES; i++) {
    const char *res = rcm_base64_test_wikipedia_results[i];
    if ((rc = rcm_base64_decode(out, res, strlen(res)))) {
      return rc;
    }
    if (strncmp(rcm_base64_test_wikipedia_strings[i], (const char *)out,
                rcm_base64_decode_len(res, strlen(res)))) {
      rcm_errbuf_set(err, "want: %s\nhave: %s",
                     rcm_base64_test_wikipedia_strings[i], out);
      return -1;
    }
  }
  return rc;
}

TEST rcm_base64_test_decode_wikipedia(void)
{
  return rcm_mfatest_wrap(rcm_base64_test_decode_wikipedia_internal);
}

TEST rcm_base64_test_decode_illegal(void)
{
  unsigned char out[1];
  ASSERT_EQ(RCM_BASE64_ERR_ILLEGAL_LENGTH,
            rcm_base64_decode(out, "xxx", strlen("xxx")));
  ASSERT_EQ(RCM_BASE64_ERR_ILLEGAL_CHAR,
            rcm_base64_decode(out, "?xxx", strlen("?xxx")));
  ASSERT_EQ(RCM_BASE64_ERR_ILLEGAL_CHAR,
            rcm_base64_decode(out, "xx=x", strlen("xx=x")));
  ASSERT_EQ(RCM_BASE64_ERR_ILLEGAL_CHAR,
            rcm_base64_decode(out, "x===", strlen("x===")));
  PASS();
}

TEST rcm_base64_test_errstr(void)
{
  ASSERT_STR_EQ("", rcm_base64_errstr(RCM_BASE64_OK));
  ASSERT_STR_EQ("rcm_base64: undefined errnum", rcm_base64_errstr(-1));
  ASSERT_STR_EQ("rcm_base64: assertion failed",
                rcm_base64_errstr(RCM_BASE64_ERR_FAILED_ASSERT));
  ASSERT_STR_EQ("rcm_base64: illegal length (must be multiple of 4)",
                rcm_base64_errstr(RCM_BASE64_ERR_ILLEGAL_LENGTH));
  ASSERT_STR_EQ("rcm_base64: illegal character",
                rcm_base64_errstr(RCM_BASE64_ERR_ILLEGAL_CHAR));
  PASS();
}

SUITE(rcm_base64_suite)
{
  RUN_TEST(rcm_base64_test_encode_len);
  RUN_TEST(rcm_base64_test_encode_rfc3339);
  RUN_TEST(rcm_base64_test_encode_wikipedia);
  RUN_TEST(rcm_base64_test_decode_len);
  RUN_TEST(rcm_base64_test_decode_rfc3339);
  RUN_TEST(rcm_base64_test_decode_wikipedia);
  RUN_TEST(rcm_base64_test_decode_illegal);
  RUN_TEST(rcm_base64_test_errstr);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_base64_suite);
  GREATEST_MAIN_END();
}
