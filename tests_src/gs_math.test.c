#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#define ASSERTS_ACTIVE 1
#include "../src/gs_assert.h"
#include "../src/gs_math.h"

internal void
GlobalTestM33(m33 A, m33 B)
{
  GlobalTest(M33AreEqual(A, B));
}

internal void
GlobalTestM44(m44 A, m44 B)
{
  GlobalTest(M44AreEqual(A, B));
}

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  TestGroup("Math Functions")
  {
    GlobalTest(XOR(true, false));
    GlobalTest(!XOR(true, true));
    
    GlobalTest(FloorR32(30.33125f) == 30);
    r32 F = FractR32(30.33125f);
    r32 R = Abs(F - 0.33125f);
    GlobalTest(R <  0.0001f);
  }
  
  TestGroup("Rounding")
  {
    u32 V0 = 250;
    GlobalTest(RoundUpToPow2U32(V0) == 256);
    
    u64 V1 = 65530;
    GlobalTest(RoundUpToPow2U64(V1) == 65536);
    
    // 10 / 4 = 2.5, rounded up = 3
    GlobalTest(U32DivideRoundUp(10, 4) == 3);
  }
  
  TestGroup("Vectors - Equality")
  {
    GlobalTest(V2Equals((v2){1, 2}, (v2){1, 2}));
    GlobalTest(V3Equals((v3){1, 2, 3}, (v3){1, 2, 3}));
    GlobalTest(V4Equals((v4){1, 2, 3, 4}, (v4){1, 2, 3, 4}));
    
    GlobalTest(!V2Equals((v2){1, 2}, (v2){1, 1}));
    GlobalTest(!V2Equals((v2){1, 2}, (v2){2, 2}));
    GlobalTest(!V3Equals((v3){1, 2, 3}, (v3){4, 2, 3}));
    GlobalTest(!V3Equals((v3){1, 2, 3}, (v3){1, 4, 3}));
    GlobalTest(!V3Equals((v3){1, 2, 3}, (v3){1, 2, 4}));
    GlobalTest(!V4Equals((v4){1, 2, 3, 4}, (v4){5, 2, 3, 4}));
    GlobalTest(!V4Equals((v4){1, 2, 3, 4}, (v4){1, 5, 3, 4}));
    GlobalTest(!V4Equals((v4){1, 2, 3, 4}, (v4){1, 2, 5, 4}));
    GlobalTest(!V4Equals((v4){1, 2, 3, 4}, (v4){1, 2, 3, 5}));
  }
  
  TestGroup("Vectors - Negate")
  {
    GlobalTest(V2Equals(V2Negate((v2){1, 1}), (v2){-1, -1}));
    GlobalTest(V3Equals(V3Negate((v3){1, 1, 1}), (v3){-1, -1, -1}));
    GlobalTest(V4Equals(V4Negate((v4){1, 1, 1, 1}), (v4){-1, -1, -1, -1}));
  }
  
  TestGroup("Vectors - Add, Sub, Scale")
  {
    // Addition
    //
    v2 V2AddResult = V2Add((v2){1, 3}, (v2){1, 1});
    GlobalTest(V2Equals(V2AddResult, (v2){2, 4}));
    
    v3 V3AddResult = V3Add((v3){1, 3, 5}, (v3){1, 1, 1});
    GlobalTest(V3Equals(V3AddResult, (v3){2, 4, 6}));
    
    v4 V4AddResult = V4Add((v4){1, 3, 5, 7}, (v4){1, 1, 1, 1});
    GlobalTest(V4Equals(V4AddResult, (v4){2, 4, 6, 8}));
    
    // Subtraction
    //
    v2 V2SubResult = V2Sub((v2){1, 3}, (v2){1, 1});
    GlobalTest(V2Equals(V2SubResult, (v2){0, 2}));
    
    v3 V3SubResult = V3Sub((v3){1, 3, 5}, (v3){1, 1, 1});
    GlobalTest(V3Equals(V3SubResult, (v3){0, 2, 4}));
    
    v4 V4SubResult = V4Sub((v4){1, 3, 5, 7}, (v4){1, 1, 1, 1});
    GlobalTest(V4Equals(V4SubResult, (v4){0, 2, 4, 6}));
    
    // Scale
    //
    v2 V2ScaleResult = V2Scale((v2){1, 3}, 2);
    GlobalTest(V2Equals(V2ScaleResult, (v2){2, 6}));
    
    v3 V3ScaleResult = V3Scale((v3){1, 3, 5}, 2);
    GlobalTest(V3Equals(V3ScaleResult, (v3){2, 6, 10}));
    
    v4 V4ScaleResult = V4Scale((v4){1, 3, 5, 7}, 2);
    GlobalTest(V4Equals(V4ScaleResult, (v4){2, 6, 10, 14}));
  }
  
  TestGroup("Vectors - Pairwise Multiply")
  {
    v2 V2MulResult = V2MultiplyPairwise((v2){1, 3}, (v2){2, 2});
    GlobalTest(V2Equals(V2MulResult, (v2){2, 6}));
    
    v3 V3MulResult = V3MultiplyPairwise((v3){1, 3, 5}, (v3){2, 2, 2});
    GlobalTest(V3Equals(V3MulResult, (v3){2, 6, 10}));
    
    v4 V4MulResult = V4MultiplyPairwise((v4){1, 3, 5, 7}, (v4){2, 2, 2, 2});
    GlobalTest(V4Equals(V4MulResult, (v4){2, 6, 10, 14}));
  }
  
  TestGroup("Vectors - Unit Vector Mag")
  {
    GlobalTest(V2Mag((v2){1,0}) == 1);
    GlobalTest(V2Mag((v2){0,1}) == 1);
    GlobalTest(V3Mag((v3){1,0,0}) == 1);
    GlobalTest(V3Mag((v3){0,1,0}) == 1);
    GlobalTest(V3Mag((v3){0,0,1}) == 1);
    GlobalTest(V4Mag((v4){1,0,0,0}) == 1);
    GlobalTest(V4Mag((v4){0,1,0,0}) == 1);
    GlobalTest(V4Mag((v4){0,0,1,0}) == 1);
    GlobalTest(V4Mag((v4){0,0,0,1}) == 1);
  }
  
  TestGroup("Vectors - V2 - Mag")
  {
    v2 V2Zero = (v2){0,0};
    v2 TestV2 = (v2){1, 2};
    r32 TestV2MagSq = (TestV2.x * TestV2.x) + (TestV2.y * TestV2.y);
    r32 TestV2Mag = SqrtR32(TestV2MagSq);
    GlobalTest(V2MagSquared(TestV2) == TestV2MagSq);
    GlobalTest(V2Mag(TestV2) == TestV2Mag);
    
    GlobalTest(V2DistanceSquared(TestV2, TestV2) == 0);
    GlobalTest(V2Distance(TestV2, TestV2) == 0);
    GlobalTest(V2DistanceSquared(TestV2, V2Zero) == TestV2MagSq);
    GlobalTest(V2DistanceSquared(V2Zero, TestV2) == TestV2MagSq);
    GlobalTest(V2Distance(TestV2, V2Zero) == TestV2Mag);
    GlobalTest(V2Distance(V2Zero, TestV2) == TestV2Mag);
  }
  
  TestGroup("Vectors - V3 - Mag")
  {
    v3 V3Zero = (v3){0,0,0};
    v3 TestV3 = (v3){1,2,3};
    r32 TestV3MagSq = (TestV3.x * TestV3.x) + (TestV3.y * TestV3.y) + (TestV3.z * TestV3.z);
    r32 TestV3Mag = SqrtR32(TestV3MagSq);
    GlobalTest(V3MagSquared(TestV3) == TestV3MagSq);
    GlobalTest(V3Mag(TestV3) == TestV3Mag);
    
    GlobalTest(V3DistanceSquared(TestV3, TestV3) == 0);
    GlobalTest(V3Distance(TestV3, TestV3) == 0);
    GlobalTest(V3DistanceSquared(TestV3, V3Zero) == TestV3MagSq);
    GlobalTest(V3DistanceSquared(V3Zero, TestV3) == TestV3MagSq);
    GlobalTest(V3Distance(TestV3, V3Zero) == TestV3Mag);
    GlobalTest(V3Distance(V3Zero, TestV3) == TestV3Mag);
  }
  
  TestGroup("Vectors - V4 - Mag")
  {
    v4 V4Zero = (v4){0,0,0,0};
    v4 TestV4 = (v4){1,2,3,4};
    r32 TestV4MagSq = (TestV4.x * TestV4.x) + (TestV4.y * TestV4.y) + (TestV4.z * TestV4.z) + (TestV4.w * TestV4.w);
    r32 TestV4Mag = SqrtR32(TestV4MagSq);
    GlobalTest(V4MagSquared(TestV4) == TestV4MagSq);
    GlobalTest(V4Mag(TestV4) == TestV4Mag);
    
    GlobalTest(V4DistanceSquared(TestV4, TestV4) == 0);
    GlobalTest(V4Distance(TestV4, TestV4) == 0);
    GlobalTest(V4DistanceSquared(TestV4, V4Zero) == TestV4MagSq);
    GlobalTest(V4DistanceSquared(V4Zero, TestV4) == TestV4MagSq);
    GlobalTest(V4Distance(TestV4, V4Zero) == TestV4Mag);
    GlobalTest(V4Distance(V4Zero, TestV4) == TestV4Mag);
  }
  
  TestGroup("Vectors - Normalize")
  {
    v2 TestV2 = (v2){1,2};
    r32 TestV2Mag = V2Mag(TestV2);
    v2 TestV2Norm = V2Scale(TestV2, 1/TestV2Mag);
    GlobalTest(V2Equals(V2Normalize(TestV2), TestV2Norm));
    
    v3 TestV3 = (v3){1,2,3};
    r32 TestV3Mag = V3Mag(TestV3);
    v3 TestV3Norm = V3Scale(TestV3, 1/TestV3Mag);
    GlobalTest(V3Equals(V3Normalize(TestV3), TestV3Norm));
    
    v4 TestV4 = (v4){1,2,3,4};
    r32 TestV4Mag = V4Mag(TestV4);
    v4 TestV4Norm = V4Scale(TestV4, 1/TestV4Mag);
    GlobalTest(V4Equals(V4Normalize(TestV4), TestV4Norm));
  }
  
  TestGroup("Vectors - Normalize")
  {
    v2 V2Unit = (v2){1, 0};
    v2 TestV2 = (v2){1,2};
    float TestV2DotR = ((TestV2.x * V2Unit.x) + 
                        (TestV2.y * V2Unit.y));
    GlobalTest(V2Dot(TestV2, V2Unit) == TestV2DotR);
    GlobalTest(V2Dot(V2Unit, TestV2) == TestV2DotR);
    
    v3 V3Unit = (v3){1, 0, 0};
    v3 TestV3 = (v3){1,2,3};
    float TestV3DotR = ((TestV3.x * V3Unit.x) + 
                        (TestV3.y * V3Unit.y) + 
                        (TestV3.z * V3Unit.z));
    GlobalTest(V3Dot(TestV3, V3Unit) == TestV3DotR);
    GlobalTest(V3Dot(V3Unit, TestV3) == TestV3DotR);
    
    v4 V4Unit = (v4){1, 0, 0, 0};
    v4 TestV4 = (v4){1,2,3,4};
    float TestV4DotR = ((TestV4.x * V4Unit.x) + 
                        (TestV4.y * V4Unit.y) + 
                        (TestV4.z * V4Unit.z) + 
                        (TestV4.w * V4Unit.w));
    GlobalTest(V4Dot(TestV4, V4Unit) == TestV4DotR);
    GlobalTest(V4Dot(V4Unit, TestV4) == TestV4DotR);
  }
  
  TestGroup("Vectors - Lerp")
  {
    v2 V2A = (v2){0, 0};
    v2 V2B = (v2){1, 1};
    v2 V2R = (v2){.5f, .5f};
    GlobalTest(V2Equals(V2Lerp(.5f, V2A, V2B), V2R));
    
    v3 V3A = (v3){0, 0, 0};
    v3 V3B = (v3){1, 1, 1};
    v3 V3R = (v3){.5f, .5f, .5f};
    GlobalTest(V3Equals(V3Lerp(.5f, V3A, V3B), V3R));
    
    v4 V4A = (v4){0, 0, 0, 0};
    v4 V4B = (v4){1, 1, 1, 1};
    v4 V4R = (v4){.5f, .5f, .5f, .5f};
    GlobalTest(V4Equals(V4Lerp(.5f, V4A, V4B), V4R));
  }
  
  TestGroup("Matrix")
  {
    m33 TestM33 = (m33){
      0, 1, 2,
      3, 4, 5,
      6, 7, 8
    };
    m33 EqualityM33 = TestM33;
    
    m33 TransposeM33 = (m33){
      0, 3, 6,
      1, 4, 7,
      2, 5, 8};
    
    m33 IdentityM33 = (m33){
      1, 0, 0,
      0, 1, 0,
      0, 0, 1};
    
    m33 TestM33Squared = (m33){
      15, 18, 21,
      42, 54, 66,
      69, 90, 111
    };
    
    m44 TestM44 = (m44){
      0, 1, 2, 3,
      4, 5, 6, 7,
      8, 9, 10, 11,
      12, 13, 14, 15
    };
    
    m44 EqualityM44 = TestM44;
    
    m44 TransposeM44 = (m44){
      0, 4, 8, 12,
      1, 5, 9, 13,
      2, 6, 10, 14,
      3, 7, 11, 15
    };
    
    m44 IdentityM44 = (m44){
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };
    
    m44 TestM44Squared = (m44){
      56, 62, 68, 74,
      152, 174, 196, 218,
      248, 286, 324, 362,
      344, 398, 452, 506,
    };
    
    GlobalTestM33(IdentityM33, IdentityM33);
    GlobalTestM33(EqualityM33, TestM33);
    GlobalTestM33(TestM33, EqualityM33);
    GlobalTestM44(IdentityM44, IdentityM44);
    GlobalTestM44(EqualityM44, TestM44);
    GlobalTestM44(TestM44, EqualityM44);
    
    GlobalTestM33(M33Transpose(IdentityM33), IdentityM33);
    GlobalTestM33(M33Transpose(TestM33), TransposeM33);
    GlobalTestM44(M44Transpose(IdentityM44), IdentityM44);
    GlobalTestM44(M44Transpose(TestM44), TransposeM44);
    
    // MatrixA * Identity == MatrixA
    GlobalTestM33(M33MultiplyM33(TestM33, IdentityM33), TestM33);
    GlobalTestM44(M44MultiplyM44(TestM44, IdentityM44), TestM44);
    
    GlobalTestM33(M33MultiplyM33(TestM33, TestM33), TestM33Squared);
    GlobalTestM44(M44MultiplyM44(TestM44, TestM44), TestM44Squared);
    
    // Useful Tests
    v4 Right = (v4){1, 0, 0, 0};
    v4 Forward = (v4){0, 0, 1, 0};
    v4 Up = (v4){0, 1, 0, 0};
    v4 Left = (v4){-1, 0, 0, 0};
    v4 Back = (v4){0, 0, -1, 0};
    v4 Down = (v4){0, -1, 0, 0};
    
    m44 NinetyDegreesAboutX = M44RotationX(PiR32 / 2);
    v4 Rotated = M44MultiplyV4(NinetyDegreesAboutX, Forward);
    GlobalTest(V4Mag(V4Sub(Rotated, Up)) < 0.0001f);
    
    m44 NinetyDegreesAboutY = M44RotationY(PiR32 / 2);
    Rotated = M44MultiplyV4(NinetyDegreesAboutY, Forward);
    GlobalTest(V4Mag(V4Sub(Rotated, Right)) < 0.0001f);
    
    m44 NinetyDegreesAboutZ = M44RotationZ(PiR32 / 2);
    Rotated = M44MultiplyV4(NinetyDegreesAboutZ, Forward);
    GlobalTest(V4Mag(V4Sub(Rotated, Forward)) < 0.0001f);
    
    v4 A = (v4){1, 2, 3, 4};
    m44 B = (m44){
      1, 2, 3, 4,
      5, 6, 7, 8,
      9, 1, 2, 3,
      4, 5, 6, 7};
    v4 VTest = (v4){30, 70, 29, 60};
    v4 VTestR = M44MultiplyV4(B, A);
    GlobalTest(V4Equals(VTestR, VTest));
    
    m44 C = (m44){
      9, 8, 7, 6,
      5, 4, 3, 2,
      1, 0, 9, 8,
      7, 6, 5, 4
    };
    m44 MTest = (m44){
      50, 40, 60, 50,
      138, 112, 156, 130,
      109,  94,  99,  84,
      116, 94, 132, 110
    };
    m44 MTestR = M44MultiplyM44(B, C);
    GlobalTestM44(MTestR, MTest);
    
    m44 Identity = M44Identity();
    m44 InvIdentity = {};
    M44Inverse(Identity, &InvIdentity);
    GlobalTestM44(Identity, InvIdentity);
    
    m44 Test = (m44){
      2, 4, 6, 7,
      5, 1, 8, 8,
      1, 7, 3, 1,
      3, 9, 2, 4
    };
    m44 PreCalcTestInv = (m44){
      -0.3904761904761904762f, 0.26190476190476190475f, -0.02857142857142857139f, 0.16666666666666666668f,
      0.022222222222222222212f, -0.055555555555555555549f, 0.06666666666666666667f, 0.055555555555555555547f,
      -0.00317460317460317458f, 0.07936507936507936506f, 0.27619047619047619045f, -0.2222222222222222222f,
      0.24444444444444444444f, -0.1111111111111111111f, -0.26666666666666666667f, 0.1111111111111111111f
    };
    m44 InvTest = {};
    M44Inverse(Test, &InvTest);
    //GlobalTest((PreCalcTestInv == InvTest), "Inverse M44");
    
  }
}



