#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <gl/gl.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#define ASSERTS_ACTIVE 1
#include "../src/gs_assert.h"
#include "../src/memory/gs_memory.h"

#include "../src/gs_stretchy_buffer.h"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){});
  gs_allocator A = CreatePlatformAllocator();
  
  TestGroup("Stretchy Buffer")
  {
    u8* Arr = SBufCreate(u8, 2, A);
    GlobalTest(SBufLen(Arr) == 0);
    GlobalTest(SBufCap(Arr) == 2);
    
    u8 Val0 = 25;
    SBufPrepend(Arr, Val0);
    GlobalTest(SBufLen(Arr) == 1);
    GlobalTest(SBufCap(Arr) == 2);
    GlobalTest(Arr[0] == Val0);
    
    u8 Val1 = 32;
    SBufAppend(Arr, Val1);
    GlobalTest(SBufLen(Arr) == 2);
    GlobalTest(SBufCap(Arr) == 2);
    GlobalTest(Arr[0] == Val0);
    GlobalTest(Arr[1] == Val1);
    
    u8 Val2 = 15;
    Arr[0] = Val2;
    GlobalTest(SBufLen(Arr) == 2);
    GlobalTest(SBufCap(Arr) == 2);
    GlobalTest(Arr[0] == Val2);
    GlobalTest(Arr[1] == Val1);
    
    u8 Val3 = 17;
    SBufPrepend(Arr, Val3);
    GlobalTest(SBufLen(Arr) == 3);
    GlobalTest(SBufCap(Arr) == 4);
    GlobalTest(Arr[0] == Val3);
    GlobalTest(Arr[1] == Val2);
    GlobalTest(Arr[2] == Val1);
  }
}