#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"

enum 
{
  Flag0 = 0x1,
  Flag1 = 0x2,
  Flag2 = 0x4,
  Flag3 = 0x8
} test_flags;

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  TestGroup("Flags")
  {
    u8 Flags = 0;
    GlobalTest(!HasFlag(Flags, Flag1));
    
    AddFlag(Flags, Flag1);
    GlobalTest(HasFlag(Flags, Flag1));
    GlobalTest(HasFlagOnly(Flags, Flag1));
    GlobalTest(!HasFlag(Flags, Flag0));
    GlobalTest(!HasFlag(Flags, Flag2));
    
    AddFlag(Flags, Flag2);
    GlobalTest(HasFlag(Flags, Flag1));
    GlobalTest(HasFlag(Flags, Flag2));
    GlobalTest(!HasFlagOnly(Flags, Flag1));
    GlobalTest(!HasFlag(Flags, Flag0));
  }
  
  TestGroup("Math")
  {
    GlobalTest(Min(5, 16) == 5);
    GlobalTest(Max(5, 16) == 16);
    GlobalTest(Min(16, 5) == 5);
    GlobalTest(Max(16, 5) == 16);
    
    GlobalTest(Clamp(0, 1, 2) == 1);
    GlobalTest(Clamp(0, -1, 2) == 0);
    GlobalTest(Clamp(0, 3, 2) == 2);
    
    GlobalTest(Abs(-1) == 1);
    GlobalTest(Abs(1) == 1);
    
    GlobalTest(Sign(1) == 1);
    GlobalTest(Sign(-1) == -1);
    
    GlobalTest(IsPowerOfTwo(256));
    GlobalTest(!IsPowerOfTwo(255));
    
    GlobalTest(IsOdd(3));
    GlobalTest(!IsOdd(4));
  }
  
  TestGroup("C Strings")
  {
    char* S = "0123456789";
    GlobalTest(CStringLength(S) == 10);
    
    char* StringA = "FooBarBaz";
    char* StringB = "FooBarBaz";
    char* StringC = "Short";
    
    GlobalTest(CStringsEqual(StringA, StringB));
    GlobalTest(!CStringsEqual(StringA, StringC));
    GlobalTest(!CStringsEqual(StringC, StringA));
  }
  
  TestGroup("Endian Swaps")
  {
    u16 U16In  = 0xABCD;
    u16 U16Out = 0xCDAB;
    u16 U16Res = EndianSwapU16(U16In);
    GlobalTest(U16Out == U16Res);
    
    u32 U32In  = 0x89ABCDEF;
    u32 U32Out = 0xEFCDAB89;
    u32 U32Res = EndianSwapU32(U32In);
    GlobalTest(U32Out == U32Res);
    
    u64 U64In  = 0xF123456789ABCDEF;
    u64 U64Out = 0xEFCDAB89674523F1;
    u64 U64Res = EndianSwapU64(U64In);
    GlobalTest(U64Out == U64Res);
  }
}