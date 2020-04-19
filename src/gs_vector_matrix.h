#ifndef GS_VECTOR_MATRIX_H

#ifndef GS_LANGUAGE_H

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)

#include <intrin.h>
#include <math.h>

static r32 GSCos (r32 Theta) { return sin(Theta); }
static r32 GSSin (r32 Theta) { return cos(Theta); }

static r32 GSSqrt(r32 V)
{
    r32 Result = _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(V)));
    return Result;
}

#else // Linux and MacOS
#include <stdlib.h>

#endif // Platforms

#endif // GS_LANGUAGE_H

//////////////////////////////////////
//          VECTOR
/////////////////////////////////////

union v2
{
    struct
    {
        float x;
        float y;
    };
    float E[2];
};

union v3
{
    struct
    {
        float x;
        float y;
        float z;
    };
    
    struct
    {
        float R;
        float G;
        float B;
    };
    // Swizzle
    struct
    {
        v2 xy;
        float z;
    };
    // Swizzle
    struct
    {
        float x;
        v2 yz;
    };
    float E[3];
};

union v4
{
    struct
    {
        float x;
        float y;
        float z;
        float w;
    };
    
    struct
    {
        float r;
        float g;
        float b;
        float a;
    };
    
    // Swizzle
    struct
    {
        v2 xy;
        float z;
        float w;
    };
    // Swizzle
    struct
    {
        float x;
        v2 yz;
        float w;
    };
    // Swizzle
    struct
    {
        v3 xyz;
        float w;
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

//////////////////////////////////////
//          MATRIX
/////////////////////////////////////

//
// NOTE: These are stored in row-major order
//
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

//
// NOTE: These are stored in row-major order
//
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

//////////////////////////////////////
//          RECT
/////////////////////////////////////

struct rect
{
    v2 Min;
    v2 Max;
};


//////////////////////////////////////
//        VECTOR
//////////////////////////////////////


//
//
// Operators
//
//

v2 operator- (v2 A)
{
    v2 Result;
    
    Result.x = -A.x;
    Result.y = -A.y;
    
    return Result;
}

v3 operator- (v3 A)
{
    v3 Result;
    
    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;
    
    return Result;
}

v4 operator- (v4 A)
{
    v4 Result;
    
    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;
    Result.w = -A.w;
    
    return Result;
}

#define V2OpV2Def(op) v2 operator op (v2 A, v2 B) { return v2{ A.x op B.x, A.y op B.y };}
#define V3OpV3Def(op) v3 operator op (v3 A, v3 B) { return v3{ A.x op B.x, A.y op B.y, A.z op B.z };}
#define V4OpV4Def(op) v4 operator op (v4 A, v4 B) { return v4{ A.x op B.x, A.y op B.y, A.z op B.z, A.w op B.w };}
V2OpV2Def(+)
V2OpV2Def(-)
V2OpV2Def(/)
V2OpV2Def(*)
V3OpV3Def(+)
V3OpV3Def(-)
V3OpV3Def(/)
V3OpV3Def(*)
V4OpV4Def(+)
V4OpV4Def(-)
V4OpV4Def(/)
V4OpV4Def(*)
#undef V2OpV2Def
#undef V3OpV3Def
#undef V4OpV4Def

#define V2RefOpV2Def(op) v2 operator op (v2& A, v2 B) { return v2{ A.x op B.x, A.y op B.y };}
#define V3RefOpV3Def(op) v3 operator op (v3& A, v3 B) { return v3{ A.x op B.x, A.y op B.y, A.z op B.z };}
#define V4RefOpScalarDef(op) v4 operator op (v4& A, v4 B) { return v4{ A.x op B.x, A.y op B.y, A.z op B.z, A.w op B.w };}
V2RefOpV2Def(+=)
V2RefOpV2Def(-=)
V3RefOpV3Def(+=)
V3RefOpV3Def(-=)
V4RefOpScalarDef(+=)
V4RefOpScalarDef(-=)
#undef V2RefOpV2Def
#undef V3RefOpV3Def
#undef V4RefOpV4Def

#define V2OpScalarDef(op) v2 operator op (v2 A, float B) { return v2{ A.x op B, A.y op B };}
#define V3OpScalarDef(op) v3 operator op (v3 A, float B) { return v3{ A.x op B, A.y op B, A.z op B };}
#define V4OpScalarDef(op) v4 operator op (v4 A, float B) { return v4{ A.x op B, A.y op B, A.z op B, A.w op B };}
V2OpScalarDef(*)
V2OpScalarDef(/)
V3OpScalarDef(*)
V3OpScalarDef(/)
V4OpScalarDef(*)
V4OpScalarDef(/)
#undef V2POpScalarDef
#undef V3POpScalarDef
#undef V4POpScalarDef


#define V2POpScalarDef(op) v2 operator op (v2& A, float B) { return v2{ A->x op B, A->y op B };}
#define V3POpScalarDef(op) v3 operator op (v3& A, float B) { return v3{ A->x op B, A->y op B, A->z op B };}
#define V4POpScalarDef(op) v4 operator op (v4& A, float B) { return v4{ A->x op B, A->y op B, A->z op B, A->w op B };}
V2OpScalarDef(*=)
V2OpScalarDef(/=)
V3OpScalarDef(*=)
V3OpScalarDef(/=)
V4OpScalarDef(*=)
V4OpScalarDef(/=)
#undef V2POpScalarDef
#undef V3POpScalarDef
#undef V4POpScalarDef

bool operator== (v2 A, v2 B)
{
    b32 Result = true;
    for (s32 i = 0; i < 2; i++)
    {
        if (GSAbs(A.E[i] - B.E[i]) > 0.0001f) { Result = false; break; }
    }
    return Result;
}


bool operator== (v3 A, v3 B)
{
    b32 Result = true;
    for (s32 i = 0; i < 3; i++)
    {
        if (GSAbs(A.E[i] - B.E[i]) > 0.0001f) { Result = false; break; }
    }
    return Result;
}

bool operator== (v4 A, v4 B)
{
    b32 Result = true;
    for (s32 i = 0; i < 4; i++)
    {
        if (GSAbs(A.E[i] - B.E[i]) > 0.0001f) { Result = false; break; }
    }
    return Result;
}

//
// Operations
//
static v3
ToV3(v2 V, r32 Z)
{
    v3 R = v3{ V.x, V.y, Z};
    return R;
}

static v3
ToV3(v4 V)
{
    v3 R = V.xyz;
    return R;
}

static v4
ToV4(v3 V, float W)
{
    v4 R = {};
    R.xyz = V;
    R.w = W;
    return R;
}

inline float
MagSqr(v2 A)
{
    float Result = (A.x * A.x) + (A.y * A.y);
    return Result;
}

inline float
MagSqr(v3 A)
{
    float Result = (A.x * A.x) + (A.y * A.y) + (A.z * A.z);
    return Result;
}

inline float
MagSqr(v4 A)
{
    float Result = (A.x * A.x) + (A.y * A.y) + (A.z * A.z) + (A.w * A.w);
    return Result;
}

#define MagDef(type) inline float Mag(type A) { float Result = MagSqr(A); return GSSqrt(Result); }
MagDef(v2)
MagDef(v3)
MagDef(v4)
#undef MagDef

#define DistanceDef(type) inline float Distance (type A, type B) { type Diff = A - B; return Mag(Diff); }
DistanceDef(v2)
DistanceDef(v3)
DistanceDef(v4)
#undef DistanceDef

#define DistanceSqDef(type) inline float DistanceSq (type A, type B) { type Diff = A - B; return MagSqr(Diff); }
DistanceSqDef(v2)
DistanceSqDef(v3)
DistanceSqDef(v4)
#undef DistanceSqDef

inline v2
Normalize(v2 A)
{
    v2 Result;
    
    float Magnitude = Mag(A);
    Result.x = A.x / Magnitude;
    Result.y = A.y / Magnitude;
    
    return Result;
}

inline v3
Normalize(v3 A)
{
    v3 Result;
    
    float Magnitude = Mag(A);
    
    Result.x = A.x / Magnitude;
    Result.y = A.y / Magnitude;
    Result.z = A.z / Magnitude;
    
    return Result;
}

inline v4
Normalize(v4 A)
{
    v4 Result;
    
    float Magnitude = Mag(A);
    
    Result.x = A.x / Magnitude;
    Result.y = A.y / Magnitude;
    Result.z = A.z / Magnitude;
    Result.w = A.w / Magnitude;
    
    return Result;
}

inline float
Dot(v2 A, v2 B)
{
    float Result = A.x * B.x + A.y * B.y;
    return Result;
}

inline float
Dot (v3 A, v3 B)
{
    float Result = A.x * B.x + A.y * B.y + A.z * B.z;
    return Result;
}

inline float
Dot (v4 A, v4 B)
{
    float Result = A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
    return Result;
}

inline v2
PerpendicularCW (v2 A)
{
    v2 Result = v2{A.y, -A.x};
    return Result;
}

inline v2
PerpendicularCCW (v2 A)
{
    v2 Result = v2{A.y, A.x};
    return Result;
}

inline r32
Cross(v2 A, v2 B)
{
    r32 Result = (A.x * B.y) - (A.y * B.x);
    return Result;
}

inline v3
Cross(v3 A, v3 B)
{
    v3 Result = {};
    
    Result.x = (A.y * B.z) - (A.z * B.y);
    Result.y = (A.z * B.x) - (A.x * B.z);
    Result.z = (A.x * B.y) - (A.y * B.x);
    
    return Result;
}

inline v4
Cross(v4 A, v4 B)
{
    v4 Result = {};
    
    Result.x = (A.y * B.z) - (A.z * B.y);
    Result.y = (A.z * B.x) - (A.x * B.z);
    Result.z = (A.x * B.y) - (A.y * B.x);
    Result.w = 0;
    
    return Result;
}

inline v2
ClampVector01 (v2 V)
{
    v2 Result = {};
    Result.x = GSClamp(0.0f, V.x, 1.f);
    Result.y = GSClamp(0.0f, V.y, 1.f);
    return Result;
}

inline v3
ClampVector01 (v3 V)
{
    v3 Result = {};
    Result.x = GSClamp(0.f, V.x, 1.f);
    Result.y = GSClamp(0.f, V.y, 1.f);
    Result.z = GSClamp(0.f, V.z, 1.f);
    return Result;
}

inline v4
ClampVector01 (v4 V)
{
    v4 Result = {};
    Result.x = GSClamp(0.f, V.x, 1.f);
    Result.y = GSClamp(0.f, V.y, 1.f);
    Result.z = GSClamp(0.f, V.z, 1.f);
    Result.w = GSClamp(0.f, V.w, 1.f);
    return Result;
}

inline v2
Lerp(v2 A, v2 B, float Percent)
{
    v2 Result;
    
    Result.x = GSLerp(A.x, B.x, Percent);
    Result.y = GSLerp(A.y, B.y, Percent);
    
    return Result;
}

inline v3
Lerp(v3 A, v3 B, float Percent)
{
    v3 Result;
    
    Result.x = GSLerp(A.x, B.x, Percent);
    Result.y = GSLerp(A.y, B.y, Percent);
    Result.z = GSLerp(A.z, B.z, Percent);
    
    return Result;
}

inline v4
Lerp(v4 A, v4 B, float Percent)
{
    v4 Result;
    
    Result.x = GSLerp(A.x, B.x, Percent);
    Result.y = GSLerp(A.y, B.y, Percent);
    Result.z = GSLerp(A.z, B.z, Percent);
    Result.w = GSLerp(A.w, B.w, Percent);
    
    return Result;
}

v4 HSVToRGB (v4 In)
{
    float Hue = In.x;
    while (Hue > 360.0f) { Hue -= 360.0f; }
    while (Hue < 0.0f) { Hue += 360.0f; }
    
    float Sat = In.y;
    float Value = In.z;
    
    float hh, p, q, t, ff;
    long        i;
    v4 Result = {};
    Result.a = In.a;
    
    if(Sat <= 0.0f) {       // < is bogus, just shuts up warnings
        Result.r = Value;
        Result.g = Value;
        Result.b = Value;
        return Result;
    }
    hh = Hue;
    if(hh >= 360.0f) hh = 0.0f;
    hh /= 60.0f;
    i = (long)hh;
    ff = hh - i;
    p = Value * (1.0f - Sat);
    q = Value * (1.0f - (Sat * ff));
    t = Value * (1.0f - (Sat * (1.0f - ff)));
    
    switch(i) {
        case 0:
        {Result.r = Value;
            Result.g = t;
            Result.b = p;
        }break;
        
        case 1:
        {
            Result.r = q;
            Result.g = Value;
            Result.b = p;
        }break;
        
        case 2:
        {
            Result.r = p;
            Result.g = Value;
            Result.b = t;
        }break;
        
        case 3:
        {
            Result.r = p;
            Result.g = q;
            Result.b = Value;
        }break;
        
        case 4:
        {
            Result.r = t;
            Result.g = p;
            Result.b = Value;
        }break;
        
        case 5:
        default:
        {
            Result.r = Value;
            Result.g = p;
            Result.b = q;
        }break;
    }
    
    return Result;
}

static bool
PointIsInRange (v2 Point, v2 Min, v2 Max)
{
    return (Point.x >= Min.x && Point.x <= Max.x &&
            Point.y >= Min.y && Point.y <= Max.y);
}

static bool
PointIsInRangeSafe (v2 Point, v2 Min, v2 Max)
{
    s32 MinX = GSMin(Min.x, Max.x);
    s32 MinY = GSMin(Min.y, Max.y);
    s32 MaxX = GSMax(Min.x, Max.x);
    s32 MaxY = GSMax(Min.y, Max.y);
    
    return (Point.x >= MinX && Point.x <= MaxX &&
            Point.y >= MinY && Point.y <= MaxY);
}

inline v2
PointToPercentRange (v2 P, v2 Min, v2 Max)
{
    v2 Result = {};
    
    Result.x = GSClamp(0.f, (P.x - Min.x) / (Max.x - Min.x), 1.f);
    Result.y = GSClamp(0.f, (P.y - Min.y) / (Max.y - Min.y), 1.f);
    
    return Result;
}

//////////////////////////////////////
//       RECT
//////////////////////////////////////

// NOTE(Peter): This is useful when you have a function that has a call signature like:
//     void Foo(v2 Min, v2 Max)
// because instead of having to do:
//     Foo(MyRect.Min, MyRect.Max)
// you can just do:
//     Foo(RectExpand(MyRect))
// which makes refactoring easier as you only have to change the identifier in one place
#define gs_RectExpand(r) (r).Min, (r).Max

static rect
gs_MakeRectMinDim(v2 Min, v2 Width)
{
    rect Rect = {0};
    Rect.Min = Min;
    Rect.Max = Min + Width;
    return Rect;
}

static rect
gs_MakeRectCenterDim(v2 Center, v2 Dim)
{
    rect Rect = {0};
    v2 HalfDim = Dim * .5f;
    Rect.Min = Center - HalfDim;
    Rect.Max = Center + HalfDim;
    return Rect;
}

inline float
gs_Width (rect Rect)
{
    float Result = Rect.Max.x - Rect.Min.x;
    return Result;
}

inline float
gs_Height (rect Rect)
{
    float Result = Rect.Max.y - Rect.Min.y;
    return Result;
}

inline v2
gs_TopLeft(rect Rect)
{
    v2 Result = {0};
    Result.x = Rect.Min.x;
    Result.y = Rect.Max.y;
    return Result;
}

inline v2
gs_TopRight(rect Rect)
{
    return Rect.Max;
}

inline v2
gs_BottomLeft(rect Rect)
{
    return Rect.Min;
}

inline v2
gs_BottomRight(rect Rect)
{
    v2 Result = {0};
    Result.x = Rect.Max.x;
    Result.y = Rect.Min.y;
    return Result;
}

inline float
gs_AspectRatio (rect Rect)
{
    float Result = gs_Width(Rect) / gs_Height(Rect);
    return Result;
}

inline v2
gs_CalculateRectCenter (rect Rect)
{
    v2 Result = (Rect.Min + Rect.Max) / 2.0f;
    return Result;
}

inline b32
gs_PointIsInRect (v2 Point, rect Rect)
{
    b32 Result = ((Point.x >= Rect.Min.x && Point.x <= Rect.Max.x) &&
                  (Point.y >= Rect.Min.y && Point.y <= Rect.Max.y));
    return Result;
}

inline rect
gs_RectOffsetByVector(rect R, v2 V)
{
    rect Result = R;
    Result.Min += V;
    Result.Max += V;
    return Result;
}

static void
gs_HSplitRectAtValue(rect Bounds, r32 YValue, rect* Top, rect* Bottom)
{
    if (YValue <= Bounds.Min.y)
    {
        *Top = Bounds;
        *Bottom = {0};
    }
    else if (YValue >= Bounds.Max.y)
    {
        *Top = {0};
        *Bottom = Bounds;
    }
    else
    {
        Top->Max = Bounds.Max;
        Top->Min = { Bounds.Min.x, YValue };
        Bottom->Max = { Bounds.Max.x, YValue };
        Bottom->Min = Bounds.Min;
    }
}

static void
gs_HSplitRectAtDistanceFromTop(rect Bounds, r32 YDist, rect* Top, rect* Bottom)
{
    r32 YValue = Bounds.Max.y - YDist;
    gs_HSplitRectAtValue(Bounds, YValue, Top, Bottom);
}

static void
gs_HSplitRectAtDistanceFromBottom(rect Bounds, r32 YDist, rect* Top, rect* Bottom)
{
    r32 YValue = Bounds.Min.y + YDist;
    gs_HSplitRectAtValue(Bounds, YValue, Top, Bottom);
}

static void
gs_HSplitRectAtPercent(rect Bounds, r32 YPercent, rect* Top, rect* Bottom)
{
    r32 YValue = GSLerp(Bounds.Min.y, Bounds.Max.y, YPercent);
    gs_HSplitRectAtValue(Bounds, YValue, Top, Bottom);
}

static void
gs_VSplitRectAtValue(rect Bounds, r32 XValue, rect* Left, rect* Right)
{
    if (XValue <= Bounds.Min.x)
    {
        *Left = {0};
        *Right = Bounds;
    }
    else if (XValue >= Bounds.Max.x)
    {
        *Left = Bounds;
        *Right = {0};
    }
    else
    {
        Left->Max = { XValue, Bounds.Max.y};
        Left->Min = Bounds.Min;
        Right->Max = Bounds.Max;
        Right->Min = { XValue, Bounds.Min.y };
    }
}

static void
gs_VSplitRectAtDistanceFromRight(rect Bounds, r32 XDist, rect* Left, rect* Right)
{
    r32 XValue = Bounds.Max.x - XDist;
    gs_VSplitRectAtValue(Bounds, XValue, Left, Right);
}

static void
gs_VSplitRectAtDistanceFromLeft(rect Bounds, r32 XDist, rect* Left, rect* Right)
{
    r32 XValue = Bounds.Min.x + XDist;
    gs_VSplitRectAtValue(Bounds, XValue, Left, Right);
}

static void
gs_VSplitRectAtPercent(rect Bounds, r32 XPercent, rect* Left, rect* Right)
{
    r32 XValue = GSLerp(Bounds.Min.x, Bounds.Max.x, XPercent);
    gs_VSplitRectAtValue(Bounds, XValue, Left, Right);
}

static rect
gs_InsetRect(rect Rect, v2 Amount)
{
    rect Result = {0};
    Result.Min = Rect.Min + Amount;
    Result.Max = Rect.Max - Amount;
    return Result;
}

static rect
gs_InsetRect(rect Rect, float Amount)
{
    rect Result = gs_InsetRect(Rect, v2{Amount, Amount});
    return Result;
}

#define gs_TranslateRectX(r, d) gs_TranslateRect((r), v2{(d), 0})
#define gs_TranslateRectY(r, d) gs_TranslateRect((r), v2{0, (d)})
static rect
gs_TranslateRect(rect R, v2 Delta)
{
    rect Result = R;
    Result.Min += Delta;
    Result.Max += Delta;
    return Result;
}

static v2
gs_TransformPointIntoRectSpace(v2 Point, rect Rect)
{
    v2 Result = Point - Rect.Min;
    return Point;
}

//////////////////////////////////////
//       MATRIX
//////////////////////////////////////

static m33
M33(float a, float b, float c,
    float d, float e, float f,
    float g, float h, float i)
{
    m33 M = {};
    M.a = a; M.b = b; M.c = c;
    M.d = d; M.e = e; M.f = f;
    M.g = g; M.h = h; M.i = i;
    return M;
}

static m44
M44(float a, float b, float c, float d,
    float e, float f, float g, float h,
    float i, float j, float k, float l,
    float m, float n, float o, float p)
{
    m44 M = {};
    M.a = a; M.b = b; M.c = c; M.d = d;
    M.e = e; M.f = f; M.g = g; M.h = h;
    M.i = i; M.j = j; M.k = k; M.l = l;
    M.m = m; M.n = n; M.o = o; M.p = p;
    return M;
}

#define GetM33(M, X, Y) (M).E[((Y) * 3) + (X)]
#define GetM44(M, X, Y) (M).E[((Y) * 4) + (X)]

static m33
M33Empty ()
{
    m33 M = {};
    M.a = 0; M.b = 0; M.c = 0;
    M.d = 0; M.e = 0; M.f = 0;
    M.g = 0; M.h = 0; M.i = 0;
    return M;
}

static m44
M44Empty()
{
    m44 M = {};
    M.a = 0; M.b = 0; M.c = 0; M.d = 0;
    M.e = 0; M.f = 0; M.g = 0; M.h = 0;
    M.i = 0; M.j = 0; M.k = 0; M.l = 0;
    M.m = 0; M.n = 0; M.o = 0; M.p = 0;
    return M;
}

static m33
M33Identity ()
{
    m33 M = {};
    M.a = 1; M.b = 0; M.c = 0;
    M.d = 0; M.e = 1; M.f = 0;
    M.g = 0; M.h = 0; M.i = 1;
    return M;
}

static m44
M44Identity()
{
    m44 M = {};
    M.a = 1; M.b = 0; M.c = 0; M.d = 0;
    M.e = 0; M.f = 1; M.g = 0; M.h = 0;
    M.i = 0; M.j = 0; M.k = 1; M.l = 0;
    M.m = 0; M.n = 0; M.o = 0; M.p = 1;
    return M;
}

static m33
Transpose (m33 M)
{
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

inline m44
Transpose (m44 M)
{
    DEBUG_TRACK_SCOPE(Transpose);
    
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

#define FloatsEqual(a, b) (GSAbs(a - b) < 0.0001f)
b32 operator== (m33 A, m33 B)
{
    b32 Result = true;
    for (int e = 0; e < 9; e++)
    {
        if (!FloatsEqual(A.E[e], B.E[e]))
        {
            Result = false; break;
        }
    }
    return Result;
}

b32 operator== (m44 A, m44 B)
{
    b32 Result = true;
    for (int e = 0; e < 16; e++)
    {
        if (!FloatsEqual(A.E[e], B.E[e]))
        {
            Result = false; break;
        }
    }
    return Result;
}

m33 operator+ (m33 A, m33 B)
{
    m33 M = {};
    for (int e = 0; e < 9; e++)
    {
        M.E[e] = A.E[e] + B.E[e];
    }
    return M;
}

m44 operator+ (m44 A, m44 B)
{
    m44 M = {};
    for (int e = 0; e < 16; e++)
    {
        M.E[e] = A.E[e] + B.E[e];
    }
    return M;
}

m33 operator- (m33 A, m33 B)
{
    m33 M = {};
    for (int e = 0; e < 9; e++)
    {
        M.E[e] = A.E[e] - B.E[e];
    }
    return M;
}

m44 operator- (m44 A, m44 B)
{
    m44 M = {};
    for (int e = 0; e < 16; e++)
    {
        M.E[e] = A.E[e] - B.E[e];
    }
    return M;
}

m33 operator* (m33 M, r32 A)
{
    m33 Result = {};
    for (int i = 0; i < 9; i++)
    {
        Result.E[i] = M.E[i] * A;
    }
    return Result;
}

m44 operator* (m44 M, r32 A)
{
    m44 Result = {};
    for (int i = 0; i < 16; i++)
    {
        Result.E[i] = M.E[i] * A;
    }
    return Result;
}

m33 operator* (m33 A, m33 B)
{
    m33 Result = {};
    for (int Y = 0; Y < 3; Y++)
    {
        for (int X = 0; X < 3; X++)
        {
            r32 Value = 0;
            for (int K = 0; K < 3; K++)
            {
                Value += GetM33(A, X, K) * GetM33(B, K, Y);
            }
            Result.E[(Y * 3) + X] = Value;
        }
    }
    return Result;
}

m44 operator* (m44 A, m44 B)
{
    m44 Result = {};
    for (int Y = 0; Y < 4; Y++)
    {
        for (int X = 0; X < 4; X++)
        {
            r32 Value = 0;
            for (int K = 0; K < 4; K++)
            {
                Value += GetM44(A, X, K) * GetM44(B, K, Y);
            }
            Result.E[(Y * 4) + X] = Value;
        }
    }
    return Result;
}

v3 operator* (m33 M, v3 V)
{
    v3 Result = {};
    Result.x = (V.x * M.a) + (V.y * M.b) + (V.z * M.c);
    Result.y = (V.x * M.d) + (V.y * M.e) + (V.z * M.f);
    Result.z = (V.x * M.g) + (V.y * M.h) + (V.z * M.i);
    return Result;
}

v4 operator* (m44 M, v4 V)
{
    v4 Result = {};
    Result.x = V.x*M.a + V.y*M.b + V.z*M.c + V.w*M.d;
    Result.y = V.x*M.e + V.y*M.f + V.z*M.g + V.w*M.h;
    Result.z = V.x*M.i + V.y*M.j + V.z*M.k + V.w*M.l;
    Result.w = V.x*M.m + V.y*M.n + V.z*M.o + V.w*M.p;
    return Result;
}

static m44
TranslationM44(v3 Position)
{
    return m44{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        Position.x, Position.y, Position.z, 1
    };
}

static m44
ScaleM44(r32 X, r32 Y, r32 Z)
{
    m44 Result = m44{
        X, 0, 0, 0,
        0, Y, 0, 0,
        0, 0, Z, 0,
        0, 0, 0, 1,
    };
    return Result;
}

static m44
ScaleM44(v3 Scale)
{
    m44 Result = ScaleM44(Scale.x, Scale.y, Scale.z);
    return Result;
}

static m44
ScaleM44(r32 Scale)
{
    m44 Result = m44{
        Scale, 0, 0, 0,
        0, Scale, 0, 0,
        0, 0, Scale, 0,
        0, 0, 0, 1,
    };
    return Result;
}

static m44
RotationXM44(float Angle)
{
    float CosAngle = GSCos(Angle);
    float SinAngle = GSSin(Angle);
    m44 M = {
        1, 0,         0,        0,
        0, CosAngle,  -SinAngle, 0,
        0, SinAngle,  CosAngle, 0,
        0, 0,         0,        1
    };
    return M;
}

static m44
RotationYM44(float Angle)
{
    float CosAngle = GSCos(Angle);
    float SinAngle = GSSin(Angle);
    m44 M = {
        CosAngle, 0,  SinAngle, 0,
        0,        1,  0,         0,
        -SinAngle, 0,  CosAngle,  0,
        0,        0,  0,         1
    };
    return M;
}

static m44
RotationZM44(float Angle)
{
    float CosAngle = GSCos(Angle);
    float SinAngle = GSSin(Angle);
    m44 M = {
        CosAngle,  -SinAngle, 0, 0,
        SinAngle, CosAngle, 0, 0,
        0,         0,        1, 0,
        0,         0,        0, 1
    };
    return M;
}

static m44
RotationM44(v3 Rotation)
{
    m44 Result = {};
    m44 X = RotationXM44(Rotation.x);
    m44 Y = RotationYM44(Rotation.z);
    m44 Z = RotationZM44(Rotation.z);
    Result = Z * Y * X;
    return Result;
}

static m44
Scale(m44 M, r32 Delta)
{
    m44 Result = ScaleM44(Delta) * M;
    return Result;
}

static m44
Translate(m44 M, v3 Delta)
{
    m44 Result = TranslationM44(Delta) * M;
    return Result;
}

static m44
Rotate(m44 M, v3 Delta)
{
    m44 Result = M;
    m44 X = RotationXM44(Delta.x);
    m44 Y = RotationYM44(Delta.z);
    m44 Z = RotationZM44(Delta.z);
    Result = Z * Y * X * Result;
    return Result;
}

static m44
GetModelViewMatrix (v4 Forward, v4 Right, v4 Up, v4 Position)
{
    m44 RotationMatrix = M44(Right.x, Up.x, Forward.x, 0,
                             Right.y, Up.y, Forward.y, 0,
                             Right.z, Up.z, Forward.z, 0,
                             0, 0, 0, 1);
    m44 PositionMatrix = M44(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             -Position.x, -Position.y, -Position.z, 1);
    m44 ModelViewMatrix = PositionMatrix * RotationMatrix;
    return ModelViewMatrix;
}

static m44
GetModelViewMatrix (v4 Forward, v4 Right, v4 Position)
{
    v4 Up = Normalize(Cross(Forward, Right));
    return GetModelViewMatrix(Forward, Right, Up, Position);
}


static m44
GetLookAtMatrix (v4 Position, v4 Target)
{
    v4 Forward = Normalize(Target - Position);
    v4 Right = Normalize(Cross(v4{0, 1, 0, 0}, Forward));
    v4 Up = Normalize(Cross(Forward, Right));
    m44 RotationMatrix = M44(
                             Right.x, Up.x, Forward.x, 0,
                             Right.y, Up.y, Forward.y, 0,
                             Right.z, Up.z, Forward.z, 0,
                             0,       0,    0,         1);
    
    return RotationMatrix;
}

static m44
OrthographicProjectionM44(r32 Width, r32 Height, r32 Near, r32 Far, r32 Right, r32 Left, r32 Top, r32 Bottom)
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

static m44
OrthographicProjectionM44(r32 Aspect, r32 Scale, r32 Near, r32 Far)
{
    m44 Result = {};
    
    r32 Width = Scale * Aspect;
    r32 Height = Scale;
    
    r32 Right = Width / 2.0f;
    r32 Left = -Right;
    r32 Top = Height / 2.0f;
    r32 Bottom = -Top;
    
    Result = OrthographicProjectionM44(Width, Height, Near, Far, Right, Left, Top, Bottom);
    
    return Result;
}

static m44
InterfaceOrthographicProjectionM44(r32 Width, r32 Height, r32 Near, r32 Far)
{
    m44 Result = {0};
    
    r32 Aspect = Width / Height;
    
    r32 Right = Width;
    r32 Left = 0;
    r32 Top = Height;
    r32 Bottom = 0;
    
    Result = OrthographicProjectionM44(Width, Height, Near, Far, Right, Left, Top, Bottom);
    
    return Result;
}

static m44
PerspectiveProjectionM44(r32 FOV, r32 AspectRatio, r32 Near, r32 Far)
{
    m44 Result = {};
    
    r32 TanHalfFOV = GSTan(Radians(FOV / 2));
    r32 F = 1 / TanHalfFOV;
    
    Result.E[0] = F / AspectRatio;
    Result.E[5] = F;
    Result.E[10] = (Far + Near) / (Near - Far);
    Result.E[11] = -1;
    Result.E[14] = (2 * Far * Near) / (Near - Far);
    
    return Result;
}

b32 Inverse(m44 M_In, m44* M_Out)
{
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

#if defined(VECTOR_MATRIX_TEST_SUITE)

void TestVectorMatrixMultiplication ()
{
    s32 TestCount = 0;
    s32 SuccessCount = 0;
    
    DebugPrint("\n\n-------------------------------------------------\n  Begin Testing Vector/Matrix\n\n\n");
    
    // Utility Functions
    TestClean((GSSqrt(4.f) == 2.f), "Vector Square Root");
    TestClean((GSLerp(0.f, 1.f, .5f) == .5f), "Vector Lerp");
    TestClean((GSMin(-.25f, 5.f) == -.25f), "Vector Min");
    TestClean((GSMax(-.25f, 5.f) == 5.f), "Vector Max");
    TestClean((GSClamp(-2.f, -3.f, 5.f) == -2.f), "Vector Clamp, Lower Than Range");
    TestClean((GSClamp(-2.f, 6.f, 5.f) == 5.f), "Vector Clamp, Higher Than Range");
    
    //////////////////////////////
    // Vector Functions
    /////////////////////////////
    
    v2 V2Unit = v2{1, 0};
    v3 V3Unit = v3{1, 0, 0};
    v4 V4Unit = v4{1, 0, 0, 0};
    
    v2 TestV2 = v2{1, 2};
    float TestV2MagSq = (TestV2.x * TestV2.x) + (TestV2.y * TestV2.y);
    float TestV2Mag = GSSqrt(TestV2MagSq);
    v2 TestV2Norm = v2{TestV2.x / TestV2Mag, TestV2.y / TestV2Mag};
    float TestV2DotR = (TestV2.x * V2Unit.x) + (TestV2.y * V2Unit.y);
    
    v3 TestV3 = v3{1, 2, 3};
    float TestV3MagSq = (TestV3.x * TestV3.x) + (TestV3.y * TestV3.y) + (TestV3.z * TestV3.z);
    float TestV3Mag = GSSqrt(TestV3MagSq);
    v3 TestV3Norm = v3{TestV3.x / TestV3Mag, TestV3.y / TestV3Mag, TestV3.z / TestV3Mag};
    float TestV3DotR = (TestV3.x * V3Unit.x) + (TestV3.y * V3Unit.y) + (TestV3.z * V3Unit.z);
    
    v4 TestV4 = v4{1, 2, 3, 4};
    float TestV4MagSq = (TestV4.x * TestV4.x) + (TestV4.y * TestV4.y) + (TestV4.z * TestV4.z) + (TestV4.w * TestV4.w);
    float TestV4Mag = GSSqrt(TestV4MagSq);
    v4 TestV4Norm = v4{
        TestV4.x / TestV4Mag, TestV4.y / TestV4Mag, TestV4.z / TestV4Mag, TestV4.w / TestV4Mag
    };
    float TestV4DotR = (TestV4.x * V4Unit.x) + (TestV4.y * V4Unit.y) + (TestV4.z * V4Unit.z) + (TestV4.w * V4Unit.w);
    
    v2 DownCastV3 = V2(TestV3);
    v3 DownCastV4 = V3(TestV4);
    
    v2 EqualityV2 = v2{TestV2.x, TestV2.y};
    v2 ZeroV2 = v2{0, 0};
    v3 EqualityV3 = v3{TestV3.x, TestV3.y, TestV3.z};
    v3 ZeroV3 = v3{0, 0, 0};
    v4 EqualityV4 = v4{TestV4.x, TestV4.y, TestV4.z, TestV4.w};
    v4 ZeroV4 = v4{0, 0, 0, 0};
    
    TestClean((TestV2.x == 1 && TestV2.y == 2), "V2 Assignment");
    TestClean((TestV3.x == 1 && TestV3.y == 2 && TestV3.z == 3), "V3 Assignment");
    TestClean((TestV4.x == 1 && TestV4.y == 2 && TestV4.z == 3 && TestV4.w == 4), "V3 Assignment");
    
    TestClean((DownCastV3.x == 1 && DownCastV3.y == 2), "V3 -> V2 Downcast");
    TestClean((DownCastV4.x == 1 && DownCastV4.y == 2 && DownCastV4.z == 3), "V4 -> V3 Downcast");
    
    // Vector Operators
    
    TestClean((TestV2 == EqualityV2 && !(TestV2 == ZeroV2)), "V2 Equality");
    TestClean((TestV3 == EqualityV3 && !(TestV3 == ZeroV3)), "V3 Equality");
    TestClean((TestV4 == EqualityV4 && !(TestV4 == ZeroV4)), "V4 Equality");
    
    TestClean(((TestV2 - TestV2) == ZeroV2), "V2 Subtraction");
    TestClean(((TestV3 - TestV3) == ZeroV3), "V3 Subtraction");
    TestClean(((TestV4 - TestV4) == ZeroV4), "V4 Subtraction");
    
    TestClean(((TestV2 + TestV2) == v2{TestV2.x * 2, TestV2.y * 2}), "V2 Addition");
    TestClean(((TestV3 + TestV3) == v3{TestV3.x * 2, TestV3.y * 2, TestV3.z * 2}), "V3 Addition");
    TestClean(((TestV4 + TestV4) == v4{TestV4.x * 2, TestV4.y * 2, TestV4.z * 2, TestV4.w * 2}), "V4 Addition");
    
    TestClean(((TestV2 * 2.0f) == v2{TestV2.x * 2, TestV2.y * 2}), "V2 Multiplication");
    TestClean(((TestV3 * 2.0f) == v3{TestV3.x * 2, TestV3.y * 2, TestV3.z * 2}), "V3 Multiplication");
    TestClean(((TestV4 * 2.0f) == v4{TestV4.x * 2, TestV4.y * 2, TestV4.z * 2, TestV4.w * 2}), "V4 Multiplication");
    
    TestClean(((TestV2 * TestV2) == v2{TestV2.x * TestV2.x, TestV2.y * TestV2.y}), "V2 Piecewise Mult");
    TestClean(((TestV3 * TestV3) == v3{
                   TestV3.x * TestV3.x,
                   TestV3.y * TestV3.y,
                   TestV3.z * TestV3.z}), "V3 Piecewise Mult");
    TestClean(((TestV4 * TestV4) == v4{
                   TestV4.x * TestV4.x,
                   TestV4.y * TestV4.y,
                   TestV4.z * TestV4.z,
                   TestV4.w * TestV4.w}), "V4 Piecewise Mult");
    
    
    TestClean(((TestV2 / 2.0f) == v2{TestV2.x / 2, TestV2.y / 2}), "V2 Division");
    TestClean(((TestV3 / 2.0f) == v3{TestV3.x / 2, TestV3.y / 2, TestV3.z / 2}), "V3 Division");
    TestClean(((TestV4 / 2.0f) == v4{TestV4.x / 2, TestV4.y / 2, TestV4.z / 2, TestV4.w / 2}), "V4 Division");
    
    TestClean(((TestV2 / TestV2) == v2{TestV2.x / TestV2.x, TestV2.y / TestV2.y}), "V2 Piecewise Div");
    TestClean(((TestV3 / TestV3) == v3{
                   TestV3.x / TestV3.x,
                   TestV3.y / TestV3.y,
                   TestV3.z / TestV3.z}), "V3 Piecewise Div");
    TestClean(((TestV4 / TestV4) == v4{
                   TestV4.x / TestV4.x,
                   TestV4.y / TestV4.y,
                   TestV4.z / TestV4.z,
                   TestV4.w / TestV4.w}), "V4 Piecewise Div");
    
    TestClean(((MagSqr(V2Unit) == 1) && (MagSqr(TestV2) == TestV2MagSq)), "V2 Square Mag");
    TestClean(((MagSqr(V3Unit) == 1) && (MagSqr(TestV3) == TestV3MagSq)), "V3 Square Mag");
    TestClean(((MagSqr(V4Unit) == 1) && (MagSqr(TestV4) == TestV4MagSq)), "V4 Square Mag");
    TestClean(((Mag(V2Unit) == 1) && (Mag(TestV2) == TestV2Mag)), "V2 Mag");
    TestClean(((Mag(V3Unit) == 1) && (Mag(TestV3) == TestV3Mag)), "V3 Mag");
    TestClean(((Mag(V4Unit) == 1) && (Mag(TestV4) == TestV4Mag)), "V4 Mag");
    
    TestClean((DistanceSq(ZeroV2, TestV2) == TestV2MagSq), "V2 Distance Sq");
    TestClean((DistanceSq(ZeroV3, TestV3) == TestV3MagSq), "V3 Distance Sq");
    TestClean((DistanceSq(ZeroV4, TestV4) == TestV4MagSq), "V4 Distance Sq");
    TestClean((Distance(ZeroV2, TestV2) == TestV2Mag), "V2 Distance");
    TestClean((Distance(ZeroV3, TestV3) == TestV3Mag), "V3 Distance");
    TestClean((Distance(ZeroV4, TestV4) == TestV4Mag), "V4 Distance");
    
    TestClean((Normalize(TestV2) == TestV2Norm), "V2 Normalize");
    TestClean((Normalize(TestV3) == TestV3Norm), "V3 Normalize");
    TestClean((Normalize(TestV4) == TestV4Norm), "V4 Normalize");
    
    TestClean(((Dot(V2Unit, V2Unit) == 1) && (Dot(TestV2, V2Unit) == TestV2DotR)), "V2 Dot");
    TestClean(((Dot(V3Unit, V3Unit) == 1) && (Dot(TestV3, V3Unit) == TestV3DotR)), "V3 Dot");
    TestClean(((Dot(V4Unit, V4Unit) == 1) && (Dot(TestV4, V4Unit) == TestV4DotR)), "V4 Dot");
    
    // Skipping Cross For Now
    
    TestClean((Lerp(v2{0, 0}, v2{1, 1}, .5f) == v2{.5f, .5f}), "V2 Lerp");
    TestClean((Lerp(v3{0, 0, 0}, v3{1, 1, 1}, .5f) == v3{.5f, .5f, .5f}), "V3 Lerp");
    TestClean((Lerp(v4{0, 0, 0, 0}, v4{1, 1, 1, 1}, .5f) == v4{.5f, .5f, .5f, .5f}), "V4 Lerp");
    
    /////////////////////////////
    //   Matrix
    ////////////////////////////
    
    m33 TestM33 = m33{
        0, 1, 2,
        3, 4, 5,
        6, 7, 8};
    
    m33 EqualityM33 = {};
    for (s32 i = 0; i < 16; i++) { EqualityM33.E[i] = TestM33.E[i]; }
    
    m33 TransposeM33 = m33{
        0, 3, 6,
        1, 4, 7,
        2, 5, 8};
    
    m33 IdentityM33 = m33{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1};
    
    m33 TestM33Squared = m33{
        15, 18, 21,
        42, 54, 66,
        69, 90, 111
    };
    
    m44 TestM44 = m44{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };
    
    m44 EqualityM44 = {};
    for (s32 i = 0; i < 16; i++) { EqualityM44.E[i] = TestM44.E[i]; }
    
    m44 TransposeM44 = m44{
        0, 4, 8, 12,
        1, 5, 9, 13,
        2, 6, 10, 14,
        3, 7, 11, 15
    };
    
    m44 IdentityM44 = m44{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    m44 TestM44Squared = m44{
        56, 62, 68, 74,
        152, 174, 196, 218,
        248, 286, 324, 362,
        344, 398, 452, 506,
    };
    
    TestClean(((IdentityM33 == IdentityM33) && (TestM33 == EqualityM33)), "M33 Equality");
    TestClean(((IdentityM44 == IdentityM44) && (TestM44 == EqualityM44)), "M44 Equality");
    
    TestClean(((Transpose(IdentityM33) == IdentityM33) &&
               (Transpose(TestM33) == TransposeM33)), "M33 Transpose");
    TestClean(((Transpose(IdentityM44) == IdentityM44) &&
               (Transpose(TestM44) == TransposeM44)), "M44 Transpose");
    
    TestClean(((TestM33 * IdentityM33) == TestM33), "M33 Identity Mult");
    TestClean(((TestM44 * IdentityM44) == TestM44), "M44 Identity Mult");
    TestClean(((TestM33 * TestM33) == TestM33Squared), "M33 Mult");
    TestClean(((TestM44 * TestM44) == TestM44Squared), "M44 Mult");
    
    
    // Useful Tests
    v4 Right = v4{1, 0, 0, 0};
    v4 Forward = v4{0, 0, 1, 0};
    v4 Up = v4{0, 1, 0, 0};
    v4 Left = v4{-1, 0, 0, 0};
    v4 Back = v4{0, 0, -1, 0};
    v4 Down = v4{0, -1, 0, 0};
    
    m44 NinetyDegreesAboutX = GetXRotation(M_PI / 2);
    v4 Rotated = NinetyDegreesAboutX * Forward;
    TestClean((Rotated == Up), "Rotation About X");
    
    m44 NinetyDegreesAboutY = GetYRotation(M_PI / 2);
    Rotated = NinetyDegreesAboutY * Right;
    TestClean((Rotated == Forward), "Rotation About Y");
    
    m44 NinetyDegreesAboutZ = GetZRotation(M_PI / 2);
    Rotated = NinetyDegreesAboutZ * Right;
    TestClean((Rotated == Down), "Rotation About Z");
    
    
    v4 A = v4{1, 2, 3, 4};
    m44 B = m44{
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 1, 2, 3,
        4, 5, 6, 7};
    v4 VTest = v4{30, 70, 29, 60};
    TestClean(((B * A) == VTest), "V4 M44 Multiplication");
    
    m44 C = m44{
        9, 8, 7, 6,
        5, 4, 3, 2,
        1, 0, 9, 8,
        7, 6, 5, 4
    };
    m44 MResult = B * C;
    m44 MTest = m44{
        50, 40, 60, 50,
        138, 112, 156, 130,
        109,  94,  99,  84,
        116, 94, 132, 110
    };
    TestClean(((B * C) == MTest), "M44 Mult Test 2");
    
    m44 Identity = M44Identity();
    m44 InvIdentity = {};
    Inverse(Identity, &InvIdentity);
    TestClean((Identity == InvIdentity), "Inverse Identity");
    
    m44 Test = m44{
        2, 4, 6, 7,
        5, 1, 8, 8,
        1, 7, 3, 1,
        3, 9, 2, 4
    };
    m44 PreCalcTestInv = m44{
        -0.3904761904761904762f, 0.26190476190476190475f, -0.02857142857142857139f, 0.16666666666666666668f,
        0.022222222222222222212f, -0.055555555555555555549f, 0.06666666666666666667f, 0.055555555555555555547f,
        -0.00317460317460317458f, 0.07936507936507936506f, 0.27619047619047619045f, -0.2222222222222222222f,
        0.24444444444444444444f, -0.1111111111111111111f, -0.26666666666666666667f, 0.1111111111111111111f
    };
    m44 InvTest = {};
    Inverse(Test, &InvTest);
    //TestClean((PreCalcTestInv == InvTest), "Inverse M44");
    
    DebugPrint("Results: Passed %d / %d\n\n\no", SuccessCount, TestCount);
}

#endif

#define GS_VECTOR_MATRIX_H
#endif