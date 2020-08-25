/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_assert.c"
#include "rcm_base64.c"
#include "rcm_base64buf.c"
#include "rcm_errbuf.c"
#include "rcm_mem.c"

TEST rcm_base64buf_test_encode_rfc3339(void)
{
  char err[RCM_ERRBUF_SIZE];
  char *out;
  ASSERT(!rcm_base64buf_encode(&out, (unsigned char *)"", strlen(""), err));
  ASSERT_STR_EQ("", out);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_encode(&out, (unsigned char *)"f", strlen("f"), err));
  ASSERT_STR_EQ("Zg==", out);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_encode(&out, (unsigned char *)"fo", strlen("fo"), err));
  ASSERT_STR_EQ("Zm8=", out);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_encode(&out, (unsigned char *)"foo", strlen("foo"), err));
  ASSERT_STR_EQ("Zm9v", out);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_encode(&out, (unsigned char *)"foob", strlen("foob"), err));
  ASSERT_STR_EQ("Zm9vYg==", out);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_encode(&out, (unsigned char *)"fooba", strlen("fooba"), err));
  ASSERT_STR_EQ("Zm9vYmE=", out);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_encode(&out, (unsigned char *)"foobar", strlen("foobar"), err));
  ASSERT_STR_EQ("Zm9vYmFy", out);
  rcm_mem_free(out);
  PASS();
}

static const char *rcm_base64buf_test_strings[] = {
  "sure.", "sure", "sur", "su", "leasure.", "easure.", "asure.", "sure."
};

static const char *rcm_base64buf_test_results[] = {
  "c3VyZS4=",     "c3VyZQ==",     "c3Vy",     "c3U=",
  "bGVhc3VyZS4=", "ZWFzdXJlLg==", "YXN1cmUu", "c3VyZS4="
};

TEST rcm_base64buf_test_encode_wikipedia(void)
{
  char err[RCM_ERRBUF_SIZE];
  char *out;
  size_t i;
  for (i = 0; i < sizeof *rcm_base64buf_test_strings; i++) {
    const char *str = rcm_base64buf_test_strings[i];
    ASSERT(!rcm_base64buf_encode(&out, (const unsigned char *)str, strlen(str), err));
    ASSERT_STR_EQ(rcm_base64buf_test_results[i], out);
    rcm_mem_free(out);
  }
  PASS();
}


TEST rcm_base64buf_test_decode_rfc3339(void)
{
  char err[RCM_ERRBUF_SIZE];
  unsigned char *out;
  size_t outlen;
  ASSERT(!rcm_base64buf_decode(&out, &outlen, "", strlen(""), err));
  ASSERT_STRN_EQ("", out, 0);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_decode(&out, &outlen, "Zg==", strlen("Zg=="), err));
  ASSERT_STRN_EQ("f", out, 1);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_decode(&out, &outlen, "Zm8=", strlen("Zm8="), err));
  ASSERT_STRN_EQ("fo", out, 2);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_decode(&out, &outlen, "Zm9v", strlen("Zm9v"), err));
  ASSERT_STRN_EQ("foo", out, 3);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_decode(&out, &outlen, "Zm9vYg==", strlen("Zm9vYg=="), err));
  ASSERT_STRN_EQ("foob", out, 4);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_decode(&out, &outlen, "Zm9vYmE=", strlen("Zm9vYmE="), err));
  ASSERT_STRN_EQ("fooba", out, 5);
  rcm_mem_free(out);
  ASSERT(!rcm_base64buf_decode(&out, &outlen, "Zm9vYmFy", strlen("Zm9vYmFy"), err));
  ASSERT_STRN_EQ("foobar", out, 6);
  rcm_mem_free(out);
  PASS();
}

TEST rcm_base64buf_test_decode_wikipedia(void)
{
  unsigned char *out;
  size_t i;
  for (i = 0; i < sizeof *rcm_base64buf_test_results; i++) {
    const char *res = rcm_base64buf_test_results[i];
    ASSERT(!rcm_base64buf_decode(&out, NULL, res, strlen(res), NULL));
    ASSERT_STRN_EQ(rcm_base64buf_test_strings[i], out,
                   rcm_base64_decode_len(res, strlen(res)));
    rcm_mem_free(out);
  }
  PASS();
}

SUITE(rcm_base64buf_suite)
{
	RUN_TEST(rcm_base64buf_test_encode_rfc3339);
	RUN_TEST(rcm_base64buf_test_encode_wikipedia);
	RUN_TEST(rcm_base64buf_test_decode_rfc3339);
	RUN_TEST(rcm_base64buf_test_decode_wikipedia);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_base64buf_suite);
  GREATEST_MAIN_END();
}
