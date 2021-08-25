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
}