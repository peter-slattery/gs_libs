//
// File: gs_extensions.cpp
// Author: Peter Slattery
// Creation Date: 2020-04-20
//
// gs_extensions.h and gs_extensions.cpp are both files which contain data structures and
// procedures which make working with multiple gs_libs easier. They facilitate translating
// from one library to another.
//
// You can include the gs_extensions files after all present gs_lib headers are included and they
// will only load in the functionality relevant to the libs you have included.
// ie if gs_renderer.h is not included, any functionality relevant to extending the renderer
// will not be included
// NOTE: You should not need to include the cpp files for these to work

#ifndef GS_EXTENSIONS_CPP

#if defined(GS_RENDERER_H) && defined(GS_ASSET_SYSTEM_H)

internal gs_dynarray_handle
ModelAssetToRenderAsset(gsr_render_queue* RenderQueue, gsa_model* Model, gsr_vertex_attribute* Attributes)
{
    gs_dynarray_handle Result = gsr_CreateModel(RenderQueue, Attributes);
    gsr_SetModelData(RenderQueue,
                     Result,
                     Model->VertexBufferSize,
                     Model->VertexBuffer,
                     Model->IndexBufferSize,
                     Model->IndexBuffer,
                     Model->VertexStride,
                     Model->PositionOffset,
                     Model->UVOffset,
                     Model->NormalOffset);
    return Result;
}

internal gsr_render_asset_handle
LoadShader(gsa_asset_system* Assets, gsr_render_queue* RenderQueue, gs_const_string VertPath, gs_const_string FragPath)
{
    gsr_render_asset_handle Result = {0};
    
    gsa_text* VertCode = gsa_GetAsset(Assets, VertPath, gsa_text);
    gsa_text* FragCode = gsa_GetAsset(Assets, FragPath, gsa_text);
    Assert(VertCode && FragCode);
    
    Result = gsr_CreateShader(RenderQueue);
    gsr_CompileShader(RenderQueue, Result, VertCode->Memory, FragCode->Memory);
    
    return Result;
}

internal gs_dynarray_handle
LoadModel(gsa_asset_system* Assets, gsr_render_queue* RenderQueue, gs_const_string Path, gsr_vertex_attribute* Attributes)
{
    gs_dynarray_handle Result = {0};
    gsa_model* Model = gsa_GetAsset(Assets, Path, gsa_model);
    Result = ModelAssetToRenderAsset(RenderQueue, Model, Attributes);
    return Result;
}

internal gs_dynarray_handle
LoadTexture(gsa_asset_system* Assets, gsr_render_queue* RenderQueue, gs_const_string Path)
{
    gs_dynarray_handle Result = {0};
    gsa_image* Asset = gsa_GetAsset(Assets, Path, gsa_image);
    Result = gsr_CreateTexture(RenderQueue);
    gsr_SetTextureData(RenderQueue, Result, Asset->Data, Asset->Width, Asset->Height, Asset->Channels);
    return Result;
}

#endif // defined(GS_RENDERER_H) && defined(GS_ASSET_SYSTEM_H)

#if defined(GS_RENDERER_H) && defined(GS_RENDER_QUEUE_H) && defined(GS_MODEL_BUILDER_H)

#if 0
internal model_builder
CreateModelBuilderFromRenderAsset(gs_render_asset_list* Assets, gsr_render_asset_handle AssetHandle)
{
    model_builder Result = {0};
    
    gsr_render_asset* Asset = gsr_GetAsset(Assets, AssetHandle);
    Assert(Asset->Type == gsr_RenderAsset_Model);
    
    InitializeModelBuilder(&Result, Assets->Arena.Allocator);
    Result.VertexStride = Asset->Model.VertexStride;
    Result.VertexAttributeCount = Asset->Model.VertexAttributeCount;
    Result.Ver
        for (u32 i = 0; i < gsr_ModelVertexAttributesCount(Asset); i++)
    {
    }
    
    return Result;
}
internal model_builder
CreateModelBuilderFromRenderAsset(gs_render_queue* RenderQueue, gsr_render_asset_handle AssetHandle)
{
    return CreateModelBuilderFromRenderAsset(RenderQueue->Assets, AssetHandle);
}
internal model_builder
CreateModelBuilderFromRenderAsset(gs_renderer* Renderer, gsr_render_asset_handle AssetHandle)
{
    return CreateModelBuilderFromRenderAsset(Renderer->Assets, AssetHandle);
}
#endif

#endif

#define GS_EXTENSIONS_CPP
#endif // GS_EXTENSIONS_CPP