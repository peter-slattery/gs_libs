#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#include "../src/gs_hash.h"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  TestGroup("SHA1")
  {
    // testing patterns
#define TESTA   "abc"
#define TESTB_1 "abcdbcdecdefdefgefghfghighij"
#define TESTB_2 "hijkijkljklmklmnlmnomnopnopq"
#define TESTB   TESTB_1 TESTB_2
#define TESTC   "a"
    
    gs_sha1_ctx SHA;
    
    SHA1Reset(&SHA);
    SHA1Input(&SHA, (u8*)TESTA, CStringLength(TESTA));
    GlobalTest(SHA1Result(&SHA) != 0);
    GlobalTest(SHA.MsgDigest[0] == 0xA9993E36);
    GlobalTest(SHA.MsgDigest[1] == 0x4706816A);
    GlobalTest(SHA.MsgDigest[2] == 0xBA3E2571);
    GlobalTest(SHA.MsgDigest[3] == 0x7850C26C);
    GlobalTest(SHA.MsgDigest[4] == 0x9CD0D89D);
    
    SHA1Reset(&SHA);
    SHA1Input(&SHA, (u8*)TESTB, CStringLength(TESTB));
    GlobalTest(SHA1Result(&SHA) != 0);
    GlobalTest(SHA.MsgDigest[0] == 0x84983E44);
    GlobalTest(SHA.MsgDigest[1] == 0x1C3BD26E);
    GlobalTest(SHA.MsgDigest[2] == 0xBAAE4AA1);
    GlobalTest(SHA.MsgDigest[3] == 0xF95129E5);
    GlobalTest(SHA.MsgDigest[4] == 0xE54670F1);
    
    // 1 million 'a' characters
    SHA1Reset(&SHA);
    for (u64 i = 0; i < 1000000; i++)
    {
      SHA1Input(&SHA, (u8*)TESTC, 1);
    }
    GlobalTest(SHA1Result(&SHA) != 0);
    GlobalTest(SHA.MsgDigest[0] == 0x34AA973C);
    GlobalTest(SHA.MsgDigest[1] == 0xD4C4DAA4);
    GlobalTest(SHA.MsgDigest[2] == 0xF61EEB2B);
    GlobalTest(SHA.MsgDigest[3] == 0xDBAD2731);
    GlobalTest(SHA.MsgDigest[4] == 0x6534016F);
    
  }
}