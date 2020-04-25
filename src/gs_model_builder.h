//
// File: gs_model_builder.h
// Author: Peter Slattery
// Creation Date: 2020-04-20
//
#ifndef GS_MODEL_BUILDER_H

union tri_indices
{
    struct
    {
        gs_dynarray_handle Index0;
        gs_dynarray_handle Index1;
        gs_dynarray_handle Index2;
    };
    gs_dynarray_handle E[3];
};

struct model_builder
{
    gs_dynarray Verteces;
    gs_dynarray Indices;
    
    u32 VertexStride;
    u32 PositionOffset;
    u32 UVOffset;
    u32 NormalOffset;
};



#define GS_MODEL_BUILDER_H
#endif // GS_MODEL_BUILDER_H