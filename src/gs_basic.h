/* date = August 23rd 2021 11:25 pm */

#ifndef GS_BASIC_H
#define GS_BASIC_H

// NOTE(PS): a utility to include all of gs_libs in one include statement
// just a convenience file

// NOTE(PS): gs_platform is excluded from this file so you can
// include it before hand, and then use the PLATFORM_* defines
// to include platform appropriate libraries/files that gs_libs
// might rely on

#define ASSERTS_ACTIVE 1
#include "../src/gs_assert.h"

#include "gs_types.h"

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
#define GS_SOCKET_PROFILE_FUNC PROFILE_SCOPE_G
#include "../src/memory/gs_memory.h"
#include "../src/file/gs_file.h"
#include "../src/thread/gs_thread.h"
#include "../src/time/gs_time.h"

#include "../src/gs_debug_break.h"
#include "../src/gs_math.h"
#include "../src/gs_printf.h"


#ifdef GS_BASIC_GL_IMPL
# define GFX_BACKEND_GL33
# include <gl/gl.h>
# include "../src/gfx/gs_gfx.h"
#endif

#include "../src/gs_profiler.c"

#include "../src/gs_string.h"
#include "../src/gs_hash.h"
#include "../src/gs_stretchy_buffer.h"
#include "../src/gs_tokenizer.h"

#endif //GS_BASIC_H
