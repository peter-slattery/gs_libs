/* date = May 10th 2021 10:49 pm */

#ifndef GS_ASSERT_H
#define GS_ASSERT_H

#if !defined(Statement)
# define Statement(s) do{ s }while(0)
#endif

// NOTE(PS): This allows you to override AssertBreak's behavior
// in the event that you want to convert asserts to something else
// like log inputs etc.
#if !defined(AssertBreak)
# define AssertBreak(m) (*((volatile s32*)0) = 0xFFFF)
#endif

#define AssertAlways(c) Statement( if (!(c)) { AssertBreak(c); } )
#define AssertMessageAlways(m) AssertBreak(m)

#if ASSERTS_ACTIVE == 1
# define Assert(c) AssertAlways(c)
# define InvalidCodePath AssertAlways(false)
# define InvalidDefaultCase default: { AssertBreak("invalid default case"); } break;
# define StaticAssert(c) \
enum { \
Glue(gs_AssertFail_, __LINE__) = 1 / (int)(!!(c)), \
}
#else
# define Assert(c)
# define InvalidCodePath
# define InvalidDefaultCase
# define StaticAssert(c)
#endif // ASSERTS_ACTIVE

#endif //GS_ASSERT_H
