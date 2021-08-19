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

#include "../src/gs_tokenizer.h"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  gs_allocator Al = CreatePlatformAllocator();
  gs_memory_arena Scratch = MemoryArenaCreate(128, 4, Al, 0, 0, "Test");
  
  TestGroup("Tokenize")
  {
    char* TestStr = "Hello World 1234 -+= 53.21f\nWhats up\nLine 2";
    u64 TestStrLen = CStringLength(TestStr);
    gs_tokenizer Tok = TokenizerCreate(TestStr, TestStrLen, &Scratch);
    
    TokenizerTokenizeAll(&Tok);
    
    GlobalTest(Tok.TokensRoot != 0);
#if 0
    for (;
         At != 0;
         At = At->Next)
    {
      printf("%lld:%lld - %s: %.*s\n", 
             At->T.Row,
             At->T.Col,
             CharTypeStrings[At->T.Type],
             (u32)(At->T.Len), At->T.Data);
    }
#endif
  }
}