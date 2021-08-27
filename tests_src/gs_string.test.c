#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#define ASSERTS_ACTIVE 1
#include "../src/gs_assert.h"
#include "../src/memory/gs_memory.h"
#include "../src/gs_string.h"
#include "../src/gs_hash.h"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  gs_allocator Al = CreatePlatformAllocator();
  gs_memory_arena Scratch = MemoryArenaCreate(128, 4, Al, 0, 0, "Test");
  
  TestGroup("C String Length")
  {
    char* CStr = "123456789012";
    GlobalTest(CStringLength(CStr) == 12);
  }
  
  TestGroup("StringCreate & LitStr")
  {
    gs_string S0 = LitStr("Testing");
    GlobalTest(S0.Len == S0.Cap);
    GlobalTest(S0.Len == 7);
    
    char CharArray[] = "Testing";
    u64 Len = sizeof(CharArray) / sizeof(CharArray[0]);
    gs_string S1 = StringCreateConst((u8*)CharArray, Len);
    GlobalTest(S1.Len == S1.Cap);
    GlobalTest(S1.Len == Len);
  }
  
  TestGroup("Index Of Char")
  {
    gs_string S0 = LitStr("Test2ing 1234");
    
    GlobalTest(StringIndexOfCharFwd(S0, 's') == 2);
    GlobalTest(StringIndexOfCharBwd(S0, 's') == 2);
    GlobalTest(StringIndexOfCharFwd(S0, '2') == 4);
    GlobalTest(StringIndexOfCharBwd(S0, '2') == 10);
    GlobalTest(StringIndexOfCharBwd(S0, '5') == -1);
  }
  
  TestGroup("Index Of String")
  {
    gs_string S0 = LitStr("Test2ing 12i34");
    gs_string S1 = LitStr("t2ing ");
    gs_string S2 = LitStr("2i");
    gs_string S3 = LitStr("t2inz ");
    gs_string S4 = LitStr("foo");
    
    GlobalTest(StringIndexOfStringFwd(S0, S1) == 3);
    GlobalTest(StringIndexOfStringBwd(S0, S1) == 3);
    GlobalTest(StringIndexOfStringFwd(S0, S2) == 4);
    GlobalTest(StringIndexOfStringBwd(S0, S2) == 10);
    GlobalTest(StringIndexOfStringBwd(S0, S3) == -1);
    GlobalTest(StringIndexOfStringBwd(S0, S4) == -1);
    
  }
  
  TestGroup("Contains")
  {
    gs_string S0 = LitStr("Testing 1234");
    gs_string S1 = LitStr("ting ");
    gs_string S2 = LitStr("tinz ");
    gs_string S3 = LitStr("foo");
    
    GlobalTest( StringContainsChar(S0, '1'));
    GlobalTest(!StringContainsChar(S0, 'z'));
    
    GlobalTest( StringContainsString(S0, S1));
    GlobalTest(!StringContainsString(S0, S2));
    GlobalTest(!StringContainsString(S0, S3));
  }
  
  TestGroup("Substring")
  {
    gs_string S0 = LitStr("Testing 1234");
    gs_string S1 = LitStr("ting ");
    
    gs_string S0Sub0 = StringSubstring(S0, 3, 8);
    gs_string S0Sub1 = StringSubstring(S0, 3, -4);
    
    GlobalTest(StringEqualsString(S0Sub0, S1));
    GlobalTest(StringEqualsString(S0Sub1, S1));
  }
  
  TestGroup("Split")
  {
    gs_string S0 = LitStr("Testing 1234");
    gs_string A = {
      .Data = PushArray(&Scratch, u8, 32),
      .Cap = 32,
      .Len = 0
    };
    gs_string B = {
      .Data = PushArray(&Scratch, u8, 32),
      .Cap = 32,
      .Len = 0
    };
    StringSplit(S0, 3, &A, &B);
    
    gs_string AValue = LitStr("Tes");
    gs_string BValue = LitStr("ting 1234");
    
    GlobalTest(StringEqualsString(A, AValue));
    GlobalTest(StringEqualsString(B, BValue));
  }
  
  TestGroup("Reverse")
  {
    gs_string S0 = {
      .Data = PushArray(&Scratch, u8, 32),
      .Cap = 32,
      .Len = 0
    };
    StringCopyCharArray("Testing 1234", &S0);
    gs_string SR = LitStr("4321 gnitseT");
    StringReverse(S0);
    
    GlobalTest(StringEqualsString(S0, SR));
  }
  
  TestGroup("Copy")
  {
    gs_string S0 = LitStr("Testing 1234");
    gs_string S1 = {
      .Data = PushArray(&Scratch, u8, S0.Cap),
      .Cap = S0.Cap,
      .Len = 0,
    };
    StringCopy(S0, &S1);
    
    GlobalTest(StringEqualsString(S0, S1));
  }
  
  MemoryArenaClear(&Scratch);
  TestGroup("Encoding - Base64")
  {
    // strings to encode
    gs_string TInStr[] = {
      LitStr("Foobar\nTest"),
      LitStr("heytherebuddyhow'sitgoing?"),
      LitStr("heytherebuddyhow'sitgoing?Thisqerqweuro82491845h23@#$%@#$%23452345\n523452345123412342\n412341234\ns\ndfs\nf\nsdf"),
    };
    
    // parallel array to TInStr,
    // containing the encoded versions of the string at the same index
    gs_string TOutStr[] = {
      LitStr("Rm9vYmFyClRlc3Q="),
      LitStr("aGV5dGhlcmVidWRkeWhvdydzaXRnb2luZz8="),
      LitStr("aGV5dGhlcmVidWRkeWhvdydzaXRnb2luZz9UaGlzcWVycXdldXJvODI0OTE4NDVoMjNAIyQl\nQCMkJTIzNDUyMzQ1CjUyMzQ1MjM0NTEyMzQxMjM0Mgo0MTIzNDEyMzQKcwpkZnMKZgpzZGY="),
    };
    
    u64 TCount = sizeof(TOutStr)/sizeof(TOutStr[0]);
    
    for (u64 i = 0; i < TCount; i++)
    {
      gs_string In = TInStr[i];
      gs_string Out = TOutStr[i];
      
      gs_string EncRes = {};
      EncRes.Data = Encode_Base64Alloc(In.Data, In.Len, 
                                       &EncRes.Len, &Scratch);
      GlobalTest(EncRes.Len == Out.Len);
      GlobalTest(StringEqualsString(EncRes, Out));
      
      gs_string DecRes = {};
      DecRes.Data = Decode_Base64Alloc(Out.Data, Out.Len, 
                                       &DecRes.Len, &Scratch);
      GlobalTest(DecRes.Len == In.Len);
      GlobalTest(StringEqualsString(DecRes, In));
      
      gs_string ReEncRes = {};
      ReEncRes.Data = Encode_Base64Alloc(DecRes.Data, DecRes.Len,
                                         &ReEncRes.Len, &Scratch);
      GlobalTest(Out.Len == ReEncRes.Len);
      GlobalTest(StringEqualsString(ReEncRes, Out));
      
      gs_string ReDecRes = {};
      ReDecRes.Data = Decode_Base64Alloc(EncRes.Data, EncRes.Len,
                                         &ReDecRes.Len, &Scratch);
      GlobalTest(In.Len == ReDecRes.Len);
      GlobalTest(StringEqualsString(ReDecRes, In));
    }
  }
  
  MemoryArenaClear(&Scratch);
  TestGroup("SHA1 + Base64 Encoding")
  {
    // From: https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/Writing_WebSocket_servers
    
    gs_string In = LitStr("dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
    u32 SHAHashRes[5] = {
      0xb37a4f2c,
      0xc0624f16,
      0x90f64606,
      0xcf385945,
      0xb2bec4ea,
    };
    gs_string Out = LitStr("s3pPLMBiTxaQ9kYGzzhZRbK+xOo=");
    
    gs_sha1_ctx SHA;
    SHA1Reset(&SHA);
    SHA1Input(&SHA, In.Data, In.Len);
    GlobalTest(SHA1Result(&SHA) != 0);
    GlobalTest(SHA.MsgDigest[0] == SHAHashRes[0]);
    GlobalTest(SHA.MsgDigest[1] == SHAHashRes[1]);
    GlobalTest(SHA.MsgDigest[2] == SHAHashRes[2]);
    GlobalTest(SHA.MsgDigest[3] == SHAHashRes[3]);
    GlobalTest(SHA.MsgDigest[4] == SHAHashRes[4]);
    
    u32 SHAHashResEndianSwapped[5] = {
      EndianSwapU32(SHA.MsgDigest[0]),
      EndianSwapU32(SHA.MsgDigest[1]),
      EndianSwapU32(SHA.MsgDigest[2]),
      EndianSwapU32(SHA.MsgDigest[3]),
      EndianSwapU32(SHA.MsgDigest[4]),
    };
    
    gs_string Res = {};
    Res.Data = Encode_Base64Alloc((u8*)SHAHashResEndianSwapped, 20, &Res.Len, &Scratch);
    
    GlobalTest(StringEqualsString(Res, Out));
  }
}