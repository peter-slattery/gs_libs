//
// File: gs_types.h
// Author: Peter Slattery
// Creation Date: 2020-04-18
//
#ifndef GS_TYPES_H

#define Glue_(a,b) a##b
#define Glue(a,b) Glue_(a,b)

#define Stringify_(a) #a
#define Stringify(a) Stringify_(a)

#define internal static
#define local_persist static
#define global static
#define local_const static const
#define global_const static const
#define external extern "C"

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
#include <stdint.h>
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

struct s8_array
{
    s8* Values;
    u32 Count;
    u32 Size;
};

struct s16_array
{
    s16* Values;
    u32 Count;
    u32 Size;
};

struct s32_array
{
    s32* Values;
    u32 Count;
    u32 Size;
};

struct s64_array
{
    s64* Values;
    u32 Count;
    u32 Size;
};

struct u8_array
{
    u8* Values;
    u32 Count;
    u32 Size;
};

struct u16_array
{
    u16* Values;
    u32 Count;
    u32 Size;
};

struct u32_array
{
    u32* Values;
    u32 Count;
    u32 Size;
};

struct u64_array
{
    u64* Values;
    u32 Count;
    u32 Size;
};


#define PointerDifference(a,b) ((u8*)(a) - (u8*)(b))
#define PointerToInt(a) PointerDifference(a, 0)
#define Member(type,member) (((type*)0)->member)
#define MemberOffset(type,member) PointerToInt(&Member(type,member))

// NOTE(Peter): Makes sure that s doesn't expand in some way that invalidates
// a nested if statement.
#define Statement(s) do{ s }while(0)

//
// Asserts
//
// AssertAlways and AssertMessageAlways should be used sparingly, because they'll
// still assert in the final build
#define AssertBreak(m) (*((s32*)0) = 0xFFFF)
#define AssertAlways(c) Statement( if (!(c)) { AssertBreak(c); } )
#define AssertMessageAlways(m) AssertBreak(m)

#if !SHIP_MODE
# define Assert(c) AssertAlways(c)
# define AssertMessage(m) AssertBreak(m)
# define InvalidDefaultCase default: { AssertBreak("invalid default case"); } break;
#else
# define Assert(c)
# define AssertMessage(m)
# define InvalidDefaultCase default: {} break;
#endif

#define InvalidCodePath AssertMessage("invalid code path")
#define NotImplemented AssertMessage("not implemented")
#define DontCompile ImAfraidICantDoThat

#define LineNumberString Stringify(__LINE__)
#define FileNameAndLineNumberString __FILE__ ":" LineNumberString ":"

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
#define Abs(x) ((x) < 0) ? x * -1 : x
#define Sign(x) ((x) < 0) ? -1 : 1;
#define IsPowerOfTwo(x) (((x) & ((x) - 1)) == 0)
#define IsOdd(x) (((x) & 1) != 0)

internal void
ZeroMemory_(u8* Memory, u64 Size)
{
    for (u64 i = 0; i < Size; i++)
    {
        Memory[i] = 0;
    }
}

internal void
CopyMemory_(u8* From, u8* To, u64 Size)
{
    for (u64 i = 0; i < Size; i++)
    {
        To[i] = From[i];
    }
}

#define ZeroStruct(str) ZeroMemory_((str), sizeof(str))
#define ZeroArray(arr, type, count) ZeroMemory_((arr), sizeof(type) * (count))

#define CopyArray(from, to, type, count) CopyMemory_((u8*)(from), (u8*)(to), sizeof(type) * (count))

// Singly Linked List Utilities

#define SLLPush_(list_head,new_ele) new_ele->next = list_head, list_head = new_ele
#define SLLPush(list_head,new_ele) (SLLPush_((list_head), (new_ele)))

#define SLLPop_(list_head) list_head=list_head=list_head->next
#define SLLPop(list_head) (SLLPop_((list_head)))

// Vectors

union v2
{
    struct
    {
        float X;
        float Y;
    };
    float E[2];
};

union v3
{
    struct
    {
        float X;
        float Y;
        float Z;
    };
    struct
    {
        float R;
        float G;
        float B;
    };
    struct
    {
        v2 XY;
        float Z;
    };
    struct
    {
        float X;
        v2 YZ;
    };
    float E[3];
};

union v4
{
    struct
    {
        float X;
        float Y;
        float Z;
        float W;
    };
    struct
    {
        float R;
        float G;
        float B;
        float A;
    };
    struct
    {
        v2 XY;
        float Z;
        float W;
    };
    struct
    {
        float X;
        v2 YZ;
        float W;
    };
    struct
    {
        v3 XYZ;
        float W;
    };
    float E[4];
};

#define WhiteV4 v4{1, 1, 1, 1}
#define BlackV4 v4{0, 0, 0, 1}
#define RedV4 v4{1, 0, 0, 1}
#define GreenV4 v4{0, 1, 0, 1}
#define BlueV4 v4{0, 0, 1, 1}
#define YellowV4 v4{1, 1, 0, 1}
#define TealV4 v4{0, 1, 1, 1}
#define PinkV4 v4{1, 0, 1, 1}

struct v2_array
{
    v2* Vectors;
    u32 Count;
    u32 Size;
};

struct v3_array
{
    v3* Vectors;
    u32 Count;
    u32 Size;
};

struct v4_array
{
    v4* Vectors;
    u32 Count;
    u32 Size;
};

struct range1
{
    r32 Min;
    r32 Max;
};

struct range2
{
    v2 Min;
    v2 Max;
};
typedef range2 rect2;

struct range3
{
    v3 Min;
    v3 Max;
};

struct range4
{
    v4 Min;
    v4 Max;
};

struct range1_array
{
    range1* Ranges;
    u32 Count;
    u32 Size;
};

struct range2_array
{
    range2* Ranges;
    u32 Count;
    u32 Size;
};

struct range3_array
{
    range3* Ranges;
    u32 Count;
    u32 Size;
};

struct range4_array
{
    range4* Ranges;
    u32 Count;
    u32 Size;
};

// Matrices
// NOTE(Peter): All matrices are stored in row major order

union m33
{
    struct
    {
        float a; float b; float c;
        float d; float e; float f;
        float g; float h; float i;
    };
    float E[9];
};

union m44
{
    struct
    {
        float a; float b; float c; float d;
        float e; float f; float g; float h;
        float i; float j; float k; float l;
        float m; float n; float o; float p;
    };
    float E[16];
};

struct m33_array
{
    m33* Matrices;
    u32 Count;
    u32 Size;
};

struct m44_array
{
    m44* Matrices;
    u32 Count;
    u32 Size;
};

//////////////////////////
//
// Strings

struct gs_const_string
{
    union
    {
        char* Str;
        u8* Data;
    };
    u64 Length;
};

struct gs_string
{
    union
    {
        gs_const_string ConstString;
        struct
        {
            char* Str;
            u64 Length;
        };
    };
    u64 Size;
};

struct gs_const_string_array
{
    gs_const_string* Strings;
    u64 Count;
    u64 Used;
};

struct gs_string_array
{
    gs_string* Strings;
    u64 Count;
    u64 Used;
};

internal u64
CStringLength(char* Str)
{
    char* At = Str;
    while (*At) { At++; }
    return PointerDifference(At, Str);
}

#define LitString(cstr) gs_const_string{(cstr), CStringLength(cstr) }

// The index of the character in these arrays corresponds to its value as a number in
// the relevant base, so you can do FindFirst on them with a char to get the int value
//   ie. 3 is at index 3 in Base10Chars.
//   ie. C is at index 12 in Base16Chars.
global_const gs_const_string Base8Chars  = LitString("01234567");
global_const gs_const_string Base10Chars = LitString("0123456789");
global_const gs_const_string Base16Chars = LitString("0123456789ABCDEF");

//////////////////////////
//
// Memory

struct gs_data
{
    u8* Memory;
    u64 Size;
};

enum gs_access_flag
{
    gs_AccessFlag_Read  = 1 << 0,
    gs_AccessFlag_Write = 1 << 1,
    gs_AccessFlag_Exec  = 1 << 2,
};

typedef s32 gs_scan_direction;
enum
{
    gs_Scan_Backward = -1,
    gs_Scan_Forward  = 1,
};

typedef void* allocator_allocate(u64 Size, u64* SizeResult);
typedef void  allocator_free(void* Ptr, u64 Size);

struct gs_allocator
{
    allocator_allocate* Alloc;
    allocator_free* Free;
};

struct gs_memory_cursor
{
    gs_data Data;
    u64 Position;
};

struct gs_temp_memory_cursor
{
    gs_memory_cursor* Cursor;
    u64 CursorPosition;
};

struct gs_memory_cursor_list
{
    gs_memory_cursor  Cursor;
    union
    {
        gs_memory_cursor_list* Next;
        gs_memory_cursor_list* Prev;
    };
};

struct gs_memory_arena
{
    gs_allocator* Allocator;
    gs_memory_cursor_list* CursorList;
    u64 MemoryChunkSize;
    u64 MemoryAlignment;
};

struct gs_temp_memory_arena
{
    gs_memory_arena* Arena;
    gs_memory_cursor_list* CursorListSnapshot;
    u64 PositionSnapshot;
    
    gs_temp_memory_arena(gs_memory_arena* Arena);
    gs_temp_memory_arena(gs_memory_arena* Arena, gs_memory_cursor_list* CursorListSnapshot, u64 PositionSnapshot);
    ~gs_temp_memory_arena();
};

struct gs_memory_arena_array
{
    gs_memory_arena* Arenas;
    u32 Count;
    u32 Size;
};

///////////////////////////////
//
// Dynamic Array
//
// I like having constant lookup times, along with growable arrays. :)
// NOTE(Peter): If you're using this, you probably want to write a Get<element_type>
// procedure to auto cast the result to the type you want. I'm even providing a
// debug check for you to make sure that you're requesting a size that matches
// the ElementSize for extra safety.

struct gs_dynarray_buffer
{
    u8* Memory;
};

struct gs_dynarray
{
    gs_allocator* Allocator;
    
    gs_dynarray_buffer* Buffers;
    u64 BuffersCount;
    u64 ElementCount;
    
    u64 ElementSize;
    u64 ElementsPerBuffer;
};

struct gs_dynarray_handle
{
    u64 BufferIndex;
    u64 IndexInBuffer;
#if !SHIP_MODE
    gs_dynarray* Array;
#endif
};

struct gs_dynarray_handle_list
{
    gs_dynarray_handle* Handles;
    u32 Count;
    u32 Size;
};

///////////////////////////////
//
// File IO

enum enumerate_directory_flag
{
    EnumerateDirectory_Recurse = 1 << 0,
    EnumerateDirectory_IncludeDirectories = 1 << 1,
};

struct gs_file
{
    gs_data FileData;
    gs_const_string Path;
};

typedef u64 file_handler_get_file_size(gs_file_handler FileHandler, gs_const_string Path);
typedef gs_file* file_handler_read_entire_file(gs_file_handler FileHandler, gs_const_string Path, gs_data Memory);
typedef bool  file_handler_write_entire_file(gs_file_handler FileHandler, gs_const_string Path, gs_data Data);
typedef gs_const_string_array file_handler_enumerate_directory(gs_file_handler FileHandler, gs_const_string Path, u32 Flags);

struct gs_file_handler
{
    file_handler_read_entire_file* ReadEntireFile;
    file_handler_write_entire_file* WriteEntireFile;
    file_handler_enumerate_directory* EnumerateDirectory;
    gs_allocator* Allocator;
};

///////////////////////////////
//
// Random

struct gs_random_series
{
    u32 Value;
};

#define GS_TYPES_H
#endif // GS_TYPES_H