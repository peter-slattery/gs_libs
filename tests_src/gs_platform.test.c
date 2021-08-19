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

internal void
PrintPlatform(char* Platform, bool WasFound)
{
  printf("\t%s: %s\n", Platform, WasFound ? "true" : "false");
}

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  bool IsWindows = false; 
  bool IsOsx = false; 
  bool IsLinux = false; 
  bool IsAndroid = false; 
  bool IsiOS = false; 
  bool IsXbox = false; 
  bool IsPS = false; 
  bool IsSwitch = false;
  
  bool IsMSVC = false;
  bool IsClang = false;
  ;
  TestGroup("Has Platform")
  {
    bool Result = false;
    s32 PlatformCount = 0;
    
#if PLATFORM_WINDOWS 
    Result = true;
    PlatformCount += 1;
    IsWindows = true;
#endif
    
#if PLATFORM_OSX 
    Result = true;
    PlatformCount += 1;
    IsOsx = true;
#endif
    
#if PLATFORM_LINUX 
    Result = true;
    PlatformCount += 1;
    IsLinux = true;
#endif
    
#if PLATFORM_ANDROID 
    Result = true;
    PlatformCount += 1;
    IsAndroid = true;
#endif
    
#if PLATFORM_IOS 
    Result = true;
    PlatformCount += 1;
    IsiOS = true;
#endif
    
#if PLATFORM_XBOX 
    Result = true;
    PlatformCount += 1;
    IsXbox = true;
#endif
    
#if PLATFORM_PS 
    Result = true;
    PlatformCount += 1;
    IsPS = true;
#endif
    
#if PLAYFORM_SWITCH 
    Result = true;
    PlatformCount += 1;
    IsSwitch = true;
#endif
    
    GlobalTest(Result);
    GlobalTest(PlatformCount == 1);
  }
  
  TestGroup("Has Platform")
  {
    bool Result = false;
    
    
    s32 CompilersCount = 0;
#if COMPILER_MSVC
    Result = true;
    CompilersCount += 1;
    IsMSVC = true;
#endif
    
#if COMPILER_CLANG
    Result = true;
    CompilersCount += 1;
    IsClang = true;
#endif
    
    GlobalTest(Result);
    
    // NOTE(PS): I think this is because on windows you can compile with clang
    // but link with msvc
    GlobalTest(CompilersCount >= 1);
  }
  
  printf("Platforms Found: \n");
  PrintPlatform("Windows", IsWindows);
  PrintPlatform("Linux", IsLinux);
  PrintPlatform("OSX", IsOsx);
  PrintPlatform("Android", IsAndroid);
  PrintPlatform("iOS", IsiOS);
  PrintPlatform("Xbox", IsXbox);
  PrintPlatform("PS", IsPS);
  PrintPlatform("Switch", IsSwitch);
  
  
  printf("Compilers Found: \n");
  PrintPlatform("MSVC", IsMSVC);
  PrintPlatform("Clang", IsClang);
}