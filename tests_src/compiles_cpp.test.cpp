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

#define GS_GFX_PROFILE_SCOPE PROFILE_SCOPE_G
#include "../src/gs_profiler.h"

#define GS_MEMORY_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_FILE_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_THREAD_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_MATH_PROFILE_FUNC PROFILE_SCOPE_G
#include "../src/memory/gs_memory.h"
#include "../src/file/gs_file.h"
#include "../src/thread/gs_thread.h"
#include "../src/time/gs_time.h"
#include "../src/gs_debug_break.h"
#include "../src/gs_math.h"
#include "../src/gs_printf.h"

#define GFX_BACKEND_GL33
#include "../src/gfx/gs_gfx.h"

#include "../src/gs_profiler.c"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  TestGroup((char*)"Compiles for CPP")
  {
    GlobalTest(true);
  }
  return 0;
}