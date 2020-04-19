//
// File: gs_types.cpp
// Author: Peter Slattery
// Creation Date: 2020-04-18
//
#ifndef GS_TYPES_CPP

internal u64
RoundUpTo64(u64 Value, u64 Alignment)
{
    Value += Alignment - 1;
    Value = Value % Alignment;
    return Value;
}

internal u8
PowU8(u8 X, u32 Power)
{
    u8 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal u16
PowU16(u16 X, u32 Power)
{
    u16 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal u32
PowU32(u32 X, u32 Power)
{
    u32 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal u64
PowU64(u64 X, u32 Power)
{
    u64 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal s8
PowS8(s8 X, u32 Power)
{
    s8 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal s16
PowS16(s16 X, u32 Power)
{
    s16 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal s32
PowS32(s32 X, u32 Power)
{
    s32 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal s64
PowS64(s64 X, u32 Power)
{
    s64 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal r32
PowR32(r32 X, u32 Power)
{
    r32 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}
internal r64
PowR64(r64 X, u32 Power)
{
    r64 Result = X;
    for (u32 i = 1; i < Power; i++) { Result *= X; }
    return Result;
}

internal u8
LerpU8(r32 T, u8 A, u8 B)
{
    return (u8)((A * (1.0f - T)) + (B * T));
}
internal u16
LerpU16(r32 T, u16 A, u16 B)
{
    return (u16)((A * (1.0f - T)) + (B * T));
}
internal u32
LerpU32(r32 T, u32 A, u32 B)
{
    return (u32)((A * (1.0f - T)) + (B * T));
}
internal u64
LerpU64(r32 T, u64 A, u64 B)
{
    return (u64)((A * (1.0f - T)) + (B * T));
}
internal s8
LerpS8(r32 T, s8 A, s8 B)
{
    return (s8)((A * (1.0f - T)) + (B * T));
}
internal s16
LerpS16(r32 T, s16 A, s16 B)
{
    return (s16)((A * (1.0f - T)) + (B * T));
}
internal s32
LerpS32(r32 T, s32 A, s32 B)
{
    return (s32)((A * (1.0f - T)) + (B * T));
}
internal s64
LerpS64(r32 T, s64 A, s64 B)
{
    return (s64)((A * (1.0f - T)) + (B * T));
}
internal r32
LerpR32(r32 T, r32 A, r32 B)
{
    return (r32)((A * (1.0f - T)) + (B * T));
}
internal r64
LerpR64(r32 T, r64 A, r64 B)
{
    return (r64)((A * (1.0f - T)) + (B * T));
}

internal u8
UnlerpU8(u8 Value, u8 Min, u8 Max)
{
    return (u8)((r64)(Value - Min) / (r64)(Max - Min));
}
internal u16
UnlerpU16(u16 Value, u16 Min, u16 Max)
{
    return (u16)((r64)(Value - Min) / (r64)(Max - Min));
}
internal u32
UnlerpU32(u32 Value, u32 Min, u32 Max)
{
    return (u32)((r64)(Value - Min) / (r64)(Max - Min));
}
internal u64
UnlerpU64(u64 Value, u64 Min, u64 Max)
{
    return (u64)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s8
UnlerpS8(s8 Value, s8 Min, s8 Max)
{
    return (s8)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s16
UnlerpS16(s16 Value, s16 Min, s16 Max)
{
    return (s16)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s32
UnlerpS32(s32 Value, s32 Min, s32 Max)
{
    return (s32)((r64)(Value - Min) / (r64)(Max - Min));
}
internal s64
UnlerpS64(s64 Value, s64 Min, s64 Max)
{
    return (s64)((r64)(Value - Min) / (r64)(Max - Min));
}
internal r32
UnlerpR32(r32 Value, r32 Min, r32 Max)
{
    return (Value - Min) / (Max - Min);
}
internal r64
UnlerpR64(r64 Value, r64 Min, r64 Max)
{
    return (Value - Min) / (Max - Min);
}


internal u8
RemapU8(u8 Value, u8 OldMin, u8 OldMax, u8 NewMin, u8 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    u8 Result = (u8)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal u16
RemapU16(u16 Value, u16 OldMin, u16 OldMax, u16 NewMin, u16 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    u16 Result = (u16)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal u32
RemapU32(u32 Value, u32 OldMin, u32 OldMax, u32 NewMin, u32 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    u32 Result = (u32)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal u64
RemapU64(u64 Value, u64 OldMin, u64 OldMax, u64 NewMin, u64 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    u64 Result = (u64)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal s8
RemapS8(s8 Value, s8 OldMin, s8 OldMax, s8 NewMin, s8 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    s8 Result = (s8)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal s16
RemapS16(s16 Value, s16 OldMin, s16 OldMax, s16 NewMin, s16 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    s16 Result = (s16)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal s32
RemapS32(s32 Value, s32 OldMin, s32 OldMax, s32 NewMin, s32 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    s32 Result = (s32)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal s64
RemapS64(s64 Value, s64 OldMin, s64 OldMax, s64 NewMin, s64 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    s64 Result = (s64)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal r32
RemapR32(r32 Value, r32 OldMin, r32 OldMax, r32 NewMin, r32 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    r32 Result = (r32)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}
internal r64
RemapR64(r64 Value, r64 OldMin, r64 OldMax, r64 NewMin, r64 NewMax)
{
    r64 A = (r64)(Value - OldMin) / (r64)(OldMax - OldMin);
    r64 Result = (r64)((A * (NewMax - NewMin)) + NewMin);
    return Result;
}

internal r32
FloorR32(r32 V)
{
    return (r32)((s64)V);
}
internal r64
FloorR64(r64 V)
{
    return (r64)((s64)V);
}

internal r32
FractR32(r32 V)
{
    return V - FloorR32(V);
}
internal r64
FractR64(r64 V)
{
    return V - FloorR64(V);
}

#include <intrin.h>
internal r32
SqrtR32(r32 V)
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(V)));
}

#include <math.h>
internal r32
SinR32(r32 Rad)
{
    return sinf(Rad);
}
internal r64
SinR64(r64 Rad)
{
    return sin(Rad);
}

internal r32
CosR32(r32 Rad)
{
    return cosf(Rad);
}
internal r64
CosR64(r64 Rad)
{
    return cos(Rad);
}

internal r32
TanR32(r32 Rad)
{
    return tanf(Rad);
}
internal r64
TanR64(r64 Rad)
{
    return tan(Rad);
}

///////////////////////////
//
// Vector

v2 operator- (v2 A) { return { -A.X, -A.Y }; }
v3 operator- (v3 A) { return { -A.X, -A.Y, -A.Z }; }
v4 operator- (v4 A) { return { -A.X, -A.Y, -A.Z, -A.W }; }

v2 operator+ (v2 A, v2 B) { return { A.X + B.X, A.Y + B.Y }; }
v3 operator+ (v3 A, v3 B) { return { A.X + B.X, A.Y + B.Y, A.Z + B.Z }; }
v4 operator+ (v4 A, v4 B) { return { A.X + B.X, A.Y + B.Y, A.Z + B.Z, A.W + B.W }; }

v2 operator- (v2 A, v2 B) { return { A.X - B.X, A.Y - B.Y }; }
v3 operator- (v3 A, v3 B) { return { A.X - B.X, A.Y - B.Y, A.Z - B.Z }; }
v4 operator- (v4 A, v4 B) { return { A.X - B.X, A.Y - B.Y, A.Z - B.Z, A.W - B.W }; }

v2 operator+= (v2& A, v2 B) { return { A.X + B.X, A.Y + B.Y }; }
v3 operator+= (v3& A, v3 B) { return { A.X + B.X, A.Y + B.Y, A.Z + B.Z }; }
v4 operator+= (v4& A, v4 B) { return { A.X + B.X, A.Y + B.Y, A.Z + B.Z, A.W + B.W }; }

v2 operator-= (v2& A, v2 B) { return { A.X - B.X, A.Y - B.Y }; }
v3 operator-= (v3& A, v3 B) { return { A.X - B.X, A.Y - B.Y, A.Z - B.Z }; }
v4 operator-= (v4& A, v4 B) { return { A.X - B.X, A.Y - B.Y, A.Z - B.Z, A.W - B.W }; }

v2 operator* (v2 A, r32 B) { return { A.X * B, A.Y * B }; }
v3 operator* (v3 A, r32 B) { return { A.X * B, A.Y * B, A.Z * B }; }
v4 operator* (v4 A, r32 B) { return { A.X * B, A.Y * B, A.Z * B, A.W * B }; }

v2 operator/ (v2 A, r32 B) { return { A.X / B, A.Y / B }; }
v3 operator/ (v3 A, r32 B) { return { A.X / B, A.Y / B, A.Z / B }; }
v4 operator/ (v4 A, r32 B) { return { A.X / B, A.Y / B, A.Z / B, A.W / B }; }

v2 operator*= (v2& A, r32 B) { return { A.X * B, A.Y * B }; }
v3 operator*= (v3& A, r32 B) { return { A.X * B, A.Y * B, A.Z * B }; }
v4 operator*= (v4& A, r32 B) { return { A.X * B, A.Y * B, A.Z * B, A.W * B }; }

v2 operator/= (v2& A, r32 B) { return { A.X / B, A.Y / B }; }
v3 operator/= (v3& A, r32 B) { return { A.X / B, A.Y / B, A.Z / B }; }
v4 operator/= (v4& A, r32 B) { return { A.X / B, A.Y / B, A.Z / B, A.W / B }; }

bool operator == (v2 A, v2 B) { return ((A.X == B.X) && (A.Y == B.Y)); }
bool operator == (v3 A, v3 B) { return ((A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z)); }
bool operator == (v4 A, v4 B) { return ((A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z) && (A.W == B.W)); }

bool operator != (v2 A, v2 B) { return !((A.X == B.X) && (A.Y == B.Y)); }
bool operator != (v3 A, v3 B) { return !((A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z)); }
bool operator != (v4 A, v4 B) { return !((A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z) && (A.W == B.W)); }

internal v3 ToV3(v2 V, r32 Z = 0) { return v3{V.X, V.Y, Z}; }
internal v4 ToV4(v2 V, r32 Z = 0, r32 W = 0) { return v4{V.X, V.Y, Z, W}; }
internal v4 ToV4(v3 V, r32 W = 0) { return v4{V.X, V.Y, V.Z, W}; }

internal r32 V2MagSquared(v2 V) { return ((V.X * V.X) + (V.Y * V.Y)); }
internal r32 V3MagSquared(v3 V) { return ((V.X * V.X) + (V.Y * V.Y) + (V.Z * V.Z)); }
internal r32 V4MagSquared(v4 V) { return ((V.X * V.X) + (V.Y * V.Y) + (V.Z * V.Z) + (V.W * V.W)); }

internal r32 V2Mag(v2 V) { return SqrtR32((V.X * V.X) + (V.Y * V.Y)); }
internal r32 V3Mag(v3 V) { return SqrtR32((V.X * V.X) + (V.Y * V.Y) + (V.Z * V.Z)); }
internal r32 V4Mag(v4 V) { return SqrtR32((V.X * V.X) + (V.Y * V.Y) + (V.Z * V.Z) + (V.W * V.W)); }

internal r32 V2DistanceSquared(v2 A, v2 B) { return V2MagSquared(A - B); }
internal r32 V3DistanceSquared(v3 A, v3 B) { return V3MagSquared(A - B); }
internal r32 V4DistanceSquared(v4 A, v4 B) { return V4MagSquared(A - B); }

internal r32 V2Distance(v2 A, v2 B) { return V2Mag(A - B); }
internal r32 V3Distance(v3 A, v3 B) { return V3Mag(A - B); }
internal r32 V4Distance(v4 A, v4 B) { return V4Mag(A - B); }

internal v2
V2Normalize(v2 A)
{
    r32 Magnitude = V2Mag(A);
    return A / Magnitude;
}
internal v3
V3Normalize(v3 A)
{
    r32 Magnitude = V3Mag(A);
    return A / Magnitude;
}
internal v4
V4Normalize(v4 A)
{
    r32 Magnitude = V4Mag(A);
    return A / Magnitude;
}

internal r32 V2Dot(v2 A, v2 B) { return ((A.X * B.X) + (A.Y * B.Y)); }
internal r32 V3Dot(v3 A, v3 B) { return ((A.X * B.X) + (A.Y * B.Y) + (A.Z * B.Z)); }
internal r32 V3Dot(v4 A, v4 B) { return ((A.X * B.X) + (A.Y * B.Y) + (A.Z * B.Z) + (A.W * B.W)); }

internal v2 V2PerpendicularCW(v2 A) { return v2{A.Y, -A.X}; }
internal v2 V2PerpendicularCCW(v2 A) { return v2{A.Y, A.X}; }

internal r32 V2Cross(v2 A, v2 B) { return ((A.X * B.Y) - (A.Y * B.X)); }
internal v3
V3Cross(v3 A, v3 B)
{
    v3 Result = {
        (A.Y * B.Z) - (A.Z * B.Y),
        (A.Z * B.X) - (A.X * B.Z),
        (A.X * B.Y) - (A.Y * B.X)
    };
    return Result;
}
internal v4
V4Cross(v4 A, v4 B)
{
    v4 Result = {
        (A.Y * B.Z) - (A.Z * B.Y),
        (A.Z * B.X) - (A.X * B.Z),
        (A.X * B.Y) - (A.Y * B.X),
        0
    };
    return Result;
}

internal v2
V2Lerp(v2 A, v2 B, r32 T)
{
    v2 Result = v2{
        LerpR32(A.X, B.X, T),
        LerpR32(A.Y, B.Y, T),
    };
    return Result;
}
internal v3
V3Lerp(v3 A, v3 B, r32 T)
{
    v3 Result = v3{
        LerpR32(A.X, B.X, T),
        LerpR32(A.Y, B.Y, T),
        LerpR32(A.Z, B.Z, T),
    };
    return Result;
}
internal v4
V4Lerp(v4 A, v4 B, r32 T)
{
    v4 Result = v4{
        LerpR32(A.X, B.X, T),
        LerpR32(A.Y, B.Y, T),
        LerpR32(A.Z, B.Z, T),
        LerpR32(A.W, B.W, T),
    };
    return Result;
}

///////////////////////////
//
// Ranges

internal b32 ValueInRangeR32(r32 Min, r32 Max, r32 V)
{
    return ((V >= Min) && (V <= Max));
}

internal b32 ValueInRange1(range1 Range, r32 V)
{
    return ValueInRangeR32(Range.Min, Range.Max, V);
}
internal b32 ValueInRange2(range2 Range, v2 V)
{
    return (ValueInRangeR32(Range.Min.X, Range.Max.X, V.X) &&
            ValueInRangeR32(Range.Min.Y, Range.Max.Y, V.Y));
}
internal b32 ValueInRange3(range3 Range, v3 V)
{
    return (ValueInRangeR32(Range.Min.X, Range.Max.X, V.X) &&
            ValueInRangeR32(Range.Min.Y, Range.Max.Y, V.Y) &&
            ValueInRangeR32(Range.Min.Z, Range.Max.Z, V.Z));
}
internal b32 ValueInRange4(range4 Range, v4 V)
{
    return (ValueInRangeR32(Range.Min.X, Range.Max.X, V.X) &&
            ValueInRangeR32(Range.Min.Y, Range.Max.Y, V.Y) &&
            ValueInRangeR32(Range.Min.Z, Range.Max.Z, V.Z) &&
            ValueInRangeR32(Range.Min.W, Range.Max.W, V.W));
}

#define PointIsInRect(range, point) ValueInRange2((range), (point))

internal r32 Range1SizeX(range1 Range) { return Range.Max - Range.Min; }
internal r32 Range2SizeX(range2 Range) { return Range.Max.X - Range.Min.X; }
internal r32 Range3SizeX(range3 Range) { return Range.Max.X - Range.Min.X; }
internal r32 Range4SizeX(range4 Range) { return Range.Max.X - Range.Min.X; }

internal r32 Range2SizeY(range2 Range) { return Range.Max.Y - Range.Min.Y; }
internal r32 Range3SizeY(range3 Range) { return Range.Max.Y - Range.Min.Y; }
internal r32 Range4SizeY(range4 Range) { return Range.Max.Y - Range.Min.Y; }

internal r32 Range3SizeZ(range3 Range) { return Range.Max.Z - Range.Min.Z; }
internal r32 Range4SizeZ(range4 Range) { return Range.Max.Z - Range.Min.Z; }

internal r32 Range4SizeW(range4 Range) { return Range.Max.W - Range.Min.W; }

internal r32 Range1Center(range1 Range) { return (Range.Max + Range.Min) / 2.0f; }
internal v2  Range2Center(range2 Range) { return (Range.Max + Range.Min) / 2.0f; }
internal v3  Range3Center(range3 Range) { return (Range.Max + Range.Min) / 2.0f; }
internal v4  Range4Center(range4 Range) { return (Range.Max + Range.Min) / 2.0f; }

internal range1 Range1Offset(range1 Range, r32 Delta) { return range1{ Range.Min + Delta, Range.Max + Delta }; }
internal range2 Range2Offset(range2 Range, v2 Delta) { return range2{ Range.Min + Delta, Range.Max + Delta }; }
internal range3 Range3Offset(range3 Range, v3 Delta) { return range3{ Range.Min + Delta, Range.Max + Delta }; }
internal range4 Range4Offset(range4 Range, v4 Delta) { return range4{ Range.Min + Delta, Range.Max + Delta }; }

internal v2 RectTopLeft(rect2 Rect)
{
    return v2{ Rect.Min.X, Rect.Max.Y };
}
internal v2 RectTopRight(rect2 Rect)
{
    return Rect.Max;
}
internal v2 RectBottomLeft(rect2 Rect)
{
    return Rect.Min;
}
internal v2 RectBottomRight(rect2 Rect)
{
    return v2{ Rect.Max.X, Rect.Min.Y };
}

internal r32 AspectRaio(r32 Width, r32 Height) { return Width / Height; }
internal r32 RectAspectRatio(rect2 Rect) { return Range2SizeX(Rect) / Range2SizeY(Rect); }

internal void
RectHSplit(rect2 Rect, r32 YValue, rect2* Top, rect2* Bottom)
{
    r32 ClampedYValue = Clamp(Rect.Min.Y, YValue, Rect.Max.Y);
    Top->Max = Rect.Max;
    Top->Min = { Rect.Min.X, ClampedYValue };
    Bottom->Max = { Rect.Max.X, ClampedYValue };
    Bottom->Min = Rect.Min;
}
internal void
RectVSplit(rect2 Rect, r32 XValue, rect2* Left, rect2* Right)
{
    r32 ClampedXValue = Clamp(Rect.Min.X, XValue, Rect.Max.X);
    Left->Max = { ClampedXValue, Rect.Max.Y};
    Left->Min = Rect.Min;
    Right->Max = Rect.Max;
    Right->Min = { ClampedXValue, Rect.Min.Y };
}

internal void
RectHSplitAtDistanceFromTop(rect2 Rect, r32 YDist, rect2* Top, rect2* Bottom)
{
    RectHSplit(Rect, Rect.Max.Y - YDist, Top, Bottom);
}
internal void
RectHSplitAtDistanceFromBottom(rect2 Rect, r32 YDist, rect2* Top, rect2* Bottom)
{
    RectHSplit(Rect, Rect.Min.Y + YDist, Top, Bottom);
}
internal void
RectVSplitAtDistanceFromRight(rect2 Rect, r32 XDist, rect2* Left, rect2* Right)
{
    RectHSplit(Rect, Rect.Max.X - XDist, Left, Right);
}
internal void
RectVSplitAtDistanceFromLeft(rect2 Rect, r32 XDist, rect2* Left, rect2* Right)
{
    RectHSplit(Rect, Rect.Min.X + XDist, Left, Right);
}
internal void
RectHSplitAtPercent(rect2 Rect, r32 YPercent, rect2* Top, rect2* Bottom)
{
    RectHSplit(Rect, UnlerpR32(Rect.Min.Y, YPercent, Rect.Max.Y), Top, Bottom);
}
internal void
RectVSplitAtPercent(rect2 Rect, r32 XPercent, rect2* Left, rect2* Right)
{
    RectHSplit(Rect, UnlerpR32(Rect.Min.X, XPercent, Rect.Max.X), Left, Right);
}

internal rect2
RectInset(rect2 Outer, v2 Amount)
{
    rect2 Result = { Outer.Min + Amount, Outer.Max - Amount };
    return Result;
}
internal rect2
RectInset(rect2 Outer, r32 UniformAmount)
{
    return RectInset(Outer, v2{UniformAmount, UniformAmount});
}

///////////////////////////
//
// Matrices

#define M33Pos(M, X, Y) (M).E[((Y) * 3) + (X)]
#define M44Pos(M, X, Y) (M).E[((Y) * 4) + (X)]

internal m33
M33Identity()
{
    m33 M = {0};
    M33Pos(M, 0, 0) = 1.0f;
    M33Pos(M, 1, 1) = 1.0f;
    M33Pos(M, 2, 2) = 1.0f;
    return M;
}
internal m44
M44Identity()
{
    m44 M = {0};
    M44Pos(M, 0, 0) = 1.0f;
    M44Pos(M, 1, 1) = 1.0f;
    M44Pos(M, 2, 2) = 1.0f;
    M44Pos(M, 3, 3) = 1.0f;
    return M;
}

m33 operator* (m33 A, m33 B)
{
    m33 M = {0};
    for (int Y = 0; Y < 3; Y++)
    {
        for (int X = 0; X < 3; X++)
        {
            r32 Acc = 0;
            for (int K = 0; K < 3; K++)
            {
                Acc += M33Pos(A, X, K) * M33Pos(B, K, Y);
            }
            M33Pos(M, X, Y) = Acc;
        }
    }
    return M;
}
m44 operator* (m44 A, m44 B)
{
    m44 M = {0};
    for (int Y = 0; Y < 4; Y++)
    {
        for (int X = 0; X < 4; X++)
        {
            r32 Acc = 0;
            for (int K = 0; K < 4; K++)
            {
                Acc += M44Pos(A, X, K) * M44Pos(B, K, Y);
            }
            M44Pos(M, X, Y) = Acc;
        }
    }
    return M;
}

m33 operator* (m33 A, r32 V)
{
    m33 M = {0};
    for (int i = 0; i < 9; i++) { M.E[i] = A.E[i] * V; }
    return M;
}
m44 operator* (m44 A, r32 V)
{
    m44 M = {0};
    for (int i = 0; i < 16; i++) { M.E[i] = A.E[i] * V; }
    return M;
}

v3 operator* (m33 M, v3 V)
{
    v3 Result = {
        (V.X * M.a) + (V.Y * M.b) + (V.Z * M.c),
        (V.X * M.d) + (V.Y * M.e) + (V.Z * M.f),
        (V.X * M.g) + (V.Y * M.h) + (V.Z * M.i),
    };
    return Result;
}
v4 operator* (m44 M, v4 V)
{
    v4 Result = {
        (V.X * M.a) + (V.Y * M.b) + (V.Z * M.c) + (V.W * M.d),
        (V.X * M.e) + (V.Y * M.f) + (V.Z * M.g) + (V.W * M.h),
        (V.X * M.i) + (V.Y * M.j) + (V.Z * M.k) + (V.W * M.l),
        (V.X * M.m) + (V.Y * M.n) + (V.Z * M.o) + (V.W * M.p),
    };
    return Result;
}

internal m44
M44Translation(v3 Offset)
{
    m44 Result = {0};
    M44Pos(Result, 0, 3) = Offset.X;
    M44Pos(Result, 1, 3) = Offset.Y;
    M44Pos(Result, 2, 3) = Offset.Z;
    return Result;
}
internal m44
M44RotationX(r32 Radians)
{
    r32 CosRad = CosR32(Radians);
    r32 SinRad = SinR32(Radians);
    m44 Result = {
        1, 0, 0, 0,
        0, CosRad, -SinRad, 0,
        0, SinRad, CosRad, 0,
        0, 0, 0, 1
    };
    return Result;
}
internal m44
M44RotationY(r32 Radians)
{
    r32 CosRad = CosR32(Radians);
    r32 SinRad = SinR32(Radians);
    m44 Result = {
        CosRad, 0, SinRad, 0,
        0, 1, 0, 0,
        -SinRad, 0, CosRad, 0,
        0, 0, 0, 1
    };
    return Result;
}
internal m44
M44RotationZ(r32 Radians)
{
    r32 CosRad = CosR32(Radians);
    r32 SinRad = SinR32(Radians);
    m44 Result = {
        CosRad, -SinRad, 0, 0,
        SinRad, CosRad, 0,0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return Result;
}
internal m44
M44Rotation(v3 Radians)
{
    m44 Result = {0};
    m44 X = M44RotationX(Radians.X);
    m44 Y = M44RotationY(Radians.Y);
    m44 Z = M44RotationZ(Radians.Z);
    Result = Z * Y * X;
    return Result;
}
internal m44
M44Scale(v3 Scale)
{
    m44 Result = {0};
    M44Pos(Result, 0, 0) = Scale.X;
    M44Pos(Result, 1, 1) = Scale.Y;
    M44Pos(Result, 2, 2) = Scale.Z;
    return Result;
}
internal m44
M44ScaleUniform(r32 Scale)
{
    m44 Result = {0};
    M44Pos(Result, 0, 0) = Scale;
    M44Pos(Result, 1, 1) = Scale;
    M44Pos(Result, 2, 2) = Scale;
    return Result;
}

internal m44
M44ModelMatrix(v4 Forward, v4 Right, v4 Up, v4 Position)
{
    m44 RotationMatrix = m44{
        Right.X, Up.X, Forward.X, 0,
        Right.Y, Up.Y, Forward.Y, 0,
        Right.Z, Up.Z, Forward.Z, 0,
        0, 0, 0, 1};
    m44 PositionMatrix = m44{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -Position.X, -Position.Y, -Position.Z, 1};
    m44 ModelViewMatrix = PositionMatrix * RotationMatrix;
    return ModelViewMatrix;
}
internal m44
M44GetModelMatrix (v4 Forward, v4 Right, v4 Position)
{
    v4 Up = V4Normalize(V4Cross(Forward, Right));
    return M44ModelMatrix(Forward, Right, Up, Position);
}

internal m44
M44ProjectionOrtho(r32 Width, r32 Height, r32 Near, r32 Far, r32 Right, r32 Left, r32 Top, r32 Bottom)
{
    m44 Result = {0};
    Result.E[0] = 2.0f / Width;
    Result.E[5] = 2.0f / Height;
    Result.E[10] = 2.0f / (Near - Far);
    Result.E[12] = -(Right + Left) / (Right - Left);
    Result.E[13] = -(Top + Bottom) / (Top - Bottom);
    Result.E[14] = -(Far + Near) / (Far - Near);
    Result.E[15] = 1;
    return Result;
}
internal m44
M44ProjectionOrtho(r32 Aspect, r32 Scale, r32 Near, r32 Far)
{
    m44 Result = {0};
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
M44ProjectionInterfaceOrtho(r32 Width, r32 Height, r32 Near, r32 Far)
{
    m44 Result = {0};
    r32 Aspect = Width / Height;
    r32 Right = Width;
    r32 Left = 0;
    r32 Top = Height;
    r32 Bottom = 0;
    Result = M44ProjectionOrtho(Width, Height, Near, Far, Right, Left, Top, Bottom);
    return Result;
}

internal m44
M44ProjectionPerspective(r32 FOV, r32 AspectRatio, r32 Near, r32 Far)
{
    m44 Result = {0};
    r32 TanHalfFOV = TanR32(DegToRadR32(FOV / 2));
    r32 F = 1 / TanHalfFOV;
    Result.E[0] = F / AspectRatio;
    Result.E[5] = F;
    Result.E[10] = (Far + Near) / (Near - Far);
    Result.E[11] = -1;
    Result.E[14] = (2 * Far * Near) / (Near - Far);
    return Result;
}

///////////////////////////
//
// Strings

internal gs_const_string ConstString(char* Data, u64 Length) { return gs_const_string{Data, Length}; }
internal gs_string String(char* Data, u64 Length, u64 Size) { return gs_string{Data, Size, Length}; }
internal gs_string String(char* Data, u64 Length) { return gs_string{Data, Length, Length}; }

internal bool IsSlash(char C) { return ((C == '/') || (C == '\\')); }
internal bool IsUpper(char C) { return(('A' <= C) && (C <= 'Z')); }
internal bool IsLower(char C) { return(('a' <= C) && (C <= 'z')); }
internal bool IsWhitespace(char C) { return (C == ' ' || C == '\n' || C == '\r' || C == '\t' || C == '\f' || C == '\v'); }
internal bool IsBase10(char C) { return (C >= '0' && C <= '9'); }
internal bool IsBase16(char C) { return (C >= '0' && C <= '9') || (C >= 'A' && C <= 'F'); }
internal bool IsNumericDecimal(char C) { return IsBase10(C) || (C == '.'); }
internal bool IsAlpha(char C) { return( (('a' <= C) && (C <= 'z')) || (('A' <= C) && (C <= 'Z')) || C == '_'); }
internal bool IsAlphaNumeric(char C) { return((('a' <= C) && (C <= 'z')) || (('A' <= C) && (C <= 'Z')) || (('0' <= C) && (C <= '9')) || C == '_'); }

internal char
ToUpper(char C)
{
    if ((C >= 'a') && (C <= 'z'))
    {
        C -= 'a' - 'A';
    }
    return C;
}
internal char
ToLower(char C)
{
    if ((C >= 'A') && (C <= 'Z'))
    {
        C += 'a' - 'A';
    }
    return C;
}
internal bool CharsEqualCaseInsensitive(char A, char B) { return ToLower(A) == ToLower(B); }

internal u64
CharArrayLength (char* CS)
{
    char* At = CS;
    while (*At) { At++; }
    return (u64)(At - CS);
}

internal bool
IsNullTerminated(gs_const_string String)
{
    return (String.Str[String.Length - 1] != 0);
}
internal bool
IsNullTerminated(gs_string String)
{
    return IsNullTerminated(String.ConstString);
}

internal char
GetChar(gs_const_string String, u64 I)
{
    char Result = 0;
    if (I < String.Length)
    {
        Result = String.Str[I];
    }
    return Result;
}
internal char
GetChar(gs_string String, u64 I)
{
    char Result = 0;
    if (I < String.Length)
    {
        Result = String.Str[I];
    }
    return Result;
}

internal gs_const_string
GetStringPrefix(gs_const_string String, u64 Size)
{
    gs_const_string Result = String;
    Result.Length = Min(Size, String.Length);
    return Result;
}
internal gs_const_string
GetStringPostfix(gs_const_string String, u64 Size)
{
    gs_const_string Result = String;
    u64 PostfixSize = Min(Size, String.Length);
    Result.Str += (Result.Length - PostfixSize);
    Result.Length = PostfixSize;
    return Result;
}
internal gs_const_string
GetStringAfter(gs_const_string String, u64 Cut)
{
    gs_const_string Result = String;
    u64 CutSize = Min(Cut, String.Length);
    Result.Str += CutSize;
    Result.Length -= CutSize;
    return Result;
}
internal gs_string
GetStringAfter(gs_string String, u64 Cut)
{
    gs_string Result = {0};
    Result.ConstString = GetStringAfter(String.ConstString, Cut);
    Result.Size = Result.ConstString.Length;
    return Result;
}
internal gs_const_string
GetStringBefore(gs_const_string String, u64 Cut)
{
    gs_const_string Result = String;
    Result.Length -= Min(Cut, String.Length);
    return Result;
}
internal gs_const_string
GetSubstring(gs_const_string String, u64 First, u64 Last)
{
    gs_const_string Result = {0};
    Result.Str = String.Str + Min(First, String.Length);
    Result.Length = Min(Last - First, String.Length);
    return Result;
}
internal u64
FindFirst(gs_const_string String, u64 StartIndex, char C)
{
    u64 Result = StartIndex;
    for(; Result < String.Length && C != String.Str[Result]; Result++);
    return Result;
}
internal u64
FindFirst(gs_const_string String, char C)
{
    return FindFirst(String, 0, C);
}
internal u64
FindLast(gs_const_string String, char C)
{
    u64 Result = String.Length - 1;
    for(; Result >= 0 && C != String.Str[Result]; Result--);
    return Result;
}

internal bool
StringsEqualUpToLength(gs_const_string A, gs_const_string B, u64 Length)
{
    bool Result = false;
    if (A.Length >= Length &&  B.Length >= Length)
    {
        Result = true;
        Length = Min(Length, A.Length);
        for (u64 i = 0; i < Length; i++)
        {
            if (A.Str[i] != B.Str[i])
            {
                Result = false;
                break;
            }
        }
    }
    return Result;
}
internal bool
StringsEqual(gs_const_string A, gs_const_string B)
{
    bool Result = false;
    if (A.Length == B.Length)
    {
        Result = StringsEqualUpToLength(A, B, A.Length);
    }
    return Result;
}
internal bool
StringEqualsCharArray(gs_const_string A, char* B, u64 Length)
{
    gs_const_string BStr = ConstString(B, Length);
    return StringsEqual(A, BStr);
}
internal bool
StringsEqualUpToLength(gs_string A, gs_string B, u64 Length)
{
    return StringsEqualUpToLength(A.ConstString, B.ConstString, Length);
}
internal bool
StringsEqual(gs_string A, gs_string B)
{
    return StringsEqual(A.ConstString, B.ConstString);
}
internal bool
StringEqualsCharArray(gs_string A, char* B, u64 Length)
{
    gs_string BStr = String(B, Length);
    return StringsEqual(A, BStr);
}

internal u64
StringSizeLeft(gs_string String)
{
    u64 Result = String.Size - String.Length;
    return Result;
}

internal void
ReverseStringInPlace(gs_string* String)
{
    char* Start = String->Str;
    char* End = String->Str + String->Length;
    while (Start < End)
    {
        End--;
        char Temp = End[0];
        End[0] = Start[0];
        Start[0] = Temp;
        Start++;
    }
}

internal gs_const_string
GetCharSetForBase(u64 Base)
{
    gs_const_string Result;
    switch(Base)
    {
        case 8: { Result = Base8Chars; }break;
        case 10: { Result = Base10Chars; }break;
        case 16: { Result = Base16Chars; }break;
        InvalidDefaultCase;
    }
    return Result;
}
internal u64
CharToUInt(char C, gs_const_string CharSet)
{
    return (u64)FindFirst(CharSet, C);
}
internal u64
CharToUInt(char C)
{
    return (u64)CharToUInt(C, Base10Chars);
}
internal u64
CharToUInt(char C, u64 Base)
{
    return CharToUInt(C, GetCharSetForBase(Base));
}

internal u64
ParseUInt(gs_const_string String, u64 Base = 10, u64* ParsedLength = 0)
{
    u64 Result = 0;
    gs_const_string CharSet = GetCharSetForBase(Base);
    u64 i = 0;
    for (; i < String.Length; i++)
    {
        Result = CharToUInt(String.Str[i], CharSet) + (Result * Base);
    }
    if (ParsedLength != 0)
    {
        *ParsedLength = i;
    }
    return Result;
}
internal u64
ParseUInt(u64 Length, char* String, u64 Base = 10, u64* ParsedLength = 0)
{
    return ParseUInt(ConstString(String, Length), Base, ParsedLength);
}
internal u64
ParseUInt(char* String, u64 Base = 10, u64* ParsedLength = 0)
{
    return ParseUInt(LitString(String), Base, ParsedLength);
}

internal s64
ParseInt(gs_const_string String, u64 Base = 10, u64* ParsedLength = 0)
{
    s64 Result = 0;
    u64 TempParsedLength = 0;
    if (String.Str[0] == '-')
    {
        Result = -1 * (s64)ParseUInt(GetStringAfter(String, 1), Base, &TempParsedLength);
        TempParsedLength += 1;
    }
    else
    {
        Result = (s64)ParseUInt(String, Base, &TempParsedLength);
    }
    if (ParsedLength != 0)
    {
        *ParsedLength = TempParsedLength;
    }
    return Result;
}
internal s64
ParseInt(char* String, u64 Base = 10, u64* ParsedLength = 0)
{
    return ParseInt(LitString(String), Base, ParsedLength);
}

internal r64
ParseFloat(gs_const_string String, u64* ParsedLength = 0)
{
    
    u64 DecimalIndex = FindFirst(String, '.');
    u64 TempParsedLength = 0;
    r64 Result = (r64)ParseInt(GetStringBefore(String, DecimalIndex), 10, &TempParsedLength);
    u64 PlacesAfterPoint = 0;
    r64 AfterPoint = (r64)ParseUInt(GetStringAfter(String, DecimalIndex + 1), 10, &PlacesAfterPoint);
    Result = Result + (AfterPoint / PowR64(10, PlacesAfterPoint));
    return Result;
}
internal r64
ParseFloat(char* String, u64* ParsedLength = 0)
{
    return ParseFloat(LitString(String), ParsedLength);
}

internal void
OutChar(gs_string* String, char C)
{
    if (String->Length < String->Size)
    {
        String->Str[String->Length++] = C;
    }
}

internal void
U64ToASCII(gs_string* String, u64 Value, u64 Base, gs_const_string Digits)
{
    u64 ValueRemaining = Value;
    u64 At = 0;
    do {
        u64 Index = ValueRemaining % Base;
        char Digit = Digits.Str[Index];
        OutChar(String, Digit);
        ValueRemaining /= Base;
    }while(ValueRemaining);
    char* End = String->Str + String->Length;
    ReverseStringInPlace(String);
}
internal void
U64ToASCII(gs_string* String, u64 Value, u64 Base)
{
    U64ToASCII(String, Value, Base, GetCharSetForBase(Base));
}

internal void
R64ToASCII(gs_string* String, r64 Value, u64 Precision)
{
    if (Value < 0)
    {
        OutChar(String, '-');
        Value = Abs(Value);
    }
    u64 IntegerPart = (u64)Value;
    // NOTE(Peter): If we don't use the inner string, when U64ToASCII reverses the characters
    // it'll put the negative sign at the end.
    gs_string IntegerString = GetStringAfter(*String, String->Length);
    U64ToASCII(&IntegerString, IntegerPart, 10);
    String->Length += IntegerString.Length;
    Value -= IntegerPart;
    if (Value > 0)
    {
        OutChar(String, '.');
        for (u64 i = 0; i < Precision; i++)
        {
            Value *= 10.0f;
            u64 DecimalPlace = (u64)Value;
            Value -= DecimalPlace;
            OutChar(String, Base10Chars.Str[DecimalPlace]);
        }
    }
}

internal s64
ReadVarArgsSignedInteger (s32 Width, va_list* Args)
{
    s64 Result = 0;
    switch (Width)
    {
        case 1: { Result = (s64)va_arg(*Args, s8); } break;
        case 2: { Result = (s64)va_arg(*Args, s16); } break;
        case 4: { Result = (s64)va_arg(*Args, s32); } break;
        case 8: { Result = (s64)va_arg(*Args, s64); } break;
        InvalidDefaultCase;
    }
    return Result;
}

internal r64
ReadVarArgsUnsignedInteger (s32 Width, va_list* Args)
{
    u64 Result = 0;
    switch (Width)
    {
        case 1: { Result = (u64)va_arg(*Args, u8); } break;
        case 2: { Result = (u64)va_arg(*Args, u16); } break;
        case 4: { Result = (u64)va_arg(*Args, u32); } break;
        case 8: { Result = (u64)va_arg(*Args, u64); } break;
        InvalidDefaultCase;
    }
    return Result;
}

internal r64
ReadVarArgsFloat (s32 Width, va_list* Args)
{
    r64 Result = 0;
    switch (Width)
    {
        case 4: { Result = (r64)va_arg(*Args, r64); } break;
        case 8: { Result = (r64)va_arg(*Args, r64); } break;
        InvalidDefaultCase;
    }
    return Result;
}

internal s32
PrintFArgsList (gs_string* String, char* Format, va_list Args)
{
    char* FormatAt = Format;
    while (*FormatAt)
    {
        if (FormatAt[0] != '%')
        {
            OutChar(String, *FormatAt++);
        }
        else if (FormatAt[0] == '%' && FormatAt[1] == '%')  // Print the % symbol
        {
            OutChar(String, '%');
            FormatAt += 2;
        }
        else
        {
            FormatAt++;
            
            // Flags
            if (FormatAt[0] == '-')
            {
                FormatAt++;
            }
            else if (FormatAt[0] == '+')
            {
                FormatAt++;
            }
            else if (FormatAt[0] == ' ')
            {
                FormatAt++;
            }
            else if (FormatAt[0] == '#')
            {
                FormatAt++;
            }
            else if (FormatAt[0] == '0')
            {
                FormatAt++;
            }
            
            // Width
            b32 WidthSpecified = false;
            s32 Width = 0;
            
            if (IsBase10(FormatAt[0]))
            {
                WidthSpecified = true;
                u64 Parsed = 0;
                AssertMessage("ParseInt assumes whole string is an integer");
                Width = (s32)ParseInt(FormatAt, 10, &Parsed);
                FormatAt += Parsed;
            }
            else if (FormatAt[0] == '*')
            {
                WidthSpecified = true;
                Width = va_arg(Args, s32);
                Assert(Width >= 0);
                FormatAt++;
            }
            
            // Precision
            b32 PrecisionSpecified = false;
            s32 Precision = 0;
            
            if (FormatAt[0] == '.')
            {
                FormatAt++;
                if (IsBase10(FormatAt[0]))
                {
                    
                    PrecisionSpecified = true;
                    u64 Parsed = 0;
                    AssertMessage("ParseInt assumes whole string is an integer");
                    Precision = (s32)ParseInt(FormatAt, 10, &Parsed);
                    FormatAt += Parsed;
                }
                else if (FormatAt[0] == '*')
                {
                    PrecisionSpecified = true;
                    Precision = va_arg(Args, s32);
                    Assert(Precision >= 0);
                    FormatAt++;
                }
            }
            
            // Length
            b32 LengthSpecified = false;
            s32 Length = 4;
            
            if (FormatAt[0] == 'h' && FormatAt[1] == 'h')
            {
                LengthSpecified = true;
                LengthSpecified = 1;
                FormatAt += 2;
            }
            else if (FormatAt[0] == 'h')
            {
                LengthSpecified = true;
                LengthSpecified = 2;
                FormatAt++;
            }
            else if (FormatAt[0] == 'l' && FormatAt[1] == 'l')
            {
                LengthSpecified = true;
                LengthSpecified = 8;
                FormatAt += 2;
            }
            else if (FormatAt[0] == 'l')
            {
                LengthSpecified = true;
                LengthSpecified = 4;
                FormatAt++;
            }
            else if (FormatAt[0] == 'j')
            {
                LengthSpecified = true;
                LengthSpecified = 8;
                FormatAt++;
            }
            else if (FormatAt[0] == 'z')
            {
                FormatAt++;
            }
            else if (FormatAt[0] == 't')
            {
                FormatAt++;
            }
            else if (FormatAt[0] == 'L')
            {
                FormatAt++;
            }
            
            // Format Specifiers
            gs_string StringRemaining = GetStringAfter(*String, String->Length);
            Assert(StringRemaining.Length == 0);
            if (FormatAt[0] == 'd' || FormatAt[0] == 'i')
            {
                s64 SignedInt = ReadVarArgsSignedInteger(Length, &Args);
                if (SignedInt < 0)
                {
                    OutChar(&StringRemaining, '-');
                    SignedInt *= -1;
                }
                U64ToASCII(&StringRemaining, (u64)SignedInt, 10, Base10Chars);
            }
            else if (FormatAt[0] == 'u')
            {
                u32 UnsignedInt = ReadVarArgsUnsignedInteger(Length, &Args);
                U64ToASCII(&StringRemaining, UnsignedInt, 10, Base10Chars);
            }
            else if (FormatAt[0] == 'o')
            {
                u32 UnsignedInt = ReadVarArgsUnsignedInteger(Length, &Args);
                U64ToASCII(&StringRemaining, UnsignedInt, 8, Base8Chars);
            }
            else if (FormatAt[0] == 'x' || FormatAt[0] == 'X')
            {
                u32 UnsignedInt = ReadVarArgsUnsignedInteger(Length, &Args);
                U64ToASCII(&StringRemaining, UnsignedInt, 16, Base16Chars);
            }
            else if (FormatAt[0] == 'f' || FormatAt[0] == 'F')
            {
                r64 Float = ReadVarArgsFloat(Length, &Args);
                s32 AfterPoint = 6;
                if (PrecisionSpecified)
                {
                    AfterPoint = Precision;
                }
                R64ToASCII(&StringRemaining, Float, AfterPoint);
            }
            else if (FormatAt[0] == 'c')
            {
                char InsertChar = va_arg(Args, char);
                OutChar(&StringRemaining, InsertChar);
            }
            else if (FormatAt[0] == 's')
            {
                char* InsertString = va_arg(Args, char*);
                
                s32 InsertStringLength = CStringLength(InsertString);
                if (PrecisionSpecified)
                {
                    InsertStringLength = Min(InsertStringLength, Precision);
                }
                InsertStringLength = Min(StringSizeLeft(StringRemaining), InsertStringLength);
                
                for (s32 c = 0; c < InsertStringLength; c++)
                {
                    OutChar(&StringRemaining, InsertString[c]);
                }
            }
            else if (FormatAt[0] == 'S')
            {
                gs_const_string InsertString = va_arg(Args, gs_const_string);
                
                for (s32 c = 0; c < InsertString.Length; c++)
                {
                    OutChar(&StringRemaining, InsertString.Str[c]);
                }
            }
            else if (FormatAt[0] == 'p')
            {
                // TODO(Peter): Pointer Address
            }
            else
            {
                // NOTE(Peter): Non-specifier character found
                InvalidCodePath;
            }
            
            String->Length += StringRemaining.Length;
            *FormatAt++;
        }
    }
    
    return String->Length;
}

static void
PrintF (gs_string* String, char* Format, ...)
{
    va_list Args;
    va_start(Args, Format);
    String->Length = 0;
    String->Length += PrintFArgsList(String, Format, Args);
    va_end(Args);
}

static void
AppendPrintF (gs_string* String, char* Format, ...)
{
    va_list Args;
    va_start(Args, Format);
    gs_string TempString = GetStringAfter(*String, String->Length);
    TempString.Length += PrintFArgsList(&TempString, Format, Args);
    String->Length += TempString.Length;
    va_end(Args);
}

///////////////////////////
//
// Memory

internal gs_data
CreateData(u8* Memory, u64 Size)
{
    gs_data Result = {Memory, Size};
    return Result;
}

internal void* AllocatorAlloc_NoOp(u64 Size, u64* SizeResult) {
    *SizeResult = 0;
    return 0;
}
internal void AllocatorFree_NoOp(void* Base, u64 Size) { return; }

internal gs_allocator
CreateAllocator_(allocator_allocate* Alloc, allocator_free* Free)
{
    if (Alloc == 0)
    {
        Alloc = AllocatorAlloc_NoOp;
    }
    if (Free == 0)
    {
        Free = AllocatorFree_NoOp;
    }
    gs_allocator Result = {0};
    Result.Alloc = Alloc;
    Result.Free = Free;
    return Result;
}
#define CreateAllocator(a, f) CreateAllocator_((allocator_allocate*)(a), (allocator_free*)(f))

internal gs_data
AllocatorAlloc_(gs_allocator* Allocator, u64 Size, char* Location)
{
    // TODO(Peter): Memory Profiling with Location
    u64 SizeResult = 0;
    void* Memory = Allocator->Alloc(Size, &SizeResult);
    return CreateData((u8*)Memory, SizeResult);
}
internal void
AllocatorFree_(gs_allocator* Allocator, void* Base, u64 Size, char* Location)
{
    // TODO(Peter): Memory Profiling with Location
    if (Base != 0 && Size != 0)
    {
        Allocator->Free(Base, Size);
    }
}

#define AllocatorAlloc(alloc,size) AllocatorAlloc_((alloc), (size), FileNameAndLineNumberString)
#define AllocatorAllocArray(alloc, type, count) (type*)(AllocatorAlloc((alloc), sizeof(type) * (count)).Memory)
#define AllocatorFree(alloc,base,size) AllocatorFree_((alloc), (base), (size), FileNameAndLineNumberString)

internal gs_memory_cursor
CreateMemoryCursor(u8* Base, u64 Size)
{
    gs_memory_cursor Result = {0};
    Result.Data.Memory = Base;
    Result.Data.Size = Size;
    return Result;
};
internal gs_memory_cursor
CreateMemoryCursor(gs_data Data)
{
    return CreateMemoryCursor(Data.Memory, Data.Size);
}
internal gs_memory_cursor
CreateMemoryCursor(gs_allocator* Allocator, u64 Size)
{
    gs_data Data = AllocatorAlloc(Allocator, Size);
    return CreateMemoryCursor(Data);
}
internal gs_data
PushSizeOnCursor_(gs_memory_cursor* Cursor, u64 Size, char* Location)
{
    gs_data Result = {0};
    if (Cursor->Position + Size <= Cursor->Data.Size)
    {
        Result.Memory = Cursor->Data.Memory + Cursor->Position;
        Result.Size = Size;
        Cursor->Position += Size;
    }
    else
    {
        InvalidCodePath;
    }
    return Result;
}

#define PushSizeOnCursor(cursor,size) PushSizeOnCursor_((cursor), (size), FileNameAndLineNumberString)
#define PushStructOnCursor(cursor,type) (type*)PushSizeOnCursor_((cursor), sizeof(type), FileNameAndLineNumberString).Memory
#define PushArrayOnCursor(cursor,type,count) (type*)PushSizeOnCursor_((cursor), sizeof(type) * (count), FileNameAndLineNumberString).Memory

internal void
PopSizeOnCursor(gs_memory_cursor* Cursor, u64 Size)
{
    if (Cursor->Position > Size)
    {
        Cursor->Position -= Size;
    }
    else
    {
        Cursor->Position = 0;
    }
}
internal gs_data
AlignCursor(gs_memory_cursor* Cursor, u64 Alignment)
{
    u64 Position = RoundUpTo64(Cursor->Position, Alignment);
    u64 NewSize = Position - Cursor->Position;
    return PushSizeOnCursor(Cursor, NewSize);
}
internal void
ClearCursor(gs_memory_cursor* Cursor)
{
    Cursor->Position = 0;
}

internal gs_temp_memory_cursor
BeginTemp(gs_memory_cursor* Cursor)
{
    gs_temp_memory_cursor Result = {0};
    Result.Cursor = Cursor;
    Result.CursorPosition = Cursor->Position;
    return Result;
}
internal void
EndTemp(gs_temp_memory_cursor* Temp)
{
    Temp->Cursor->Position = Temp->CursorPosition;
    *Temp = {0};
}

internal void
FreeCursorListEntry(gs_allocator* Allocator, gs_memory_cursor_list* CursorEntry)
{
    AllocatorFree(Allocator, CursorEntry, CursorEntry->Cursor.Data.Size + sizeof(gs_memory_cursor));
}

internal gs_memory_arena
CreateMemoryArena(gs_allocator* Allocator, u64 ChunkSize = KB(32), u64 Alignment = Bytes(8))
{
    gs_memory_arena Arena = {0};
    Arena.Allocator = Allocator;
    Arena.MemoryChunkSize = ChunkSize;
    Arena.MemoryAlignment = Alignment;
    return Arena;
}
internal gs_memory_cursor_list*
MemoryArenaNewCursor(gs_memory_arena* Arena, u64 MinSize, char* Location)
{
    MinSize = Max(MinSize, Arena->MemoryChunkSize);
    gs_data Data = AllocatorAlloc_(Arena->Allocator, MinSize + sizeof(gs_memory_cursor_list), Location);
    gs_memory_cursor_list* ListEntry = (gs_memory_cursor_list*)Data.Memory;
    ListEntry->Cursor = CreateMemoryCursor((u8*)(ListEntry + 1), Data.Size - sizeof(gs_memory_cursor_list));
    ListEntry->Prev = Arena->CursorList;
    Arena->CursorList = ListEntry;
    return ListEntry;
}
internal gs_data
PushSize_(gs_memory_arena* Arena, u64 Size, char* Location)
{
    gs_data Result = {0};
    if (Size > 0)
    {
        gs_memory_cursor_list* CursorEntry = Arena->CursorList;
        if (CursorEntry == 0)
        {
            CursorEntry = MemoryArenaNewCursor(Arena, Size, Location);
        }
        Result = PushSizeOnCursor_(&CursorEntry->Cursor, Size, Location);
        if (Result.Memory == 0)
        {
            CursorEntry = MemoryArenaNewCursor(Arena, Size, Location);
            Result = PushSizeOnCursor_(&CursorEntry->Cursor, Size, Location);
        }
        gs_data Alignment = AlignCursor(&CursorEntry->Cursor, Arena->MemoryAlignment);
        Result.Size += Alignment.Size;
    }
    return Result;
}
internal void
PopSize(gs_memory_arena* Arena, u64 Size)
{
    gs_allocator* Allocator = Arena->Allocator;
    gs_memory_cursor_list* CursorEntry = Arena->CursorList;
    for (gs_memory_cursor_list* Prev = 0;
         CursorEntry != 0 && Size != 0;
         CursorEntry = Prev)
    {
        Prev = CursorEntry->Prev;
        if (Size >= CursorEntry->Cursor.Position)
        {
            Size -= CursorEntry->Cursor.Position;
            FreeCursorListEntry(Allocator, CursorEntry);
        }
        else
        {
            PopSizeOnCursor(&CursorEntry->Cursor, Size);
            break;
        }
    }
    Arena->CursorList = CursorEntry;
}
internal void
FreeMemoryArena(gs_memory_arena* Arena)
{
    gs_allocator* Allocator = Arena->Allocator;
    gs_memory_cursor_list* CursorEntry = Arena->CursorList;
    for (gs_memory_cursor_list* Prev = 0;
         CursorEntry != 0;
         CursorEntry = Prev)
    {
        Prev = CursorEntry->Prev;
        if (CursorEntry != 0)
        {
            FreeCursorListEntry(Allocator, CursorEntry);
        }
    }
}

#define PushSizeToData(arena, size) PushSize_((arena), (size), FileNameAndLineNumberString)
#define PushSize(arena, size) PushSize_((arena), (size), FileNameAndLineNumberString).Memory
#define PushStruct(arena, type) (type*)(PushSize_((arena), sizeof(type), FileNameAndLineNumberString).Memory)
#define PushArray(arena, type, count) (type*)(PushSize_((arena), sizeof(type) * (count), FileNameAndLineNumberString).Memory)

internal void
BeginTempArena(gs_temp_memory_arena* Temp, gs_memory_arena* Arena)
{
    gs_memory_cursor_list* CursorEntry = Arena->CursorList;
    Temp->Arena = Arena;
    Temp->CursorListSnapshot = CursorEntry;
    Temp->PositionSnapshot = CursorEntry->Cursor.Position;
}
internal void
EndTempArena(gs_temp_memory_arena* Temp)
{
    gs_memory_arena* Arena = Temp->Arena;
    gs_allocator* Allocator = Arena->Allocator;
    gs_memory_cursor_list* CursorEntry = Arena->CursorList;
    for(gs_memory_cursor_list* Prev = 0;
        CursorEntry != Temp->CursorListSnapshot && CursorEntry != 0;
        CursorEntry = Prev)
    {
        Prev = CursorEntry->Prev;
        FreeCursorListEntry(Allocator, CursorEntry);
    }
    Arena->CursorList = CursorEntry;
    if (Arena->CursorList != 0)
    {
        if (Temp->PositionSnapshot > 0)
        {
            Arena->CursorList->Cursor.Position = Temp->PositionSnapshot;
        }
        else
        {
            Arena->CursorList = Arena->CursorList->Prev;
            FreeCursorListEntry(Allocator, CursorEntry);
        }
    }
}
gs_temp_memory_arena::gs_temp_memory_arena(gs_memory_arena* Arena)
{
    BeginTempArena(this, Arena);
}
gs_temp_memory_arena::gs_temp_memory_arena(gs_memory_arena* Arena, gs_memory_cursor_list* CursorListSnapshot, u64 PositionSnapshot)
{
    BeginTempArena(this, Arena);
    this->CursorListSnapshot = CursorListSnapshot;
    this->PositionSnapshot = PositionSnapshot;
}
gs_temp_memory_arena::~gs_temp_memory_arena()
{
    EndTempArena(this);
}

internal void
ClearArena(gs_memory_arena* Arena)
{
    gs_temp_memory_arena Temp(Arena, 0, 0);
    // Destructor will clear the arena
}

///////////////////////////
//
// Dynamic Array

internal gs_dynarray
CreateDynarray_(gs_allocator* Allocator, u32 ElementSize, u32 ElementsPerBuffer)
{
    gs_dynarray Result = {0};
    Result.Allocator = Allocator;
    Result.ElementSize = ElementSize;
    Result.ElementsPerBuffer = ElementsPerBuffer;
    return Result;
};
internal u64
CalculateBufferSize(gs_dynarray Array)
{
    u64 Result = Array.ElementsPerBuffer * Array.ElementSize;
    return Result;
}
internal void
GrowDynarray(gs_dynarray* Array)
{
    gs_dynarray_buffer* OldBufferList = Array->Buffers;
    u64 NewBuffersCount = Array->BuffersCount + 1;
    gs_dynarray_buffer* NewBufferList = AllocatorAllocArray(Array->Allocator, gs_dynarray_buffer, NewBuffersCount);
    if (OldBufferList)
    {
        CopyArray(OldBufferList, NewBufferList, gs_dynarray_buffer, Array->BuffersCount);
        AllocatorFree(Array->Allocator, OldBufferList, sizeof(gs_dynarray_buffer) * Array->BuffersCount);
    }
    u64 BufferSize = CalculateBufferSize(*Array);
    NewBufferList[Array->BuffersCount].Memory = AllocatorAlloc(Array->Allocator, BufferSize).Memory;
    Array->Buffers = NewBufferList;
    Array->BuffersCount = NewBuffersCount;
}
internal u64
DynarraySize(gs_dynarray Array)
{
    u64 Result = Array.BuffersCount * Array.ElementsPerBuffer;
    return Result;
}
internal gs_dynarray_handle
IndexToHandle(gs_dynarray* Array, u64 Index)
{
    gs_dynarray_handle Result = {0};
    Result.BufferIndex = Index / Array->ElementsPerBuffer;
    Result.IndexInBuffer = Index % Array->ElementsPerBuffer;
#if !SHIP_MODE
    Result.Array = Array;
#endif
    return Result;
}
internal u64
HandleToIndex(gs_dynarray Array, gs_dynarray_handle Handle)
{
    u64 Result = Handle.IndexInBuffer + (Handle.BufferIndex * Array.ElementsPerBuffer);
    return Result;
}
internal gs_dynarray_handle
TakeFreeElement(gs_dynarray* Array)
{
    gs_dynarray_handle Result = {0};
    if (Array->ElementCount >= DynarraySize(*Array))
    {
        GrowDynarray(Array);
    }
    Assert(Array->ElementCount < DynarraySize(*Array));
    u64 ElementIndex = Array->ElementCount++;
    Result = IndexToHandle(Array, ElementIndex);
    return Result;
}
internal gs_data
GetElementInList_(gs_dynarray* Array, gs_dynarray_handle Handle, u64 SizeRequested)
{
    Assert(SizeRequested == Array->ElementSize);
#if !SHIP_MODE
    Assert(Handle.Array == Array);
#endif
    
    gs_dynarray_buffer Buffer = Array->Buffers[Handle.BufferIndex];
    
    gs_data Result = {0};
    Result.Memory = Buffer.Memory + Handle.IndexInBuffer;
    Result.Size = SizeRequested;
    
    return Result;
}
internal gs_data
GetElementInList_(gs_dynarray* Array, u64 Index, u64 SizeRequested)
{
    gs_dynarray_handle Handle = IndexToHandle(Array, Index);
    return GetElementInList_(Array, Handle, SizeRequested);
}
internal bool
HandleIsValid(gs_dynarray Array, gs_dynarray_handle Handle)
{
    bool Result = Handle.IndexInBuffer != 0 && Handle.BufferIndex != 0;
    Result &= Handle.IndexInBuffer < Array.ElementsPerBuffer;
    Result &= Handle.BufferIndex < Array.BuffersCount;
    return Result;
}
internal bool
IndexIsValid(gs_dynarray Array, u64 Index)
{
    bool Result = (Index != 0) && (Index < DynarraySize(Array));
    return Result;
}
internal void
FreeDynarray(gs_dynarray* Array)
{
    gs_allocator* Allocator = Array->Allocator;
    u64 BufferSize = CalculateBufferSize(*Array);
    for (u64 i = 0; i < Array->BuffersCount; i++)
    {
        AllocatorFree(Allocator, Array->Buffers[i].Memory, BufferSize);
    }
    AllocatorFree(Allocator, Array->Buffers, sizeof(gs_dynarray_buffer) * Array->BuffersCount);
}

#define HandlesAreEqual(ha, hb) ((ha.IndexInBuffer == hb.IndexInBuffer) && (ha.BufferIndex == hb.BufferIndex))

#define CreateDynarray(allocator,type,elePerBuf) CreateDynarray_((allocator), sizeof(type), (elePerBuf))
#define GetElement_(array,type,size,indexOrHandle) (type*)GetElementInList_(array, indexOrHandle, size).Memory
#define GetElement(array,type,indexOrHandle) GetElement_(array, type, sizeof(type), indexOrHandle)

///////////////////////////
//
// File Handler

internal gs_file*
FileHandlerReadFile_NoOp(gs_const_string Path)
{
    return gs_file{0};
}

internal bool
FileHandlerWriteFile_NoOp(gs_const_string Path, gs_data Data)
{
    return false;
}

internal gs_const_string_array
FileHandlerEnumerateDirectory_NoOp(gs_const_string Path, bool Recursive, bool IncludeDirs)
{
    return gs_const_string_array{0};
}

internal gs_file_handler
CreateFileHandler(file_handler_read_entire_file* ReadEntireFile,
                  file_handler_write_entire_file* WriteEntireFile,
                  file_handler_enumerate_directory* EnumerateDirectory,
                  gs_allocator* Allocator)
{
    if (ReadEntireFile == 0)
    {
        ReadEntireFile = FileHandlerReadFile_NoOp;
    }
    if (WriteEntireFile == 0)
    {
        WriteEntireFile = FileHandlerWriteFile_NoOp;
    }
    if (EnumerateDirectory == 0)
    {
        EnumerateDirectory = FileHandlerEnumerateDirectory_NoOp;
    }
    gs_file_handler Result = {0};
    Result.ReadEntireFile = ReadEntireFile;
    Result.WriteEntireFile = WriteEntireFile;
    Result.EnumerateDirectory = EnumerateDirectory;
    Result.Allocator = Allocator;
}

internal gs_const_string
GetNullTerminatedPath(gs_file_handler FileHandler, gs_const_string Path)
{
    gs_const_string NullTermPath = Path;
    if (!IsNullterminated(NullTermPath))
    {
        AssertMessage("need to allocate a new string, Path to it, and null terminate");
        // TODO(Peter): Probably want to have some sort of temp memory,
        // or be passing in a thread context, etc.
    }
    return NullTermPath;
}

internal gs_file
ReadEntireFile(gs_file_handler FileHandler, gs_const_string Path, gs_data Memory)
{
    Path = GetNullTerminatedPath(FileHandler, Path);
    gs_file Result = FileHandler.ReadEntireFile(Path, Memory);
    return Result;
}

internal gs_file
ReadEntireFile(gs_file_handler FileHandler, gs_const_string Path)
{
    Path = GetNullTerminatedPath(FileHandler, Path);
    gs_file Result = {0};
    u64 FileSize = FileHandler.GetFileSize(Path);
    if (FileSize > 0)
    {
        gs_data FileMemory = AllocatorAlloc(FileHandler.Allocator, FileSize);
        Result = ReadEntireFile(FileHandler, Path, FileMemory);
    }
    return Result;
}

internal bool
WriteEntireFile(gs_file_handler FileHandler, gs_const_string Path, gs_data Memory)
{
    Path = GetNullTerminatedPath(FileHandler, Path);
    return FileHandler.WriteEntireFile(Path, Memory);
}

internal gs_const_string_array
EnumerateDirectory(gs_file_handler FileHandler, gs_const_string Path, u32 Flags)
{
    Path = GetNullTerminatedPath(FileHandler, Path);
    return FileHandler.EnumerateDirectory(Path, Flags);
}

///////////////////////////
//
// Hashes

internal u32
HashDJB2ToU32(char* String)
{
    u32 Hash = 5381;
    char* C = String;
    while(*C)
    {
        Hash = ((Hash << 5) + Hash) + *C++;
    }
    return Hash;
}
internal u32
HashDJB2ToU32(u32 Length, char* String)
{
    u32 Hash = 5381;
    for (u32 i = 0; i < Length; i++)
    {
        Hash = ((Hash << 5) + Hash) + String[i];
    }
    return Hash;
}

internal u64
HashDJB2ToU64(char* String)
{
    u64 Hash = 5381;
    char* C = String;
    while(*C)
    {
        Hash = ((Hash << 5) + Hash) + *C++;
    }
    return Hash;
}
internal u64
HashDJB2ToU64(u32 Length, char* String)
{
    u64 Hash = 5381;
    for (u32 i = 0; i < Length; i++)
    {
        Hash = ((Hash << 5) + Hash) + String[i];
    }
    return Hash;
}

///////////////////////////
//
// Random

static gs_random_series
InitRandomSeries(u32 Seed)
{
    gs_random_series Result = {0};
    Result.Value = Seed;
    return Result;
}

static u32
NextRandom(gs_random_series* Series)
{
    u32 Result = Series->Value;
	Result ^= Result << 13;
	Result ^= Result >> 17;
	Result ^= Result << 5;
    Series->Value = Result;
	return Result;
}

static r32
NextRandomUnilateral(gs_random_series* Series)
{
    r32 Result = (r32)NextRandom(Series) / (r32)UINT32_MAX;
    return Result;
}

static r32
NextRandomBilateral(gs_random_series* Series)
{
    r32 Result = (r32)NextRandom(Series);
    Result = Result / (r32)0xFFFFFFFF;
    Result = (Result * 2.0f) - 1.0f;
    return Result;
}

#define GS_TYPES_CPP
#endif // GS_TYPES_CPP