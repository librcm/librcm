/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_hex.h"
#include "rcm_sha256.h"

#define RCM_SHA256_TEST_CASES 14

static const char *rcm_sha256_test_strings[] = {
  "",
  "a",
  "ab",
  "abc",
  "abcd",
  "abcde",
  "abcdef",
  "abcdefg",
  "abcdefgh",
  "abcdefghi",
  "abcdefghij",
  "Discard medicine more than two years old.",
  "He who has a shady past knows that nice guys finish last.",
  "I wouldn't marry him with a ten foot pole."
};

static const char *rcm_sha256_test_results[] = {
  "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
  "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb",
  "fb8e20fc2e4c3f248c60c39bd652f3c1347298bb977b8b4d5903b85055620603",
  "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
  "88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589",
  "36bbe50ed96841d10443bcb670d6554f0a34b761be67ec9c4a8ad2c0c44ca42c",
  "bef57ec7f53a6d40beb640a780a639c83bc29ac8a9816f1fc6c5c6dcd93c4721",
  "7d1a54127b222502f5b79b5fb0803061152a44f92b37e23c6527baf665d4da9a",
  "9c56cc51b374c3ba189210d5b6d4bf57790d351c96c47c02190ecf1e430635ab",
  "19cc02f26df43cc571bc9ed7b0c4d29224a3ec229529221725ef76d021c8326f",
  "72399361da6a7754fec986dca5b7cbaf1c810a28ded4abaf56b2106d06cb78b0",
  "a144061c271f152da4d151034508fed1c138b8c976339de229c3bb6d4bbb4fce",
  "6dae5caa713a10ad04b46028bf6dad68837c581616a1589a265a11288d4bb5c4",
  "ae7a702a9509039ddbf29f0765e70d0001177914b86459284dab8b348c2dce3f",
};

TEST rcm_sha256_test(void)
{
  unsigned char out[RCM_SHA256_SIZE], res[RCM_SHA256_SIZE];
  int i;
  for (i = 0; i < RCM_SHA256_TEST_CASES; i++) {
    ASSERT(!rcm_hex_parse_str(res, rcm_sha256_test_results[i],
                              strlen(rcm_sha256_test_results[i])));
    rcm_sha256(out, (const unsigned char *)rcm_sha256_test_strings[i],
               strlen(rcm_sha256_test_strings[i]));
    ASSERT_MEM_EQ(res, out, RCM_SHA256_SIZE);
  }
  PASS();
}

SUITE(rcm_sha256_suite)
{
  RUN_TEST(rcm_sha256_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_sha256_suite);
  GREATEST_MAIN_END();
}
