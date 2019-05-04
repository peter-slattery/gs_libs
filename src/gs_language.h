#ifndef GS_LANGUAGE_H

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#include <windows.h>

#elif defined(__APPLE__) && defined(__MAC__)
// TODO(Peter): 

#else // Std lib
#include <stdlib.h>

#endif

#define internal static
#define local_persist static
#define global_variable static


#if !defined(GS_TYPES)

#define GSINT64(s) (s) ## L
#define GSUINT64(s) (s) ## UL

typedef signed char    b8;
typedef short int      b16;
typedef int            b32;
typedef long long int  b64;

typedef unsigned char          u8;
typedef unsigned short int     u16;
typedef unsigned int           u32;
typedef unsigned long long int u64;

typedef signed char   s8;
typedef short int     s16;
typedef int           s32;
typedef long long int s64;

typedef float  r32;
typedef double r64;

#define INT8_MIN   (-128)
#define INT16_MIN  (-32767-1)
#define INT32_MIN  (-2147483647-1)
#define INT64_MIN  (-GSINT64(9223372036854775807)-1)

#define INT8_MAX   (127)
#define INT16_MAX  (32767)
#define INT32_MAX  (2147483647)
#define INT64_MAX  (GSINT64(9223372036854775807))

#define UINT8_MAX  (255)
#define UINT16_MAX (65535)
#define UINT32_MAX (4294967295U)
#define UINT64_MAX (GSUINT64(18446744073709551615))

#define FLOAT_MIN  (1.175494351e-38F)
#define FLOAT_MAX  (3.402823466e+38F)
#define DOUBLE_MIN (2.2250738585072014e-308)
#define DOUBLE_MAX (1.7976931348623158e+308)

#define Kilobytes(Value) ((Value) * 1024)
#define Megabytes(Value) (Kilobytes(Value) * 1024)
#define Gigabytes(Value) (Megabytes(Value) * 1024)
#define Terabytes(Value) (Gigabytes(Value) * 1024)

#define GS_TYPES
#endif


#ifdef DEBUG

#if !defined(Assert)
// NOTE(peter): this writes to address 0 which is always illegal and will cause a crash
#define Assert(expression) if(!(expression)){ *((int *)0) = 5; }
#endif

#define InvalidCodePath Assert(0);
#define STBI_ASSERT(x) Assert(x)

#else

#define Assert(expression)
#define InvalidCodePath

#endif // DEBUG


static void
GSZeroMemory (u8* Memory, s32 Size)
{
    for (int i = 0; i < Size; i++) { Memory[i] = 0; }
}

#define GSMinDef(type) static type GSMin(type A, type B) { return (A < B ? A : B); }
GSMinDef(s8)
GSMinDef(s16)
GSMinDef(s32)
GSMinDef(s64)
GSMinDef(u8)
GSMinDef(u16)
GSMinDef(u32)
GSMinDef(u64)
GSMinDef(r32)
GSMinDef(r64)
#undef GSMinDef

#define GSMaxDef(type) static type GSMax(type A, type B) { return (A > B ? A : B); }
GSMaxDef(s8)
GSMaxDef(s16)
GSMaxDef(s32)
GSMaxDef(s64)
GSMaxDef(u8)
GSMaxDef(u16)
GSMaxDef(u32)
GSMaxDef(u64)
GSMaxDef(r32)
GSMaxDef(r64)
#undef GSMaxDef

#define GSAbsDef(type) static type GSAbs(type A) { return (A < 0 ? -A : A); }
GSAbsDef(s8)
GSAbsDef(s16)
GSAbsDef(s32)
GSAbsDef(s64)
GSAbsDef(r32)
GSAbsDef(r64)
#undef GSAbsDef

#define GSPowDef(type) static type GSPow(type N, s32 Power) { \
        type Result = N; \
        for(s32 i = 1; i < Power; i++) { Result *= N; } \
        return Result; \
}
GSPowDef(s8)
GSPowDef(s16)
GSPowDef(s32)
GSPowDef(s64)
GSPowDef(u8)
GSPowDef(u16)
GSPowDef(u32)
GSPowDef(u64)
GSPowDef(r32)
GSPowDef(r64)
#undef GSPowDef

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)

internal u8* Allocate (s32 Size) 
{
    return (u8*)VirtualAlloc(NULL, Size, 
                             MEM_COMMIT | MEM_RESERVE, 
                             PAGE_EXECUTE_READWRITE);
}

internal b32 Free (u8* Base, u32 Size)
{
    b32 Result = false;
    Result = VirtualFree(Base, Size, MEM_RELEASE);
    return Result;
}

#elif defined(__APPLE__) && defined(__MAC__)


internal u8* Allocate (s32 Size) 
{
    // TODO(Peter): 
    return 0; 
}

internal void Free (u8* Base, u32 Size)
{
    // TODO(Peter): 
    b32 Result = false;
    return Result;
}

#else // Std lib

internal u8* Allocate (s32 Size) 
{
    return (u8*)malloc(Size); 
}

internal void Free (u8* Base, u32 Size)
{
    b32 Result = true;
    free(Base);
    return Result;
}

#endif

#define GS_LANGUAGE_H
#endif