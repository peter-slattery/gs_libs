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
#include "../src/file/gs_file.h"
#include "../src/thread/gs_thread.h"
#include "../src/time/gs_time.h"
#include "../src/gs_debug_break.h"
#include "../src/gs_math.h"
#include "../src/gs_printf.h"

#define GFX_BACKEND_GL33
#include "../src/gfx/gs_gfx.h"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  TestGroup("Input Button States")
  {
    gs_input_btn_state TestState = {0};
    
    // An default button state
    GlobalTest(InputButtonIsUp(&TestState, 0));
    GlobalTest(!InputButtonIsDown(&TestState, 0));
    GlobalTest(!InputButtonTransitionedUp(&TestState, 0));
    GlobalTest(!InputButtonTransitionedDown(&TestState, 0));
    
    // transitioned down
    InputButtonSetDown(&TestState, 0);
    GlobalTest(!InputButtonIsUp(&TestState, 0));
    GlobalTest(InputButtonIsDown(&TestState, 0));
    GlobalTest(!InputButtonTransitionedUp(&TestState, 0));
    GlobalTest(InputButtonTransitionedDown(&TestState, 0));
    
    // held down
    InputButtonSetDown(&TestState, 0);
    GlobalTest(!InputButtonIsUp(&TestState, 0));
    GlobalTest(InputButtonIsDown(&TestState, 0));
    GlobalTest(!InputButtonTransitionedUp(&TestState, 0));
    GlobalTest(!InputButtonTransitionedDown(&TestState, 0));
    
    // held down (repeat)
    InputButtonSetDown(&TestState, 0);
    GlobalTest(!InputButtonIsUp(&TestState, 0));
    GlobalTest(InputButtonIsDown(&TestState, 0));
    GlobalTest(!InputButtonTransitionedUp(&TestState, 0));
    GlobalTest(!InputButtonTransitionedDown(&TestState, 0));
    
    // transitioned up
    InputButtonSetUp(&TestState, 0);
    GlobalTest(InputButtonIsUp(&TestState, 0));
    GlobalTest(!InputButtonIsDown(&TestState, 0));
    GlobalTest(InputButtonTransitionedUp(&TestState, 0));
    GlobalTest(!InputButtonTransitionedDown(&TestState, 0));
    
    // held up
    InputButtonSetUp(&TestState, 0);
    GlobalTest(InputButtonIsUp(&TestState, 0));
    GlobalTest(!InputButtonIsDown(&TestState, 0));
    GlobalTest(!InputButtonTransitionedUp(&TestState, 0));
    GlobalTest(!InputButtonTransitionedDown(&TestState, 0));
    
    // held up (repeat)
    InputButtonSetUp(&TestState, 0);
    GlobalTest(InputButtonIsUp(&TestState, 0));
    GlobalTest(!InputButtonIsDown(&TestState, 0));
    GlobalTest(!InputButtonTransitionedUp(&TestState, 0));
    GlobalTest(!InputButtonTransitionedDown(&TestState, 0));
  }
  return 0;
}