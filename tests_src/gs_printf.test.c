#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#define ASSERTS_ACTIVE 1
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#include "../src/gs_assert.h"
#include "../src/memory/gs_memory.h"
#include "../src/gs_printf.h"

#ifdef GS_PLATFORM_WINDOWS
# include <windows.h>
#endif

internal bool
TestStringsEqual(char* A, char* B, u64 Len)
{
  for (u64 i = 0; i < Len; i++)
  {
    if (A[i] != B[i]) return false;
  }
  return true;
}

typedef struct test_compound_struct
{
  u32 A;
  r32 B;
  bool C;
} test_compound_struct;

PRINTF_TYPE_PRINT(PrintF_TestStruct)
{
  test_compound_struct S = *((test_compound_struct*)Value);
  SPrintFNested(PFState, "tcs{{ .A={u32}, .B={r32}, .C={bool} }}", 
                &S.A, &S.B, &S.C);
}

PRINTF_TYPE_PRINT(PrintF_TestStructAlt)
{
  test_compound_struct S = *((test_compound_struct*)Value);
  if (S.C)
  {
    SPrintFNested(PFState, "I'm a real boy!");
  }
  else
  {
    SPrintFNested(PFState, "I'm still a puppet.");
  }
}

typedef struct test_nested_struct
{
  u64 A;
  test_compound_struct B;
  s32 C;
} test_nested_struct;

PRINTF_TYPE_PRINT(PrintF_TestNestedStruct)
{
  test_nested_struct S = *((test_nested_struct*)Value);
  SPrintFNested(PFState, "tns{{ .A={u64}, .B={test_compound_struct}, .C={s32} }}", 
                &S.A, &S.B, &S.C);
}

int main(int ArgCoutn, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  gs_allocator A = CreatePlatformAllocator();
  PrintFInitNoBasic(A);
  
  TestGroup("Register Collisions")
  {
    char* TypeName = "bool";
    u64 TypeNameHash = PrintFHash(TypeName, CStringLength(TypeName));
    
    s64 HashIndex = PrintFTypeHashLookup(0, TypeNameHash);
    GlobalTest(HashIndex == -1);
    
    PrintFRegisterType(TypeName, PrintF_Bool);
    
    HashIndex = PrintFTypeHashLookup(0, TypeNameHash);
    GlobalTest(HashIndex >= 0);
    
    PrintFReset();
  }
  
  TestGroup("Basic Strings")
  {
    char* FormatStr = "Testing, 1234, hello, \t what's up?\n";
    u64 FormatStrLen = CStringLength(FormatStr);
    
    // Big Enough Buffer
    u64 DestStrLen = 1024;
    char* DestStr = AllocArray(A, char, DestStrLen, "root");
    
    // LengthPrinted should equal FormatStrLen only in this case because
    // we don't have any elements in the format string that
    // expand or contract - ie. SPrintF is basically behaving as
    // a MemCopy in this case - which is kind of what we're testing
    u64 LengthPrinted = SPrintF(DestStr, DestStrLen, FormatStr);
    GlobalTest(LengthPrinted == FormatStrLen);
    GlobalTest(TestStringsEqual(DestStr, FormatStr, FormatStrLen));
    
    // Too Small Buffer
    DestStrLen = 10;
    DestStr = AllocArray(A, char, DestStrLen, "root");
    
    // NOTE(PS): LengthPrinted should be the length needed to 
    // print the whole format string, not the length actually
    // printed out 
    LengthPrinted = SPrintF(DestStr, DestStrLen, FormatStr);
    GlobalTest(LengthPrinted == FormatStrLen);
    GlobalTest(TestStringsEqual(DestStr, FormatStr, DestStrLen));
  }
  
  PrintFRegisterBasics();
  
  TestGroup("Basic Types")
  {
    u8 TestU8 = 123;
    u64 TestU64 = 4312;
    s8 TestS8 = -3;
    s64 TestS64 = 154312;
    bool TestBool = false;
    r32 TestR32 = 3.141592f;
    r64 TestR64 = 6.283184;
    
    char* FormatStr = "{u8} {u64} {s8} {s64} {bool} {r32} {r64} {{}}";
    char* ExpectedResult = "123 4312 -3 154312 false 3.141592 6.283184 {}";
    u64 ExpectedResultLen = CStringLength(ExpectedResult);
    
    u64 DestStrLen = 1024;
    char* DestStr = AllocArray(A, char, DestStrLen, "root");
    u64 LengthPrinted = SPrintF(DestStr, DestStrLen, FormatStr,
                                &TestU8, &TestU64, &TestS8, &TestS64, &TestBool, &TestR32, &TestR64);
    GlobalTest(LengthPrinted == ExpectedResultLen);
    GlobalTest(TestStringsEqual(DestStr, ExpectedResult, ExpectedResultLen));
    
  }
  
  TestGroup("Compound Types")
  {
    PrintFRegisterType("test_compound_struct", PrintF_TestStruct);
    
    test_compound_struct StructValue = {
      .A = 1234,
      .B = 3.1415f,
      .C = true
    };
    
    // NOTE(PS): Even though we set .B to 3.1415, PrintF_R64 prints it
    // as 3.14149 - while this is incorrect, the actual print function
    // is working as designed (ie. its a poor float print function, not
    // a buggy one... if that distinction means anything)
    // TODO(PS): Fix PrintF_R64 to be better
    char* ExpectedResult = "tcs{ .A=1234, .B=3.141499, .C=true }";
    u64 ExpectedResultLen = CStringLength(ExpectedResult);
    
    u64 DestStrLen = 1024;
    char* DestStr = AllocArray(A, char, DestStrLen, "root");
    u64 LengthPrinted = SPrintF(DestStr, DestStrLen, "{test_compound_struct}",
                                &StructValue);
    
    GlobalTest(LengthPrinted == ExpectedResultLen);
    GlobalTest(TestStringsEqual(DestStr, ExpectedResult, ExpectedResultLen));
  }
  
  TestGroup("Nested Types")
  {
    PrintFRegisterType("test_nested_struct", PrintF_TestNestedStruct);
    
    test_nested_struct StructValue = {
      .A = 15536,
      .B = {
        .A = 1234,
        .B = 6.1415f,
        .C = false
      },
      .C = -156,
    };
    
    char* ExpectedResult = "tns{ .A=15536, .B=tcs{ .A=1234, .B=6.141499, .C=false }, .C=-156 }";
    u64 ExpectedResultLen = CStringLength(ExpectedResult);
    
    u64 DestStrLen = 1024;
    char* DestStr = AllocArray(A, char, DestStrLen, "root");
    u64 LengthPrinted = SPrintF(DestStr, DestStrLen, "{test_nested_struct}",
                                &StructValue);
    
    GlobalTest(LengthPrinted == ExpectedResultLen);
    GlobalTest(TestStringsEqual(DestStr, ExpectedResult, ExpectedResultLen));
  }
  
  TestGroup("Same Type, Many Formats")
  {
    PrintFRegisterType("foo", PrintF_TestStructAlt);
    
    test_compound_struct StructValue = { .C = true };
    
    char* ExpectedResult = "I'm a real boy!";
    u64 ExpectedResultLen = CStringLength(ExpectedResult);
    
    u64 DestStrLen = 1024;
    char* DestStr = AllocArray(A, char, DestStrLen, "root");
    u64 LengthPrinted = SPrintF(DestStr, DestStrLen, "{foo}",
                                &StructValue);
    
    GlobalTest(LengthPrinted == ExpectedResultLen);
    GlobalTest(TestStringsEqual(DestStr, ExpectedResult, ExpectedResultLen));
  }
  
  TestGroup("MPrintF")
  {
    gs_memory_arena M = MemoryArenaCreate(4096, 8, A, 0, 0, "mprintf arena");
    
    char* FormatString = "That's a big pile of {s32} leaves!";
    u64 FormatStringLen = CStringLength(FormatString);
    
    char* ExpectedString = "That's a big pile of 40312 leaves!";
    u64 ExpectedStringLen = CStringLength(ExpectedString);
    
    s32 V = 40312;
    
    char* TestString = MPrintF(&M, FormatString, &V);
    u64 TestStringLen = CStringLength(TestString);
    
    GlobalTest(TestStringLen == ExpectedStringLen);
    GlobalTest(TestStringsEqual(TestString, ExpectedString, ExpectedStringLen));
  }
  
  return 0;
}

