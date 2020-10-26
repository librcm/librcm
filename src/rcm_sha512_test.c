/* SPDX-License-Identifier: Unlicense OR MIT */

#include "greatest.h"

#include "rcm_hex.h"
#include "rcm_sha512.h"

#define RCM_SHA512_TEST_CASES 14

static const char *rcm_sha512_test_strings[] = {
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

static const char *rcm_sha512_test_results[] = {
  "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d"
  "85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
  "1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652b"
  "f08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75",
  "2d408a0717ec188158278a796c689044361dc6fdde28d6f04973b80896e1823975cdbf12eb"
  "63f9e0591328ee235d80e9b5bf1aa6a44f4617ff3caf6400eb172d",
  "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a27"
  "4fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f",
  "d8022f2060ad6efd297ab73dcc5355c9b214054b0d1776a136a669d26a7d3b14f73aa0d0eb"
  "ff19ee333368f0164b6419a96da49e3e481753e7e96b716bdccb6f",
  "878ae65a92e86cac011a570d4c30a7eaec442b85ce8eca0c2952b5e3cc0628c2e79d889ad4"
  "d5c7c626986d452dd86374b6ffaa7cd8b67665bef2289a5c70b0a1",
  "e32ef19623e8ed9d267f657a81944b3d07adbb768518068e88435745564e8d4150a0a703be"
  "2a7d88b61e3d390c2bb97e2d4c311fdc69d6b1267f05f59aa920e7",
  "d716a4188569b68ab1b6dfac178e570114cdf0ea3a1cc0e31486c3e41241bc6a76424e8c37"
  "ab26f096fc85ef9886c8cb634187f4fddff645fb099f1ff54c6b8c",
  "a3a8c81bc97c2560010d7389bc88aac974a104e0e2381220c6e084c4dccd1d2d17d4f86db3"
  "1c2a851dc80e6681d74733c55dcd03dd96f6062cdda12a291ae6ce",
  "f22d51d25292ca1d0f68f69aedc7897019308cc9db46efb75a03dd494fc7f126c010e8ade6"
  "a00a0c1a5f1b75d81e0ed5a93ce98dc9b833db7839247b1d9c24fe",
  "ef6b97321f34b1fea2169a7db9e1960b471aa13302a988087357c520be957ca119c3ba68e6"
  "b4982c019ec89de3865ccf6a3cda1fe11e59f98d99f1502c8b9745",
  "2210d99af9c8bdecda1b4beff822136753d8342505ddce37f1314e2cdbb488c6016bdaa9bd"
  "2ffa513dd5de2e4b50f031393d8ab61f773b0e0130d7381e0f8a1d",
  "a687a8985b4d8d0a24f115fe272255c6afaf3909225838546159c1ed685c211a203796ae8e"
  "cc4c81a5b6315919b3a64f10713da07e341fcdbb08541bf03066ce",
  "8ddb0392e818b7d585ab22769a50df660d9f6d559cca3afc5691b8ca91b8451374e42bcdab"
  "d64589ed7c91d85f626596228a5c8572677eb98bc6b624befb7af8",
};

TEST rcm_sha512_test(void)
{
  unsigned char out[RCM_SHA512_SIZE], res[RCM_SHA512_SIZE];
  int i;
  for (i = 0; i < RCM_SHA512_TEST_CASES; i++) {
    ASSERT(!rcm_hex_parse_str(res, rcm_sha512_test_results[i],
                              strlen(rcm_sha512_test_results[i])));
    ASSERT(!rcm_sha512(out, (const unsigned char *)rcm_sha512_test_strings[i],
                       strlen(rcm_sha512_test_strings[i])));
    ASSERT_MEM_EQ(res, out, RCM_SHA512_SIZE);
  }
  ASSERT_EQ(RCM_SHA512_ERR_FAILED_ASSERT, rcm_sha512(NULL, NULL, 0));
  PASS();
}

SUITE(rcm_sha512_suite)
{
  RUN_TEST(rcm_sha512_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(rcm_sha512_suite);
  GREATEST_MAIN_END();
}
