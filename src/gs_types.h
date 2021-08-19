/* date = May 10th 2021 10:20 pm */

#ifndef GS_TYPES_H
#define GS_TYPES_H

//
// Scoping macros
//
#define internal static
#define local_persist static
#define global static
#define local_const static const
#define global_const static const
#define external extern "C"

//
// Macro utilities
//
#define Glue_(a,b) a##b
#define Glue(a,b) Glue_(a,b)

#define Stringify_(a) #a
#define Stringify(a) Stringify_(a)

#define DontCompile ImAfraidICantDoThat

//
// Metaprogramming Macros
//
#define PointerDifference(a,b) ((u8*)(a) - (u8*)(b))
#define PointerToInt(a) PointerDifference(a, 0)
#define Member(type,member) (((type*)0)->member)
#define MemberOffset(type,member) PointerToInt(&Member(type,member))

//
// Basic types
//
#if !defined(GUESS_INTS)
# include <stdint.h>
#endif // !defined(GUESS_INTS)

#if defined(GUESS_INTS)
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#else
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#endif

typedef s8 b8;
typedef s32 b32;
typedef s64 b64;

typedef float r32;
typedef double r64;

//
// Boundary values of basic types
//
global_const u8  MaxU8  = 0xFF;
global_const u16 MaxU16 = 0xFFFF;
global_const u32 MaxU32 = 0xFFFFFFFF;
global_const u64 MaxU64 = 0xFFFFFFFFFFFFFFFF;

global_const s8  MaxS8  = 127;
global_const s16 MaxS16 = 32767;
global_const s32 MaxS32 = 2147483647;
global_const s64 MaxS64 = 9223372036854775807;

global_const s8  MinS8  = -127 - 1;
global_const s16 MinS16 = -32767 - 1;
global_const s32 MinS32 = -2147483647 - 1;
global_const s64 MinS64 = -9223372036854775807 - 1;

global_const r32 MaxR32  = 3.402823466e+38f;
global_const r32 MinR32  = -MaxR32;
global_const r32 SmallestPositiveR32 = 1.1754943508e-38f;
global_const r32 EpsilonR32 = 5.96046448e-8f;

global_const r32 PiR32 = 3.14159265359f;
global_const r32 HalfPiR32 = 1.5707963267f;
global_const r32 TauR32 = 6.28318530717f;

global_const r64 MaxR64 = 1.79769313486231e+308;
global_const r64 MinR64 = -MaxR64;
global_const r64 SmallestPositiveR64 = 4.94065645841247e-324;
global_const r64 EpsilonR64 = 1.11022302462515650e-16;

global_const r64 NanosToSeconds = 1 / 10000000.0;
global_const r64 SecondsToNanos = 10000000.0;

internal r32
DegToRadR32(r32 Degrees)
{
  return (Degrees * (PiR32 / 180.0f));
}

internal r32
RadToDegR32(r32 Radians)
{
  return (Radians * (180.0f / PiR32));
}

//
// Debug Utilities
//

typedef struct gs_debug_loc
{
  char* File;
  char* Function;
  u32 Line;
} gs_debug_loc;

#define DEBUG_LOC (gs_debug_loc){ (char*)__FILE__, (char*)__FUNCTION__, __LINE__ }

//
// Compound Types
//

typedef struct gs_data32
{
  u8* Memory;
  u32 Size;
} gs_data32;

typedef struct gs_data64
{
  u8* Memory;
  u64 Size;
} gs_data64, gs_data;

//
// Utility Functions
//

#define Bytes(x) (x)
#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) (((u64)x) << 40)

#define HasFlag(data, flag) (((data) & (flag)) != 0)
#define HasFlagOnly(data, flag) (((data) & (flag)) == (data))
#define AddFlag(data, flag) ((data) |= (flag))
#define RemoveFlag(data, flag) ((data) &= (~(flag)))

#define Max(a,b) (((a) > (b)) ? (a) : (b))
#define Min(a,b) (((a) > (b)) ? (b) : (a))
#define Clamp_(a,x,b) ((x < a) ? a : ((x > b) ? b : x))
#define Clamp(a,x,b) Clamp_((a), (x), (b))
#define Clamp01(x) Clamp_(0.0f, (x), 1.0f)
#define Abs(x) (((x) < 0) ? ((x) * -1) : x)
#define Sign(x) (((x) < 0) ? -1 : 1)
#define IsPowerOfTwo(x) (((x) & ((x) - 1)) == 0)
#define IsOdd(x) (((x) & 1) != 0)

#define CArrayLength(arr) (sizeof(arr) / (sizeof(arr[0])))

internal u64
CStringLength(char* Str)
{
  char* At = Str;
  while (At[0] != 0) At++;
  u64 Result = At - Str;
  return Result;
}

internal bool
CStringsEqual(char* A, char* B)
{
  bool Result = true;
  char* AAt = A;
  char* BAt = B;
  while (*AAt != 0 && *BAt != 0)
  {
    if (*AAt != *BAt)
    {
      Result = false;
      break;
    }
    
    AAt++;
    BAt++;
  }
  return Result;
}

internal u64
RoundUpTo64(u64 Value, u64 Alignment)
{
  Value += Alignment - 1;
  Value -= Value % Alignment;
  return Value;
}

// SLL

#define SLLPush(ele, root, head) (head)->Next = (ele); (head)=(ele); (head)->Next=0
#define SLLPushOrInit(ele, root, head) if (!(root)) { (root)=(ele); (head)=(ele); } SLLPush(ele, root, head)

#endif //GS_TYPES_H
