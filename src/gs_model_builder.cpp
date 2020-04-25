//
// File: gs_model_builder.cpp
// Author: Peter Slattery
// Creation Date: 2020-04-05
//
#ifndef GS_MODEL_BUILDER_CPP

internal void
InitializeModelBuilder(model_builder* Builder, gs_allocator* Allocator)
{
    Assert(Builder->VertexStride != 0);
    Builder->Verteces = CreateDynarray_(Allocator, Builder->VertexStride, 256);
    Builder->Indices = CreateDynarray(Allocator, tri_indices, 256);
}

internal void
ClearModelBuilder(model_builder* Builder)
{
    Builder->Verteces.ElementCount = 0;
    Builder->Indices.ElementCount = 0;
}

internal void
CleanupModelBuilder(model_builder* Builder)
{
    FreeDynarray(&Builder->Verteces);
    FreeDynarray(&Builder->Indices);
}

internal r32*
GetVertexData(model_builder Builder, gs_dynarray_handle VertexHandle)
{
    r32* Result = (r32*)GetElementInList_(&Builder.Verteces, VertexHandle, Builder.VertexStride).Memory;
    return Result;
}

internal r32*
GetVertexData(model_builder Builder, u64 Index)
{
    gs_dynarray_handle Handle = IndexToHandle(&Builder.Verteces, Index);
    r32* Result = GetVertexData(Builder, Handle);
    return Result;
}

internal tri_indices*
GetTriData(model_builder Builder, gs_dynarray_handle TriHandle)
{
    tri_indices* Result = GetElement(&Builder.Indices, tri_indices, TriHandle);
    return Result;
}

internal tri_indices*
GetTriData(model_builder Builder, u64 Index)
{
    gs_dynarray_handle Handle = IndexToHandle(&Builder.Indices, Index);
    tri_indices* Result = GetTriData(Builder, Handle);
    return Result;
}

internal void
SetVertexData(r32* Data, v3 Position, v2 UV, v3 Normal)
{
    Data[0] = Position.X;
    Data[1] = Position.Y;
    Data[2] = Position.Z;
    
    Data[3] = UV.X;
    Data[4] = UV.Y;
    
    Data[5] = Normal.X;
    Data[6] = Normal.Y;
    Data[7] = Normal.Z;
}

internal gs_dynarray_handle
PushVertex(model_builder* Builder, v3 Position, v2 UV = v2{0}, v3 Normal = v3{0})
{
    gs_dynarray_handle Result = TakeFreeElement(&Builder->Verteces);
    r32* VertexData = GetVertexData(*Builder, Result);
    SetVertexData(VertexData, Position, UV, Normal);
    return Result;
}

internal gs_dynarray_handle
PushTri(model_builder* Builder, gs_dynarray_handle V0, gs_dynarray_handle V1, gs_dynarray_handle V2)
{
    gs_dynarray_handle Result = TakeFreeElement(&Builder->Indices);
    tri_indices* Tri = GetTriData(*Builder, Result);
    
    Tri->E[0] = V0;
    Tri->E[1] = V1;
    Tri->E[2] = V2;
    
    return Result;
}

internal u32
SetTriVertex(model_builder* Builder, u32 Tri, u32 Vertex, v3 Position, v2 UV, v3 Normal)
{
    Assert(IndexIsValid(Builder->Indices, Tri));
    Assert(Vertex < 3);
    
    tri_indices* IndexData = GetTriData(*Builder, Tri);
    gs_dynarray_handle VertexIndex = IndexData->E[Vertex];
    
    r32* VertexData = GetVertexData(*Builder, VertexIndex);
    SetVertexData(VertexData, Position, UV, Normal);
}

//
// 2D Drawing Functions
//

internal void
PushLine2D(model_builder* Builder, v2 P0, v2 P1, r32 Thickness, r32 Depth)
{
    v2 Dim = V2Normalize(V2PerpendicularCW(P1 - P0)) * Thickness;
    
    gs_dynarray_handle V0 = PushVertex(Builder, ToV3(P0, Depth), v2{0, 0});
    gs_dynarray_handle V1 = PushVertex(Builder, ToV3(P0 + Dim, Depth), v2{1, 0});
    gs_dynarray_handle V2 = PushVertex(Builder, ToV3(P1 + Dim, Depth), v2{1, 1});
    gs_dynarray_handle V3 = PushVertex(Builder, ToV3(P1, Depth), v2{0, 1});
    
    gs_dynarray_handle Tri0 = PushTri(Builder, V0, V1, V2);
    gs_dynarray_handle Tri1 = PushTri(Builder, V0, V2, V3);
}

internal void
PushLine2D(model_builder* Builder, r32 Thickness, u32 PointsCount, v2* Points, r32 Depth)
{
    for (u32 Seg = 0; Seg < PointsCount - 1; Seg++)
    {
        v2 P0 = Points[Seg];
        v2 P1 = Points[Seg + 1];
        PushLine2D(Builder, P0, P1, Thickness, Depth);
    }
}

internal void
PushCircle2D(model_builder* Builder, v2 Center, r32 Radius, r32 Thickness, r32 Depth, u32 PointsCount = 25)
{
    for (u32 Point = 0; Point < PointsCount; Point++)
    {
        r32 Theta0 = ((r32)Point / (PointsCount - 1)) * TauR32;
        v2 P0 = Center + (v2{ CosR32(Theta0), SinR32(Theta0) } * Radius);
        
        r32 Theta1 = ((r32)(Point + 1) / (PointsCount - 1)) * TauR32;
        v2 P1 = Center + (v2{ CosR32(Theta1), SinR32(Theta1) } * Radius);
        
        PushLine2D(Builder, P0, P1, Thickness, Depth);
    }
}

internal void
PushBox2D(model_builder* Builder, v2 P0, v2 P1, v2 P2, v2 P3, r32 Depth, v2 UV0, v2 UV1, v2 UV2, v2 UV3)
{
    gs_dynarray_handle V0 = PushVertex(Builder, ToV3(P0, Depth), UV0);
    gs_dynarray_handle V1 = PushVertex(Builder, ToV3(P1, Depth), UV1);
    gs_dynarray_handle V2 = PushVertex(Builder, ToV3(P2, Depth), UV2);
    gs_dynarray_handle V3 = PushVertex(Builder, ToV3(P3, Depth), UV3);
    
    gs_dynarray_handle Tri0 = PushTri(Builder, V0, V1, V2);
    gs_dynarray_handle Tri1 = PushTri(Builder, V0, V2, V3);
}

internal void
PushBox2D(model_builder* Builder, rect2 Rect, r32 Depth, v2 UVMin = {0, 0}, v2 UVMax = {1, 1})
{
    v2 P0 = Rect.Min;
    v2 P1 = v2{ Rect.Max.X, Rect.Min.Y };
    v2 P2 = Rect.Max;
    v2 P3 = v2{ Rect.Min.X, Rect.Max.Y };
    
    v2 UV0 = UVMin;
    v2 UV1 = v2{ UVMax.X, UVMin.Y };
    v2 UV2 = UVMax;
    v2 UV3 = v2{ UVMin.X, UVMax.X };
    PushBox2D(Builder, P0, P1, P2, P3, Depth, UV0, UV1, UV2, UV3);
}

internal void
PushBoundingBox2D(model_builder* Builder, v2 P0, v2 P1, v2 P2, v2 P3, r32 Thickness, r32 Depth)
{
    PushLine2D(Builder, P0, P1, Thickness, Depth);
    PushLine2D(Builder, P1, P2, Thickness, Depth);
    PushLine2D(Builder, P2, P3, Thickness, Depth);
    PushLine2D(Builder, P3, P0, Thickness, Depth);
}

internal void
PushBoundingBox2D(model_builder* Builder, rect2 Rect, r32 Thickness, r32 Depth)
{
    v2 OuterP0 = Rect.Min;
    v2 OuterP1 = v2{ Rect.Max.X, Rect.Min.Y };
    v2 OuterP2 = Rect.Max;
    v2 OuterP3 = v2{ Rect.Min.X, Rect.Max.Y };
    
    PushBoundingBox2D(Builder, OuterP0, OuterP1, OuterP2, OuterP3, Thickness, Depth);
}

//
// Copy Data Out
//

internal void
CopyVertexDataToContiguousBuffer(model_builder Builder, u8* Buffer, u32 BufferSize)
{
    gs_dynarray Verteces = Builder.Verteces;
    Assert(BufferSize >= Verteces.ElementSize * Verteces.ElementCount);
    
    u8* DestAt = Buffer;
    for (u64 V = 0; V < Verteces.ElementCount; V++)
    {
        Assert(((DestAt + Builder.VertexStride) - Buffer) <= BufferSize);
        
        r32* VertexData = GetVertexData(Builder, V);
        CopyMemory_((u8*)VertexData, DestAt, Verteces.ElementSize);
        DestAt += Builder.VertexStride;
    }
}

internal void
CopyIndexDataToContiguousBuffer(model_builder Builder, u8* Buffer, u32 BufferSize)
{
    gs_dynarray Indices = Builder.Indices;
    u32 DestElementStride = sizeof(u32) * 3;
    Assert(BufferSize >= DestElementStride * Indices.ElementCount);
    
    u8* DestAt = Buffer;
    for (u64 Tri = 0; Tri < Indices.ElementCount; Tri++)
    {
        Assert(((DestAt + DestElementStride) - Buffer) <= BufferSize);
        tri_indices* TriData = GetTriData(Builder, Tri);
        
        u32* DestIndices = (u32*)DestAt;
        DestIndices[0] = HandleToIndex(Builder.Indices, TriData->E[0]);
        DestIndices[1] = HandleToIndex(Builder.Indices, TriData->E[1]);
        DestIndices[2] = HandleToIndex(Builder.Indices, TriData->E[2]);
        
        DestAt += DestElementStride;
    }
}



#define GS_MODEL_BUILDER_CPP
#endif // GS_MODEL_BUILDER_CPP