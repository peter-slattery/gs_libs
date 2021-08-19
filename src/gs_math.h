/* date = May 13th 2021 5:29 pm */

#ifndef GS_MATH_H
#define GS_MATH_H

#include <math.h>
#include <intrin.h>

#if !defined(GS_MATH_PROFILE_FUNC)
# define GS_MATH_PROFILE_FUNC
#endif

//
// Basics
//

internal u32
U32DivideRoundUp (u32 A, u32 B)
{
  GS_MATH_PROFILE_FUNC;
  
  r32 Result = (r32)A / (r32)B;
  Result += .99999f;
  return (u32)Result;
}

inline bool XOR(bool A, bool B) { return (A == !B); }
inline bool B8XOR(b8 A, b8 B) { return (A == !B); }
inline bool B32XOR(b32 A, b32 B) { return (A == !B); }
inline bool B64XOR(b64 A, b64 B) { return (A == !B); }

internal u32
RoundUpToMultiple(u32 Value, u32 MultipleOf)
{
  GS_MATH_PROFILE_FUNC;
  
  u32 Result = Value;
  if (MultipleOf != 0)
  {
    u32 Remainder = Value % MultipleOf;
    Result = Value + (MultipleOf - Remainder);
  }
  return Result;
}

internal u32
RoundUpToPow2U32(u32 Value)
{
  GS_MATH_PROFILE_FUNC;
  
  u32 Result = Value - 1;
  Result |= Result >> 1;
  Result |= Result >> 2;
  Result |= Result >> 4;
  Result |= Result >> 8;
  Result |= Result >> 16;
  Result++;
  return Result;
}

internal u32
RoundUpToPow2U64(u64 Value)
{
  GS_MATH_PROFILE_FUNC;
  
  u64 Result = Value - 1;
  Result |= Result >> 1;
  Result |= Result >> 2;
  Result |= Result >> 4;
  Result |= Result >> 8;
  Result |= Result >> 16;
  Result |= Result >> 32;
  Result++;
  return Result;
}

internal u8
PowU8(u8 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  u8 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal u16
PowU16(u16 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  u16 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal u32
PowU32(u32 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  u32 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal u64
PowU64(u64 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  u64 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal s8
PowS8(s8 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  s8 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal s16
PowS16(s16 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  s16 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal s32
PowS32(s32 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  s32 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal s64
PowS64(s64 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  s64 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal r32
PowR32(r32 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  r32 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}
internal r64
PowR64(r64 X, u32 Power)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 Result = X;
  for (u32 i = 1; i < Power; i++) { Result *= X; }
  return Result;
}

internal u8
LerpU8(r32 T, u8 A, u8 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u8)((A * (1.0f - T)) + (B * T));
}
internal u16
LerpU16(r32 T, u16 A, u16 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u16)((A * (1.0f - T)) + (B * T));
}
internal u32
LerpU32(r32 T, u32 A, u32 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u32)((A * (1.0f - T)) + (B * T));
}
internal u64
LerpU64(r32 T, u64 A, u64 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u64)((A * (1.0f - T)) + (B * T));
}
internal s8
LerpS8(r32 T, s8 A, s8 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s8)((A * (1.0f - T)) + (B * T));
}
internal s16
LerpS16(r32 T, s16 A, s16 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s16)((A * (1.0f - T)) + (B * T));
}
internal s32
LerpS32(r32 T, s32 A, s32 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s32)((A * (1.0f - T)) + (B * T));
}
internal s64
LerpS64(r32 T, s64 A, s64 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s64)((A * (1.0f - T)) + (B * T));
}
internal r32
LerpR32(r32 T, r32 A, r32 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (r32)((A * (1.0f - T)) + (B * T));
}
internal r64
LerpR64(r32 T, r64 A, r64 B)
{
  GS_MATH_PROFILE_FUNC;
  
  return (r64)((A * (1.0f - T)) + (B * T));
}

internal u8
UnlerpU8(u8 Value, u8 Min, u8 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u8)((r64)(Value - Min) / (r64)(Max - Min));
}
internal u16
UnlerpU16(u16 Value, u16 Min, u16 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u16)((r64)(Value - Min) / (r64)(Max - Min));
}
internal u32
UnlerpU32(u32 Value, u32 Min, u32 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u32)((r64)(Value - Min) / (r64)(Max - Min));
}
internal u64
UnlerpU64(u64 Value, u64 Min, u64 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (u64)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s8
UnlerpS8(s8 Value, s8 Min, s8 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s8)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s16
UnlerpS16(s16 Value, s16 Min, s16 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s16)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s32
UnlerpS32(s32 Value, s32 Min, s32 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s32)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s64
UnlerpS64(s64 Value, s64 Min, s64 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (s64)((r64)(Value - Min) / (r64)(Max - Min));
}
internal r32
UnlerpR32(r32 Value, r32 Min, r32 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (Value - Min) / (Max - Min);
}
internal r64
UnlerpR64(r64 Value, r64 Min, r64 Max)
{
  GS_MATH_PROFILE_FUNC;
  
  return (Value - Min) / (Max - Min);
}


internal u8
RemapU8(u8 Value, u8 OldMin, u8 OldMax, u8 NewMin, u8 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  u8 Result = (u8)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal u16
RemapU16(u16 Value, u16 OldMin, u16 OldMax, u16 NewMin, u16 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  u16 Result = (u16)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal u32
RemapU32(u32 Value, u32 OldMin, u32 OldMax, u32 NewMin, u32 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  u32 Result = (u32)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal u64
RemapU64(u64 Value, u64 OldMin, u64 OldMax, u64 NewMin, u64 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  u64 Result = (u64)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal s8
RemapS8(s8 Value, s8 OldMin, s8 OldMax, s8 NewMin, s8 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  s8 Result = (s8)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal s16
RemapS16(s16 Value, s16 OldMin, s16 OldMax, s16 NewMin, s16 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  s16 Result = (s16)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal s32
RemapS32(s32 Value, s32 OldMin, s32 OldMax, s32 NewMin, s32 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  s32 Result = (s32)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal s64
RemapS64(s64 Value, s64 OldMin, s64 OldMax, s64 NewMin, s64 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  s64 Result = (s64)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal r32
RemapR32(r32 Value, r32 OldMin, r32 OldMax, r32 NewMin, r32 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r32 Result = (r32)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}
internal r64
RemapR64(r64 Value, r64 OldMin, r64 OldMax, r64 NewMin, r64 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 Result = (r64)((A * (NewMax - NewMin)) + NewMin);
  return Result;
}

internal u8
RemapClampedU8(u8 Value, u8 OldMin, u8 OldMax, u8 NewMin, u8 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  u8 Result = (u8)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal u16
RemapClampedU16(u16 Value, u16 OldMin, u16 OldMax, u16 NewMin, u16 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  u16 Result = (u16)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal u32
RemapClampedU32(u32 Value, u32 OldMin, u32 OldMax, u32 NewMin, u32 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  u32 Result = (u32)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal u64
RemapClampedU64(u64 Value, u64 OldMin, u64 OldMax, u64 NewMin, u64 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  u64 Result = (u64)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal s8
RemapClampedS8(s8 Value, s8 OldMin, s8 OldMax, s8 NewMin, s8 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  s8 Result = (s8)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal s16
RemapClampedS16(s16 Value, s16 OldMin, s16 OldMax, s16 NewMin, s16 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  s16 Result = (s16)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal s32
RemapClampedS32(s32 Value, s32 OldMin, s32 OldMax, s32 NewMin, s32 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  s32 Result = (s32)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal s64
RemapClampedS64(s64 Value, s64 OldMin, s64 OldMax, s64 NewMin, s64 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  s64 Result = (s64)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal r32
RemapClampedR32(r32 Value, r32 OldMin, r32 OldMax, r32 NewMin, r32 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  r32 Result = (r32)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}
internal r64
RemapClampedR64(r64 Value, r64 OldMin, r64 OldMax, r64 NewMin, r64 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
  r64 AClamped = Clamp01(A);
  r64 UnclampedResult = ((AClamped * (NewMax - NewMin)) + NewMin);
  r64 Result = (r64)Clamp(NewMin, UnclampedResult, NewMax);
  return Result;
}

internal r32
FloorR32(r32 V)
{
  GS_MATH_PROFILE_FUNC;
  
  return (r32)((s64)V);
}
internal r64
FloorR64(r64 V)
{
  GS_MATH_PROFILE_FUNC;
  
  return (r64)((s64)V);
}

internal r32
FractR32(r32 V)
{
  GS_MATH_PROFILE_FUNC;
  
  return V - FloorR32(V);
}
internal r64
FractR64(r64 V)
{
  GS_MATH_PROFILE_FUNC;
  
  return V - FloorR64(V);
}

internal r32
SqrtR32(r32 V)
{
  GS_MATH_PROFILE_FUNC;
  
  return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(V)));
}
internal u32
SqrtU32(u32 V)
{
  GS_MATH_PROFILE_FUNC;
  
  return sqrt(V);
}

internal r32
ModR32(r32 Value, r32 Int)
{
  GS_MATH_PROFILE_FUNC;
  
  r32 Div = Value / Int;
  r32 Fract = Abs(FractR32(Div));
  return Int * Fract;
}
internal r64
ModR64(r64 Value, r64 Int)
{
  GS_MATH_PROFILE_FUNC;
  
  r64 Div = Value / Int;
  r64 Fract = Abs(FractR64(Div));
  return Int * Fract;
}

internal r32
SinR32(r32 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return sinf(Rad);
}
internal r64
SinR64(r64 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return sin(Rad);
}

internal r32
CosR32(r32 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return cosf(Rad);
}
internal r64
CosR64(r64 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return cos(Rad);
}

internal r32
TanR32(r32 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return tanf(Rad);
}
internal r64
TanR64(r64 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return tan(Rad);
}

internal r32
ASinR32(r32 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return asinf(Rad);
}
internal r64
ASinR64(r64 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return asin(Rad);
}

internal r32
ACosR32(r32 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return acosf(Rad);
}
internal r64
ACosR64(r64 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return acos(Rad);
}

internal r32
ATanR32(r32 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return atanf(Rad);
}
internal r64
ATanR64(r64 Rad)
{
  GS_MATH_PROFILE_FUNC;
  
  return atan(Rad);
}

//
// Vectors
//
typedef union v2
{
  struct
  {
    r32 x;
    r32 y;
  };
  r32 E[2];
} v2;

typedef union v3
{
  struct
  {
    r32 x;
    r32 y;
    r32 z;
  };
  struct
  {
    r32 r;
    r32 g;
    r32 b;
  };
  struct
  {
    v2 xy;
    r32 _z0;
  };
  struct
  {
    r32 _x0;
    v2 yz;
  };
  r32 E[3];
} v3;

typedef union v4
{
  struct
  {
    r32 x;
    r32 y;
    r32 z;
    r32 w;
  };
  struct
  {
    r32 r;
    r32 g;
    r32 b;
    r32 a;
  };
  struct
  {
    v2 xy;
    v2 zw;
  };
  struct
  {
    r32 _x0;
    v2 yz;
    r32 _w0;
  };
  struct
  {
    v3 xyz;
    r32 _w1;
  };
  r32 E[4];
} v4;

#define ZeroPointV4 (v4){0, 0, 0, 1}
#define ZeroVectorV4 (v4){0, 0, 0, 1}

#define ForwardV4 (v4){0, 0, 1, 0}
#define UpV4 (v4){0, 1, 0, 0}
#define RightV4 (v4){1, 0, 0, 0}
#define BackV4 (v4){0, 0, -1, 0}
#define DownV4 (v4){0, -1, 0, 0}
#define LeftV4 (v4){-1, 0, 0, 0}

#define WhiteV4 (v4){1, 1, 1, 1}
#define BlackV4 (v4){0, 0, 0, 1}
#define RedV4 (v4){1, 0, 0, 1}
#define GreenV4 (v4){0, 1, 0, 1}
#define BlueV4 (v4){0, 0, 1, 1}
#define YellowV4 (v4){1, 1, 0, 1}
#define TealV4 (v4){0, 1, 1, 1}
#define PinkV4 (v4){1, 0, 1, 1}

internal v2
V2Negate(v2 A)
{
  GS_MATH_PROFILE_FUNC;
  
  v2 Result = (v2){
    -A.x,
    -A.y,
  };
  return Result;
}

internal v3
V3Negate(v3 A)
{
  GS_MATH_PROFILE_FUNC;
  
  v3 Result = (v3){
    -A.x,
    -A.y,
    -A.z,
  };
  return Result;
}

internal v4
V4Negate(v4 A)
{
  GS_MATH_PROFILE_FUNC;
  
  v4 Result = (v4){
    -A.x,
    -A.y,
    -A.z,
    -A.w,
  };
  return Result;
}

internal v2
V2Add(v2 A, v2 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v2 Result = (v2){
    A.x + B.x,
    A.y + B.y,
  };
  return Result;
}

internal v3
V3Add(v3 A, v3 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v3 Result = (v3){
    A.x + B.x,
    A.y + B.y,
    A.z + B.z,
  };
  return Result;
}

internal v4
V4Add(v4 A, v4 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v4 Result = (v4){
    A.x + B.x,
    A.y + B.y,
    A.z + B.z,
    A.w + B.w,
  };
  return Result;
}

internal v2
V2Sub(v2 A, v2 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v2 Result = (v2){
    A.x - B.x,
    A.y - B.y,
  };
  return Result;
}

internal v3
V3Sub(v3 A, v3 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v3 Result = (v3){
    A.x - B.x,
    A.y - B.y,
    A.z - B.z,
  };
  return Result;
}

internal v4
V4Sub(v4 A, v4 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v4 Result = (v4){
    A.x - B.x,
    A.y - B.y,
    A.z - B.z,
    A.w - B.w,
  };
  return Result;
}

internal v2
V2Scale(v2 A, r32 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v2 Result = (v2){
    A.x * B,
    A.y * B,
  };
  return Result;
}

internal v3
V3Scale(v3 A, r32 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v3 Result = (v3){
    A.x * B,
    A.y * B,
    A.z * B,
  };
  return Result;
}

internal v4
V4Scale(v4 A, r32 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v4 Result = (v4){
    A.x * B,
    A.y * B,
    A.z * B,
    A.w * B,
  };
  return Result;
}

internal v2
V2MultiplyPairwise(v2 A, v2 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v2 Result = (v2){
    A.x * B.x,
    A.y * B.y,
  };
  return Result;
}

internal v3
V3MultiplyPairwise(v3 A, v3 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v3 Result = (v3){
    A.x * B.x,
    A.y * B.y,
    A.z * B.z,
  };
  return Result;
}

internal v4
V4MultiplyPairwise(v4 A, v4 B)
{
  GS_MATH_PROFILE_FUNC;
  
  v4 Result = (v4){
    A.x * B.x,
    A.y * B.y,
    A.z * B.z,
    A.w * B.w,
  };
  return Result;
}

internal bool
V2Equals(v2 A, v2 B)
{
  GS_MATH_PROFILE_FUNC;
  
  bool Result = (A.x == B.x &&
                 A.y == B.y);
  return Result;
}

internal bool
V3Equals(v3 A, v3 B)
{
  GS_MATH_PROFILE_FUNC;
  
  bool Result = (A.x == B.x &&
                 A.y == B.y &&
                 A.z == B.z);
  return Result;
}

internal bool
V4Equals(v4 A, v4 B)
{
  GS_MATH_PROFILE_FUNC;
  
  bool Result = (A.x == B.x &&
                 A.y == B.y &&
                 A.z == B.z &&
                 A.w == B.w);
  return Result;
}

#ifdef __cplusplus
v2 operator- (v2 A) { return V2Negate(A); }
v3 operator- (v3 A) { return V3Negate(A); }
v4 operator- (v4 A) { return V4Negate(A); }

v2 operator+ (v2 A, v2 B) { return V2Add(A, B); }
v3 operator+ (v3 A, v3 B) { return V3Add(A, B); }
v4 operator+ (v4 A, v4 B) { return V4Add(A, B); }

v2 operator- (v2 A, v2 B) { return V2Sub(A, B); }
v3 operator- (v3 A, v3 B) { return V3Sub(A, B); }
v4 operator- (v4 A, v4 B) { return V4Sub(A, B); }

void operator+= (v2& A, v2 B) { A = V2Add(A, B); }
void operator+= (v3& A, v3 B) { A = V3Add(A, B); }
void operator+= (v4& A, v4 B) { A = V4Add(A, B); }

void operator-= (v2& A, v2 B) { A = V2Sub(A, B); }
void operator-= (v3& A, v3 B) { A = V3Sub(A, B); }
void operator-= (v4& A, v4 B) { A = V4Sub(A, B); }

v2 operator* (v2 A, r32 B) { return V2Scale(A, B); }
v3 operator* (v3 A, r32 B) { return V3Scale(A, B); }
v4 operator* (v4 A, r32 B) { return V4Scale(A, B); }

v2 operator/ (v2 A, r32 B) { B = 1.0f / B; return V2Scale(A, B); }
v3 operator/ (v3 A, r32 B) { B = 1.0f / B; return V3Scale(A, B); }
v4 operator/ (v4 A, r32 B) { B = 1.0f / B; return V4Scale(A, B); }

void operator*= (v2& A, r32 B) { A = V2Scale(A, B); }
void operator*= (v3& A, r32 B) { A = V3Scale(A, B); }
void operator*= (v4& A, r32 B) { A = V4Scale(A, B); }

void operator/= (v2& A, r32 B) { B = 1.0f / B; A = V2Scale(A, B); };
void operator/= (v3& A, r32 B) { B = 1.0f / B; A = V3Scale(A, B); };
void operator/= (v4& A, r32 B) { B = 1.0f / B; A = V4Scale(A, B); };

bool operator == (v2 A, v2 B) { return V2Equals(A, B); }
bool operator == (v3 A, v3 B) { return V3Equals(A, B); }
bool operator == (v4 A, v4 B) { return V4Equals(A, B); }

bool operator != (v2 A, v2 B) { return !V2Equals(A, B); }
bool operator != (v3 A, v3 B) { return !V3Equals(A, B); }
bool operator != (v4 A, v4 B) { return !V4Equals(A, B); }
#endif

internal v3 V2ToV3(v2 V, r32 Z) 
{
  GS_MATH_PROFILE_FUNC;
  return (v3){V.x, V.y, Z}; 
}
internal v4 V2ToV4(v2 V, r32 Z, r32 W) 
{
  GS_MATH_PROFILE_FUNC;
  return (v4){V.x, V.y, Z, W}; 
}
internal v4 ToV4_(v3 V, r32 W)
{
  GS_MATH_PROFILE_FUNC;
  return (v4){V.x, V.y, V.z, W};
}
// all points have a w value of 1
#define ToV4Point(v) ToV4_((v), 1.0f) 
// all vectors have a w value of 0 ie. they cannot be translated
#define ToV4Vec(v) ToV4_((v), 0.0f) 

internal r32 V2MagSquared(v2 V) 
{
  GS_MATH_PROFILE_FUNC;
  return ((V.x * V.x) + (V.y * V.y)); 
}
internal r32 V3MagSquared(v3 V) 
{
  GS_MATH_PROFILE_FUNC;
  return ((V.x * V.x) + (V.y * V.y) + (V.z * V.z)); 
}
internal r32 V4MagSquared(v4 V) 
{ 
  GS_MATH_PROFILE_FUNC;
  return ((V.x * V.x) + (V.y * V.y) + (V.z * V.z) + (V.w * V.w)); 
}

internal r32 V2Mag(v2 V) 
{ 
  GS_MATH_PROFILE_FUNC;
  return SqrtR32(V2MagSquared(V)); 
}
internal r32 V3Mag(v3 V) 
{ 
  GS_MATH_PROFILE_FUNC;
  return SqrtR32(V3MagSquared(V)); 
}
internal r32 V4Mag(v4 V) 
{ 
  GS_MATH_PROFILE_FUNC;
  return SqrtR32(V4MagSquared(V)); 
}

internal r32 V2DistanceSquared(v2 A, v2 B) 
{
  GS_MATH_PROFILE_FUNC;
  v2 Delta = V2Sub(A, B);
  return V2MagSquared(Delta); 
}
internal r32 V3DistanceSquared(v3 A, v3 B)
{
  GS_MATH_PROFILE_FUNC;
  v3 Delta = V3Sub(A, B);
  return V3MagSquared(Delta); 
}
internal r32 V4DistanceSquared(v4 A, v4 B)
{
  GS_MATH_PROFILE_FUNC;
  v4 Delta = V4Sub(A, B);
  return V4MagSquared(Delta); 
}

internal r32 V2Distance(v2 A, v2 B) 
{
  GS_MATH_PROFILE_FUNC;
  return SqrtR32(V2DistanceSquared(A, B)); 
}
internal r32 V3Distance(v3 A, v3 B) 
{
  GS_MATH_PROFILE_FUNC;
  return SqrtR32(V3DistanceSquared(A, B)); 
}
internal r32 V4Distance(v4 A, v4 B) 
{
  GS_MATH_PROFILE_FUNC;
  return SqrtR32(V4DistanceSquared(A, B)); 
}

internal v2
V2Normalize(v2 A)
{
  GS_MATH_PROFILE_FUNC;
  r32 Magnitude = V2Mag(A);
  v2 Result = V2Scale(A, 1 / Magnitude);
  return Result;
}
internal v3
V3Normalize(v3 A)
{
  GS_MATH_PROFILE_FUNC;
  r32 Magnitude = V3Mag(A);
  v3 Result = V3Scale(A, 1 / Magnitude);
  return Result;
}
internal v4
V4Normalize(v4 A)
{
  GS_MATH_PROFILE_FUNC;
  r32 Magnitude = V4Mag(A);
  v4 Result = V4Scale(A, 1 / Magnitude);
  return Result;
}

internal r32 V2Dot(v2 A, v2 B) 
{
  GS_MATH_PROFILE_FUNC; 
  return ((A.x * B.x) + (A.y * B.y)); 
}
internal r32 V3Dot(v3 A, v3 B) 
{ 
  GS_MATH_PROFILE_FUNC;
  return ((A.x * B.x) + (A.y * B.y) + (A.z * B.z)); 
}
internal r32 V4Dot(v4 A, v4 B) 
{ 
  GS_MATH_PROFILE_FUNC;
  return ((A.x * B.x) + (A.y * B.y) + (A.z * B.z) + (A.w * B.w)); 
}

internal v2 V2PerpendicularCW(v2 A) 
{
  GS_MATH_PROFILE_FUNC;
  return (v2){A.y, -A.x}; 
}
internal v2 V2PerpendicularCCW(v2 A) 
{
  GS_MATH_PROFILE_FUNC;
  return (v2){A.y, A.x}; 
}

internal r32
V2Cross(v2 A, v2 B)
{
  GS_MATH_PROFILE_FUNC;
  return ((A.x * B.y) - (A.y * B.x));
}

internal v3
V3Cross(v3 A, v3 B)
{
  GS_MATH_PROFILE_FUNC;
  v3 Result = {
    (A.y * B.z) - (A.z * B.y),
    (A.z * B.x) - (A.x * B.z),
    (A.x * B.y) - (A.y * B.x)
  };
  return Result;
}

internal v4
V4Cross(v4 A, v4 B)
{
  GS_MATH_PROFILE_FUNC;
  v4 Result = {
    (A.y * B.z) - (A.z * B.y),
    (A.z * B.x) - (A.x * B.z),
    (A.x * B.y) - (A.y * B.x),
    0
  };
  return Result;
}

internal v2
V2Lerp(r32 T, v2 A, v2 B)
{
  GS_MATH_PROFILE_FUNC;
  v2 Result = (v2){
    LerpR32(T, A.x, B.x),
    LerpR32(T, A.y, B.y),
  };
  return Result;
}

internal v3
V3Lerp(r32 T, v3 A, v3 B)
{
  GS_MATH_PROFILE_FUNC;
  v3 Result = (v3){
    LerpR32(T, A.x, B.x),
    LerpR32(T, A.y, B.y),
    LerpR32(T, A.z, B.z),
  };
  return Result;
}

internal v4
V4Lerp(r32 T, v4 A, v4 B)
{
  GS_MATH_PROFILE_FUNC;
  v4 Result = (v4){
    LerpR32(T, A.x, B.x),
    LerpR32(T, A.y, B.y),
    LerpR32(T, A.z, B.z),
    LerpR32(T, A.w, B.w),
  };
  return Result;
}

internal v2
V2Remap(v2 P, v2 OldMin, v2 OldMax, v2 NewMin, v2 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  v2 Result = {0};
  Result.x = RemapR32(P.x, OldMin.x, OldMax.x, NewMin.x, NewMax.x);
  Result.y = RemapR32(P.y, OldMin.y, OldMax.y, NewMin.y, NewMax.y);
  return Result;
}

internal v3
V3Remap(v3 P, v3 OldMin, v3 OldMax, v3 NewMin, v3 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  v3 Result = {0};
  Result.x = RemapR32(P.x, OldMin.x, OldMax.x, NewMin.x, NewMax.x);
  Result.y = RemapR32(P.y, OldMin.y, OldMax.y, NewMin.y, NewMax.y);
  Result.z = RemapR32(P.z, OldMin.z, OldMax.z, NewMin.z, NewMax.z);
  return Result;
}

internal v4
V4Remap(v4 P, v4 OldMin, v4 OldMax, v4 NewMin, v4 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  v4 Result = {0};
  Result.x = RemapR32(P.x, OldMin.x, OldMax.x, NewMin.x, NewMax.x);
  Result.y = RemapR32(P.y, OldMin.y, OldMax.y, NewMin.y, NewMax.y);
  Result.z = RemapR32(P.z, OldMin.z, OldMax.z, NewMin.z, NewMax.z);
  Result.w = RemapR32(P.w, OldMin.w, OldMax.w, NewMin.w, NewMax.w);
  return Result;
}

internal v4
V4RemapAsV3(v4 P, v4 OldMin, v4 OldMax, v4 NewMin, v4 NewMax)
{
  GS_MATH_PROFILE_FUNC;
  v4 Result = {0};
  Result.xyz = V3Remap(P.xyz, OldMin.xyz, OldMax.xyz, NewMin.xyz, NewMax.xyz);
  Result.w = P.w;
  return Result;
}

//
// Matrix
//

typedef union m33
{
  struct
  {
    r32 AXx; r32 AYx; r32 AZx;
    r32 AXy; r32 AYy; r32 AZy;
    r32 AXz; r32 AYz; r32 AZz;
  };
  r32 E[9];
} m33;

typedef union m44
{
  struct
  {
    r32 AXx; r32 AYx; r32 AZx; r32 Tx;
    r32 AXy; r32 AYy; r32 AZy; r32 Ty;
    r32 AXz; r32 AYz; r32 AZz; r32 Tz;
    r32 AXw; r32 AYw; r32 AZw; r32 Tw;
  };
  r32 E[16];
} m44;


static m33
M33Empty ()
{
  GS_MATH_PROFILE_FUNC;
  m33 M = {
    0, 0, 0, 
    0, 0, 0,
    0, 0, 0
  };
  return M;
}

static m44
M44Empty()
{
  GS_MATH_PROFILE_FUNC;
  m44 M = {
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
  };
  return M;
}

static m33
M33Identity ()
{
  GS_MATH_PROFILE_FUNC;
  m33 M = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
  };
  return M;
}

static m44
M44Identity()
{
  GS_MATH_PROFILE_FUNC;
  m44 M = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
  };
  return M;
}

internal m44
M44RotationX(r32 Radians)
{
  GS_MATH_PROFILE_FUNC;
  r32 CosRad = CosR32(Radians);
  r32 SinRad = SinR32(Radians);
  m44 Result = M44Identity();
  Result.AYy = CosRad;
  Result.AZy = SinRad;
  Result.AYz = -SinRad;
  Result.AZz = CosRad;
  return Result;
}

internal m44
M44RotationY(r32 Radians)
{
  GS_MATH_PROFILE_FUNC;
  r32 CosRad = CosR32(Radians);
  r32 SinRad = SinR32(Radians);
  m44 Result = M44Identity();
  Result.AXx = CosRad;
  Result.AZx = SinRad;
  Result.AXz = -SinRad;
  Result.AZz = CosRad;
  return Result;
}

internal m44
M44RotationZ(r32 Radians)
{
  GS_MATH_PROFILE_FUNC;
  r32 CosRad = CosR32(Radians);
  r32 SinRad = SinR32(Radians);
  m44 Result = M44Identity();
  Result.AXx = CosRad;
  Result.AYx = -SinRad;
  Result.AXy = SinRad;
  Result.AYy = CosRad;
  return Result;
}

static m33
M33Transpose (m33 M)
{
  GS_MATH_PROFILE_FUNC;
  
  m33 Result = {};
  
  for (s32 x = 0; x < 3; x++)
  {
    for (s32 y = 0; y < 3; y++)
    {
      Result.E[x + (y * 3)] = M.E[y + (x * 3)];
    }
  }
  
  return Result;
}

static inline m44
M44Transpose (m44 M)
{
  GS_MATH_PROFILE_FUNC;
  
  m44 Result = {};
  
  Result.E[0] = M.E[0];
  Result.E[1] = M.E[4];
  Result.E[2] = M.E[8];
  Result.E[3] = M.E[12];
  
  Result.E[4] = M.E[1];
  Result.E[5] = M.E[5];
  Result.E[6] = M.E[9];
  Result.E[7] = M.E[13];
  
  Result.E[8] = M.E[2];
  Result.E[9] = M.E[6];
  Result.E[10] = M.E[10];
  Result.E[11] = M.E[14];
  
  Result.E[12] = M.E[3];
  Result.E[13] = M.E[7];
  Result.E[14] = M.E[11];
  Result.E[15] = M.E[15];
  
  return Result;
}

static inline bool
M33AreEqual(m33 A, m33 B)
{
  GS_MATH_PROFILE_FUNC;
  b32 Result = true;
  for (int e = 0; e < 9; e++) { if (Abs(A.E[e] - B.E[e]) > 0.0001f) { Result = false; break; } }
  return Result;
}

static inline bool
M44AreEqual(m44 A, m44 B)
{
  GS_MATH_PROFILE_FUNC;
  b32 Result = true;
  for (int e = 0; e < 16; e++) { if (Abs(A.E[e] - B.E[e]) > 0.0001f) { Result = false; break; } }
  return Result;
}

static inline m33
M33AddM33(m33 A, m33 B)
{
  GS_MATH_PROFILE_FUNC;
  m33 M = {};
  for (int e = 0; e < 9; e++) { M.E[e] = A.E[e] + B.E[e]; }
  return M;
}

static inline m44
M44AddM44(m44 A, m44 B)
{
  GS_MATH_PROFILE_FUNC;
  m44 M = {};
  for (int e = 0; e < 16; e++) { M.E[e] = A.E[e] + B.E[e]; }
  return M;
}

static inline m33
M33SubtractM33(m33 A, m33 B)
{
  GS_MATH_PROFILE_FUNC;
  m33 M = {};
  for (int e = 0; e < 9; e++) { M.E[e] = A.E[e] - B.E[e]; }
  return M;
}

static inline m44
M44SubtractM44(m44 A, m44 B)
{
  GS_MATH_PROFILE_FUNC;
  m44 M = {};
  for (int e = 0; e < 16; e++) { M.E[e] = A.E[e] - B.E[e]; }
  return M;
}

static inline m33
M33MultiplyM33(m33 A, m33 B)
{
  GS_MATH_PROFILE_FUNC;
  m33 M = {};
  for (int rx = 0; rx < 3; rx++)
  {
    for (int ry = 0; ry < 3; ry++)
    {
      int RIndex = (ry * 3) + rx;
      M.E[RIndex] = 0;
      for (int i = 0; i < 3; i++)
      {
        M.E[RIndex] += B.E[(ry * 3) + i] * A.E[(i * 3) + rx];
      }
    }
  }
  return M;
}

static inline m44
M44MultiplyM44(m44 A, m44 B)
{
  GS_MATH_PROFILE_FUNC;
  m44 M = {};
  
  float A00=A.E[0+4*0];
  float A01=A.E[0+4*1];
  float A02=A.E[0+4*2];
  float A03=A.E[0+4*3];
  
  float A10=A.E[1+4*0];
  float A11=A.E[1+4*1];
  float A12=A.E[1+4*2];
  float A13=A.E[1+4*3];
  
  float A20=A.E[2+4*0];
  float A21=A.E[2+4*1];
  float A22=A.E[2+4*2];
  float A23=A.E[2+4*3];
  
  float A30=A.E[3+4*0];
  float A31=A.E[3+4*1];
  float A32=A.E[3+4*2];
  float A33=A.E[3+4*3];
  
  float B00=B.E[0+4*0];
  float B01=B.E[0+4*1];
  float B02=B.E[0+4*2];
  float B03=B.E[0+4*3];
  
  float B10=B.E[1+4*0];
  float B11=B.E[1+4*1];
  float B12=B.E[1+4*2];
  float B13=B.E[1+4*3];
  
  float B20=B.E[2+4*0];
  float B21=B.E[2+4*1];
  float B22=B.E[2+4*2];
  float B23=B.E[2+4*3];
  
  float B30=B.E[3+4*0];
  float B31=B.E[3+4*1];
  float B32=B.E[3+4*2];
  float B33=B.E[3+4*3];
  
  M.E[0+4*0] = A00*B00+A10*B01+A20*B02+A30*B03;
  M.E[0+4*1] = A01*B00+A11*B01+A21*B02+A31*B03;
  M.E[0+4*2] = A02*B00+A12*B01+A22*B02+A32*B03;
  M.E[0+4*3] = A03*B00+A13*B01+A23*B02+A33*B03;
  
  M.E[1+4*0] = A00*B10+A10*B11+A20*B12+A30*B13;
  M.E[1+4*1] = A01*B10+A11*B11+A21*B12+A31*B13;
  M.E[1+4*2] = A02*B10+A12*B11+A22*B12+A32*B13;
  M.E[1+4*3] = A03*B10+A13*B11+A23*B12+A33*B13;
  
  M.E[2+4*0] = A00*B20+A10*B21+A20*B22+A30*B23;
  M.E[2+4*1] = A01*B20+A11*B21+A21*B22+A31*B23;
  M.E[2+4*2] = A02*B20+A12*B21+A22*B22+A32*B23;
  M.E[2+4*3] = A03*B20+A13*B21+A23*B22+A33*B23;
  
  M.E[3+4*0] = A00*B30+A10*B31+A20*B32+A30*B33;
  M.E[3+4*1] = A01*B30+A11*B31+A21*B32+A31*B33;
  M.E[3+4*2] = A02*B30+A12*B31+A22*B32+A32*B33;
  M.E[3+4*3] = A03*B30+A13*B31+A23*B32+A33*B33;
  
  return M;
}

static inline v3
M33MultiplyV3 (m33 M, v3 V)
{
  GS_MATH_PROFILE_FUNC;
  v3 Result = {};
  int i = 0;
  for (int y = 0; y < 3; y++)
  {
    Result.E[y] = 0;
    for (int x = 0; x < 3; x++)
    {
      Result.E[y] += M.E[(y * 3) + x] * V.E[x];
    }
  }
  return Result;
}


static inline v4
M44MultiplyV4(m44 M, v4 V)
{
  GS_MATH_PROFILE_FUNC;
  v4 Result = {};
  Result.x = (V.x * M.AXx) + (V.y * M.AYx) + (V.z * M.AZx) + (V.w * M.Tx);
  Result.y = (V.x * M.AXy) + (V.y * M.AYy) + (V.z * M.AZy) + (V.w * M.Ty);
  Result.z = (V.x * M.AXz) + (V.y * M.AYz) + (V.z * M.AZz) + (V.w * M.Tz);
  Result.w = (V.x * M.AXw) + (V.y * M.AYw) + (V.z * M.AZw) + (V.w * M.Tw);
  return Result;
}

#if __cplusplus

bool operator== (m33 A, m33 B) { return M33AreEqual(A, B); }
bool operator== (m44 A, m44 B) { return M44AreEqual(A, B); }
m33 operator+ (m33 A, m33 B) { return M33AddM33(A, B); }
m44 operator+ (m44 A, m44 B) { return M44AddM44(A, B); }
m33 operator- (m33 A, m33 B) { return M33SubtractM33(A, B); }
m44 operator- (m44 A, m44 B) { return M44SubtractM44(A, B); }
m33 operator* (m33 A, m33 B) { return M33MultiplyM33(A, B); }
m44 operator* (m44 A, m44 B) { return M44MultiplyM44(A, B); }

v3 operator* (m33 M, v3 V) { return M33MultiplyV3(M, V); }
v4 operator* (m44 M, v4 V) { return M44MultiplyV4(M, V); }

#endif // __cplusplus

static m44
M44Translation(v4 Delta)
{
  GS_MATH_PROFILE_FUNC;
  m44 Result = M44Identity();
  Result.Tx = Delta.x;
  Result.Ty = Delta.y;
  Result.Tz = Delta.z;
  return Result;
}

static m44
M44Rotation(v3 Delta)
{
  GS_MATH_PROFILE_FUNC;
  m44 Result = (m44){0};
  
  r32 CosX = CosR32(Delta.x);
  r32 SinX = SinR32(Delta.x);
  r32 CosY = CosR32(Delta.y);
  r32 SinY = SinR32(Delta.y);
  r32 CosZ = CosR32(Delta.z);
  r32 SinZ = SinR32(Delta.z);
  
  Result.AXx = CosY * CosZ;
  Result.AXy = -(SinX * SinY * CosZ) + (CosX * SinZ);
  Result.AXz = -(CosX * SinY * CosZ) - (SinX * SinZ);
  Result.AXw = 0;
  
  Result.AYx = -(SinZ * CosY);
  Result.AYy = (SinX * SinY * SinZ) + (CosX * CosZ);
  Result.AYz = (CosX * SinY * SinZ) - (SinX * CosZ);
  Result.AYw = 0;
  
  Result.AZx = SinY;
  Result.AZy = SinX * CosY;
  Result.AZz = CosX * CosY;
  Result.AZw = 0;
  
  Result.Tx = 0;
  Result.Ty = 0;
  Result.Tz = 0;
  Result.Tw = 1;
  
  return Result;
}

static m44
M44ScaleUniform(r32 Scale)
{
  m44 Result = (m44){
    Scale, 0, 0, 0,
    0, Scale, 0, 0,
    0, 0, Scale, 0,
    0, 0, 0, 1
  };
  return Result;
}

static m44
M44FromPosition (v4 Position)
{
  GS_MATH_PROFILE_FUNC;
  return (m44){
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    Position.x, Position.y, Position.z, Position.w
  };
}

internal m44
M44CoordinateFrame(v4 Forward, v4 Right, v4 Up)
{
  GS_MATH_PROFILE_FUNC;
  m44 Result = {0};
  Result.AXx = Right.x;
  Result.AYx = Right.y;
  Result.AZx = Right.z;
  Result.Tx = Right.w;
  
  Result.AXy = Up.x;
  Result.AYy = Up.y;
  Result.AZy = Up.z;
  Result.Ty = Up.w;
  
  Result.AXz = Forward.x;
  Result.AYz = Forward.y;
  Result.AZz = Forward.z;
  Result.Tz = Forward.w;
  
  Result.Tw = 1.0f;
  return Result;
}

static m44
M44Model (v4 Forward, v4 Right, v4 Up, v4 Position)
{
  GS_MATH_PROFILE_FUNC;
  m44 RotationMatrix = M44CoordinateFrame(Forward, Right, Up);
  m44 PositionMatrix = M44Translation(V4Negate(Position));
  m44 ModelViewMatrix = M44MultiplyM44(PositionMatrix, RotationMatrix);
  return ModelViewMatrix;
}

static m44
M44ModelView (v4 Forward, v4 Right, v4 Position)
{
  GS_MATH_PROFILE_FUNC;
  v4 Up = V4Normalize(V4Cross(Forward, Right));
  return M44Model(Forward, Right, Up, Position);
}

static m44
M44LookAt (v4 Position, v4 Target)
{
  GS_MATH_PROFILE_FUNC;
  // NOTE(Peter): the camera usually points along the -z axis, hence
  // Forward = a ray that points from the target back towards your position
  v4 Forward = V4Normalize(V4Sub(Position, Target));
  v4 Right = V4Normalize(V4Cross((v4){0, 1, 0, 0}, Forward));
  v4 Up = V4Normalize(V4Cross(Forward, Right));
  m44 Result = M44CoordinateFrame(Forward, Right, Up);
  return Result;
}

static b32 
M44Inverse(m44 M_In, m44* M_Out)
{
  GS_MATH_PROFILE_FUNC;
  b32 Result = false;
  
  float det;
  s32 i;
  
  
  M_Out->E[0] = M_In.E[5]  * M_In.E[10] * M_In.E[15] -
    M_In.E[5]  * M_In.E[11] * M_In.E[14] -
    M_In.E[9]  * M_In.E[6]  * M_In.E[15] +
    M_In.E[9]  * M_In.E[7]  * M_In.E[14] +
    M_In.E[13] * M_In.E[6]  * M_In.E[11] -
    M_In.E[13] * M_In.E[7]  * M_In.E[10];
  
  M_Out->E[4] = -M_In.E[4]  * M_In.E[10] * M_In.E[15] +
    M_In.E[4]  * M_In.E[11] * M_In.E[14] +
    M_In.E[8]  * M_In.E[6]  * M_In.E[15] -
    M_In.E[8]  * M_In.E[7]  * M_In.E[14] -
    M_In.E[12] * M_In.E[6]  * M_In.E[11] +
    M_In.E[12] * M_In.E[7]  * M_In.E[10];
  
  M_Out->E[8] = M_In.E[4]  * M_In.E[9] * M_In.E[15] -
    M_In.E[4]  * M_In.E[11] * M_In.E[13] -
    M_In.E[8]  * M_In.E[5] * M_In.E[15] +
    M_In.E[8]  * M_In.E[7] * M_In.E[13] +
    M_In.E[12] * M_In.E[5] * M_In.E[11] -
    M_In.E[12] * M_In.E[7] * M_In.E[9];
  
  M_Out->E[12] = -M_In.E[4]  * M_In.E[9] * M_In.E[14] +
    M_In.E[4]  * M_In.E[10] * M_In.E[13] +
    M_In.E[8]  * M_In.E[5] * M_In.E[14] -
    M_In.E[8]  * M_In.E[6] * M_In.E[13] -
    M_In.E[12] * M_In.E[5] * M_In.E[10] +
    M_In.E[12] * M_In.E[6] * M_In.E[9];
  
  M_Out->E[1] = -M_In.E[1]  * M_In.E[10] * M_In.E[15] +
    M_In.E[1]  * M_In.E[11] * M_In.E[14] +
    M_In.E[9]  * M_In.E[2] * M_In.E[15] -
    M_In.E[9]  * M_In.E[3] * M_In.E[14] -
    M_In.E[13] * M_In.E[2] * M_In.E[11] +
    M_In.E[13] * M_In.E[3] * M_In.E[10];
  
  M_Out->E[5] = M_In.E[0]  * M_In.E[10] * M_In.E[15] -
    M_In.E[0]  * M_In.E[11] * M_In.E[14] -
    M_In.E[8]  * M_In.E[2] * M_In.E[15] +
    M_In.E[8]  * M_In.E[3] * M_In.E[14] +
    M_In.E[12] * M_In.E[2] * M_In.E[11] -
    M_In.E[12] * M_In.E[3] * M_In.E[10];
  
  M_Out->E[9] = -M_In.E[0]  * M_In.E[9] * M_In.E[15] +
    M_In.E[0]  * M_In.E[11] * M_In.E[13] +
    M_In.E[8]  * M_In.E[1] * M_In.E[15] -
    M_In.E[8]  * M_In.E[3] * M_In.E[13] -
    M_In.E[12] * M_In.E[1] * M_In.E[11] +
    M_In.E[12] * M_In.E[3] * M_In.E[9];
  
  M_Out->E[13] = M_In.E[0]  * M_In.E[9] * M_In.E[14] -
    M_In.E[0]  * M_In.E[10] * M_In.E[13] -
    M_In.E[8]  * M_In.E[1] * M_In.E[14] +
    M_In.E[8]  * M_In.E[2] * M_In.E[13] +
    M_In.E[12] * M_In.E[1] * M_In.E[10] -
    M_In.E[12] * M_In.E[2] * M_In.E[9];
  
  M_Out->E[2] = M_In.E[1]  * M_In.E[6] * M_In.E[15] -
    M_In.E[1]  * M_In.E[7] * M_In.E[14] -
    M_In.E[5]  * M_In.E[2] * M_In.E[15] +
    M_In.E[5]  * M_In.E[3] * M_In.E[14] +
    M_In.E[13] * M_In.E[2] * M_In.E[7] -
    M_In.E[13] * M_In.E[3] * M_In.E[6];
  
  M_Out->E[6] = -M_In.E[0]  * M_In.E[6] * M_In.E[15] +
    M_In.E[0]  * M_In.E[7] * M_In.E[14] +
    M_In.E[4]  * M_In.E[2] * M_In.E[15] -
    M_In.E[4]  * M_In.E[3] * M_In.E[14] -
    M_In.E[12] * M_In.E[2] * M_In.E[7] +
    M_In.E[12] * M_In.E[3] * M_In.E[6];
  
  M_Out->E[10] = M_In.E[0]  * M_In.E[5] * M_In.E[15] -
    M_In.E[0]  * M_In.E[7] * M_In.E[13] -
    M_In.E[4]  * M_In.E[1] * M_In.E[15] +
    M_In.E[4]  * M_In.E[3] * M_In.E[13] +
    M_In.E[12] * M_In.E[1] * M_In.E[7] -
    M_In.E[12] * M_In.E[3] * M_In.E[5];
  
  M_Out->E[14] = -M_In.E[0]  * M_In.E[5] * M_In.E[14] +
    M_In.E[0]  * M_In.E[6] * M_In.E[13] +
    M_In.E[4]  * M_In.E[1] * M_In.E[14] -
    M_In.E[4]  * M_In.E[2] * M_In.E[13] -
    M_In.E[12] * M_In.E[1] * M_In.E[6] +
    M_In.E[12] * M_In.E[2] * M_In.E[5];
  
  M_Out->E[3] = -M_In.E[1] * M_In.E[6] * M_In.E[11] +
    M_In.E[1] * M_In.E[7] * M_In.E[10] +
    M_In.E[5] * M_In.E[2] * M_In.E[11] -
    M_In.E[5] * M_In.E[3] * M_In.E[10] -
    M_In.E[9] * M_In.E[2] * M_In.E[7] +
    M_In.E[9] * M_In.E[3] * M_In.E[6];
  
  M_Out->E[7] = M_In.E[0] * M_In.E[6] * M_In.E[11] -
    M_In.E[0] * M_In.E[7] * M_In.E[10] -
    M_In.E[4] * M_In.E[2] * M_In.E[11] +
    M_In.E[4] * M_In.E[3] * M_In.E[10] +
    M_In.E[8] * M_In.E[2] * M_In.E[7] -
    M_In.E[8] * M_In.E[3] * M_In.E[6];
  
  M_Out->E[11] = -M_In.E[0] * M_In.E[5] * M_In.E[11] +
    M_In.E[0] * M_In.E[7] * M_In.E[9] +
    M_In.E[4] * M_In.E[1] * M_In.E[11] -
    M_In.E[4] * M_In.E[3] * M_In.E[9] -
    M_In.E[8] * M_In.E[1] * M_In.E[7] +
    M_In.E[8] * M_In.E[3] * M_In.E[5];
  
  M_Out->E[15] = M_In.E[0] * M_In.E[5] * M_In.E[10] -
    M_In.E[0] * M_In.E[6] * M_In.E[9] -
    M_In.E[4] * M_In.E[1] * M_In.E[10] +
    M_In.E[4] * M_In.E[2] * M_In.E[9] +
    M_In.E[8] * M_In.E[1] * M_In.E[6] -
    M_In.E[8] * M_In.E[2] * M_In.E[5];
  
  det = M_In.E[0] * M_Out->E[0] + M_In.E[1] * M_Out->E[4] + M_In.E[2] * M_Out->E[8] + M_In.E[3] * M_Out->E[12];
  
  if (det == 0)
  {
    return false;
  }
  
  det = 1.0 / det;
  
  for (i = 0; i < 16; i++)
  {
    M_Out->E[i] = M_Out->E[i] * det;
  }
  
  return true;
}

internal m44
M44ProjectionOrtho (r32 Width, r32 Height, r32 Near, r32 Far, r32 Right, r32 Left, r32 Top, r32 Bottom)
{
  GS_MATH_PROFILE_FUNC;
  m44 Result = (m44){0};
  Result.AXx = 2.0f / Width;
  Result.AYy = 2.0f / Height;
  Result.AZz = 2.0f / (Near - Far);
  Result.Tx = -(Right + Left) / (Right - Left);
  Result.Ty = -(Top + Bottom) / (Top - Bottom);
  Result.Tz = -(Far + Near) / (Far - Near);
  Result.Tw = 1;
  return Result;
}

internal m44
M44ProjectionOrthoAspect(r32 Aspect, r32 Scale, r32 Near, r32 Far)
{
  GS_MATH_PROFILE_FUNC;
  m44 Result = (m44){0};
  r32 Width = Scale * Aspect;
  r32 Height = Scale;
  r32 Right = Width / 2.0f;
  r32 Left = -Right;
  r32 Top = Height / 2.0f;
  r32 Bottom = -Top;
  Result = M44ProjectionOrtho(Width, Height, Near, Far, Right, Left, Top, Bottom);
  return Result;
}

internal m44
M44ProjectionOrthoScreen(r32 Width, r32 Height, r32 Near, r32 Far)
{
  GS_MATH_PROFILE_FUNC;
  r32 Right = Width;
  r32 Left = 0;
  r32 Top = Height;
  r32 Bottom = 0;
  m44 Result = M44ProjectionOrtho(Width, Height, Near, Far, Right, Left, Top, Bottom);
  return Result;
}

internal m44
M44ProjectionPerspective(r32 FieldOfViewDegrees, r32 AspectRatio, r32 Near, r32 Far)
{
  GS_MATH_PROFILE_FUNC;
  m44 Result = M44Identity();
  
  // The perspective divide step involves dividing x and y by -z
  // Making Tz = -1 will make Tw of the result = -z
  Result.Tw = 0;
  Result.AZw = -1;
  
  // Remap z' from the range [near clip : far clip] to [0 : 1]
  r32 ViewRange = Far - Near;
  Result.AZz = -((Far + Near) / ViewRange);
  Result.Tz = -(2 * Near * Far) / ViewRange;
  
  // Adjust for field of view - adjust the x' and y coordinates based
  // on how
  r32 FovBasedScale = TanR32(DegToRadR32(FieldOfViewDegrees / 2));
  r32 Top = Near * FovBasedScale;
  r32 Bottom = -Top;
  r32 Right = Top * AspectRatio;
  r32 Left = -Right;
  Result.AXx = (2 * Near) / (Right - Left);
  Result.AZx = (Right + Left) / (Right - Left);
  Result.AYy = (2 * Near) / (Top - Bottom);
  Result.AZy = (Top + Bottom) / (Top - Bottom);
  
  return Result;
}


#endif //GS_MATH_H
