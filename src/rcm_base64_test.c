/* SPDX-License-Identifier: Unlicense OR MIT */

#include <string.h>

#include "greatest.h"

#include "rcm_assert.c"
#include "rcm_base64.c"

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

TEST rcm_base64_test_encode_rfc3339(void)
{
  char out[9];
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"", strlen("")));
  ASSERT_STR_EQ("", out);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"f", strlen("f")));
  ASSERT_STR_EQ("Zg==", out);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"fo", strlen("fo")));
  ASSERT_STR_EQ("Zm8=", out);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"foo", strlen("foo")));
  ASSERT_STR_EQ("Zm9v", out);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"foob", strlen("foob")));
  ASSERT_STR_EQ("Zm9vYg==", out);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"fooba", strlen("fooba")));
  ASSERT_STR_EQ("Zm9vYmE=", out);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"foobar", strlen("foobar")));
  ASSERT_STR_EQ("Zm9vYmFy", out);
  PASS();
}

static const char *rcm_base64_test_strings[] = {
  "sure.", "sure", "sur", "su", "leasure.", "easure.", "asure.", "sure."
};

static const char *rcm_base64_test_results[] = {
  "c3VyZS4=",     "c3VyZQ==",     "c3Vy",     "c3U=",
  "bGVhc3VyZS4=", "ZWFzdXJlLg==", "YXN1cmUu", "c3VyZS4="
};

TEST rcm_base64_test_encode_wikipedia(void)
{
  char out[13];
  size_t i;
  for (i = 0; i < sizeof *rcm_base64_test_strings; i++) {
    const char *str = rcm_base64_test_strings[i];
    ASSERT(!rcm_base64_encode(out, (const unsigned char *)str, strlen(str)));
    ASSERT_STR_EQ(rcm_base64_test_results[i], out);
  }
  PASS();
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
  PASS();
}

TEST rcm_base64_test_decode_rfc3339(void)
{
  char out[9];
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"", strlen("")));
  ASSERT_STRN_EQ("", out, 0);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"Zg==", strlen("Zg==")));
  ASSERT_STRN_EQ("f", out, 1);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"Zm8=", strlen("Zm8=")));
  ASSERT_STRN_EQ("fo", out, 2);
  ASSERT(!rcm_base64_encode(out, (unsigned char *)"Zm9v", strlen("Zm9v")));
  ASSERT_STRN_EQ("foo", out, 3);
  ASSERT(
      !rcm_base64_encode(out, (unsigned char *)"Zm9vYg==", strlen("Zm9vYg==")));
  ASSERT_STRN_EQ("foob", out, 4);
  ASSERT(
      !rcm_base64_encode(out, (unsigned char *)"Zm9vYmE=", strlen("Zm9vYmE=")));
  ASSERT_STRN_EQ("fooba", out, 5);
  ASSERT(
      !rcm_base64_encode(out, (unsigned char *)"Zm9vYmFy", strlen("Zm9vYmFy")));
  ASSERT_STRN_EQ("foobar", out, 6);
  PASS();
}

TEST rcm_base64_test_decode_wikipedia(void)
{
  unsigned char out[9];
  size_t i;
  for (i = 0; i < sizeof *rcm_base64_test_results; i++) {
    const char *res = rcm_base64_test_results[i];
    ASSERT(!rcm_base64_decode(out, res, strlen(res)));
    ASSERT_STR_EQ(rcm_base64_test_strings[i], out);
  }
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
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_base64_suite);
  GREATEST_MAIN_END();
}