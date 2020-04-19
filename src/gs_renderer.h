//
// File: gs_renderer.h
// Author: Peter Slattery
// Creation Date: 2020-03-23
//
#ifndef GS_RENDERER_H

enum gsr_render_asset_type
{
    gsr_RenderAsset_Model,
    gsr_RenderAsset_FragmentShader,
    gsr_RenderAsset_VertexShader,
    gsr_RenderAsset_ShaderProgram,
    gsr_RenderAsset_Texture,
    gsr_RenderAsset_Count,
};

struct gsr_render_asset
{
    gsr_render_asset_type Type;
    u8* BackendAssetData;
};

struct gsr_render_asset_buffer
{
    gsr_render_asset* Assets;
};

struct gsr_render_asset_list
{
    gs_memory_arena Arena;
    gs_dynarray AssetHeaders;
};

#ifdef DEBUG
internal void
gsr_AssertAssetTypeEquals(gsr_render_asset_list Assets, gs_dynarray_handle Handle, gsr_render_asset_type Type)
{
    gsr_render_asset* Asset = GetElement(&Assets.AssetHeaders, gsr_render_asset, Handle);
    Assert(Asset->Type == Type);
}
#else
#define gsr_AssertAssetTypeEquals(assets, handle, type)
#endif

internal gsr_render_asset_list
CreateRenderAssetList(gs_allocator* Allocator)
{
    gsr_render_asset_list Result = {0};
    Result.Arena = CreateMemoryArena(Allocator);
    Result.AssetHeaders = CreateDynarray(Allocator, gsr_render_asset,512);
    return Result;
}

inline gsr_render_asset*
gsr_GetAsset(gsr_render_asset_list* Assets, gs_dynarray_handle Handle)
{
    gsr_render_asset* Result = GetElement(&Assets->AssetHeaders, gsr_render_asset, Handle);
    return Result;
}

static gs_dynarray_handle
gsr_TakeAssetHandle(gsr_render_asset_list* Assets, gsr_render_asset_type Type)
{
    gs_dynarray_handle Result = TakeFreeElement(&Assets->AssetHeaders);
    return Result;
}

#define GS_RENDERER_H
#endif // GS_RENDERER_H