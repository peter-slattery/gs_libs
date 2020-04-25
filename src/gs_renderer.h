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
    gsr_RenderAsset_gsr_vertex_buffer,
    gsr_RenderAsset_Count,
};

typedef gs_dynarray_handle gsr_render_asset_handle;

struct gsr_vertex_attribute
{
    char* Name;
    
    gs_basic_type ElementType;
    // This is the number of elements per entry. ie. a v3 is ElementType = Float32, ElementCount = 3
    u32 ElementCount;
    
    s64 Offset;
    b32 Normalize;
};

#define VertexAttributeSize(attr) (BasicTypeSize((attr).ElementType) * (attr).ElementCount)

struct gsr_vertex_buffer_buffer
{
    u32 VertexStride;
    
    u32 VertsCount;
    u8* Verts;
    
    u32 IndicesCount;
    u32* Indices;
    
    gsr_vertex_buffer_buffer* Next;
};

struct gsr_vertex_buffer
{
    gsr_render_asset_handle ModelHandle;
    gsr_vertex_buffer_buffer* BuffersRoot;
    gsr_vertex_buffer_buffer* BuffersHead;
    u32 TotalVertsCount;
    u32 TotalIndicesCount;
};

struct gsr_model
{
    u32 VertexCount;
    u32 VertexStride;
    u32 VertexAttributesCount;
    gsr_vertex_attribute* VertexAttributes;
};

struct gsr_render_asset
{
    gsr_render_asset_type Type;
    union
    {
        gsr_model Model;
    };
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

//////////
// Basic Shaders

global char* gsr_StandardVertexShader = R"FOO(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 aColor;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 Color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main ()
{
gl_Position = Projection * View * Model * vec4(aPos, 1.0);
TexCoord = aTexCoord;
Normal = aNormal;
FragPos = vec3(Model * vec4(aPos, 1.0));
Color = aColor;
}
)FOO";

global char* gsr_StandardFragmentShader = R"FOO(
#version 330 core

in vec3 Normal;
in vec4 Color;

out vec4 FragColor;
uniform sampler2D Textures[4];

void main()
{
//FragColor = vec4(1, 0, 1, 1);
FragColor = Color;
}
)FOO";

global gsr_vertex_attribute gsr_StandardVertexAttributes[] = {
    { "aPos", gs_BasicType_r32, 3, 0 },
    { "aTexCoords", gs_BasicType_r32, 2, 0 },
    { "aNormal", gs_BasicType_r32, 3, 0 },
    { "aColor", gs_BasicType_r32, 4, 0},
    { 0 }
};

u8 gsr_DefaultTextureData[] = { 255, 0, 255, 255 };
u32 gsr_DefaultTextureWidth = 1;
u32 gsr_DefaultTextureHeight = 1;
u32 gsr_DefaultTextureChannels = 4;

u8 gsr_BTextureData[] = { 0, 255, 255, 255 };
u32 gsr_BTextureWidth = 1;
u32 gsr_BTextureHeight = 1;
u32 gsr_BTextureChannels = 4;

#ifdef DEBUG
internal void
gsr_AssertAssetTypeEquals(gsr_render_asset_list Assets, gsr_render_asset_handle Handle, gsr_render_asset_type Type)
{
    gsr_render_asset* Asset = GetElement(&Assets.AssetHeaders, gsr_render_asset, Handle);
    Assert(Asset->Type == Type);
}
#else
#define gsr_AssertAssetTypeEquals(assets, handle, type)
#endif

internal bool
AssetHandleIsValid(gsr_render_asset_list Assets, gsr_render_asset_handle Handle)
{
    return HandleIsValid(Assets.AssetHeaders, Handle);
}

internal gsr_render_asset_list
CreateRenderAssetList(gs_allocator* Allocator)
{
    gsr_render_asset_list Result = {0};
    Result.Arena = CreateMemoryArena(Allocator);
    Result.AssetHeaders = CreateDynarray(Allocator, gsr_render_asset,512);
    return Result;
}

inline gsr_render_asset*
gsr_GetAsset(gsr_render_asset_list* Assets, gsr_render_asset_handle Handle)
{
    gsr_render_asset* Result = GetElement(&Assets->AssetHeaders, gsr_render_asset, Handle);
    return Result;
}

#define gsr_GetAssetDataOfType(assets, handle, type) (type*)gsr_GetAssetDataOfType_((assets), (handle), Glue(gsr_RenderAsset_, type));
internal u8*
gsr_GetAssetDataOfType_(gsr_render_asset_list* Assets, gsr_render_asset_handle Handle, gsr_render_asset_type Type)
{
    u8* Result = 0;
    gsr_render_asset* Asset = gsr_GetAsset(Assets, Handle);
    Assert(Asset->Type == Type);
    return Asset->BackendAssetData;
}

static gsr_render_asset_handle
gsr_TakeAssetHandle(gsr_render_asset_list* Assets, gsr_render_asset_type Type)
{
    gsr_render_asset_handle Result = TakeFreeElement(&Assets->AssetHeaders);
    gsr_render_asset* Asset = gsr_GetAsset(Assets, Result);
    Asset->Type = Type;
    return Result;
}

//
internal u32
gsr_CalculateVertexAttributesStride(gsr_vertex_attribute* Attrs, u32 AttrsCount)
{
    u32 Result = 0;
    for (u32 Attr = 0; Attr < AttrsCount; Attr++)
    {
        gsr_vertex_attribute AttrAt = Attrs[Attr];
        u32 AttrEnd = AttrAt.Offset + VertexAttributeSize(AttrAt);
        if (AttrEnd > Result)
        {
            Result = AttrEnd;
        }
    }
    return Result;
}

internal gsr_render_asset_handle
gsr_TakeVertexBuffer(gsr_render_asset_list* Assets)
{
    gsr_render_asset_handle Result = gsr_TakeAssetHandle(Assets, gsr_RenderAsset_gsr_vertex_buffer);
    
    gsr_render_asset* Asset = gsr_GetAsset(Assets, Result);
    gsr_vertex_buffer* Buffer = PushStruct(&Assets->Arena, gsr_vertex_buffer);
    *Buffer = {0};
    Asset->BackendAssetData = (u8*)Buffer;
    
    return Result;
}
internal void
gsr_SetVertexBufferModel(gsr_render_asset_list* Assets, gsr_render_asset_handle BufferHandle, gsr_render_asset_handle Model)
{
    // These three lines can be compressed somehow
    gsr_vertex_buffer* Buffer = gsr_GetAssetDataOfType(Assets, BufferHandle, gsr_vertex_buffer);
    Buffer->ModelHandle = Model;
}
internal void
gsr_ResetVertexBuffer(gsr_vertex_buffer* Buffer)
{
    Buffer->BuffersRoot = 0;
    Buffer->BuffersHead = 0;
    Buffer->TotalVertsCount = 0;
    Buffer->TotalIndicesCount = 0;
}
internal gsr_vertex_buffer_buffer*
gsr_PushVertexBufferBuffer(gs_memory_arena* Arena)
{
    gsr_vertex_buffer_buffer* NewBuffer = PushStruct(Arena, gsr_vertex_buffer_buffer);
    *NewBuffer = {0};
    return NewBuffer;
}
internal void
gsr_AppendVertexBufferBuffer(gsr_vertex_buffer* Buffer, gsr_vertex_buffer_buffer* NewBuffer)
{
    if (Buffer->BuffersHead != 0)
    {
        Buffer->BuffersHead->Next = NewBuffer;
        Buffer->BuffersHead = NewBuffer;
    }
    else
    {
        Buffer->BuffersRoot = NewBuffer;
        Buffer->BuffersHead = NewBuffer;
    }
    
    Buffer->TotalVertsCount += NewBuffer->VertsCount;
    Buffer->TotalIndicesCount += NewBuffer->IndicesCount;
}
internal gsr_vertex_buffer_buffer*
gsr_GrowVertexBuffer(gsr_render_asset_list* Assets, gs_memory_arena* Arena, gsr_render_asset_handle BufferHandle, u32 VertexCount, u32 IndexCount)
{
    gsr_vertex_buffer_buffer* NewBuffer = gsr_PushVertexBufferBuffer(Arena);
    gsr_vertex_buffer* Buffer = gsr_GetAssetDataOfType(Assets, BufferHandle, gsr_vertex_buffer);
    gsr_render_asset* ModelAsset = gsr_GetAsset(Assets, Buffer->ModelHandle);
    NewBuffer->VertexStride = ModelAsset->Model.VertexStride;
    NewBuffer->VertsCount = VertexCount;
    NewBuffer->Verts = PushArray(Arena, u8, VertexCount * ModelAsset->Model.VertexStride);
    NewBuffer->IndicesCount = IndexCount;
    NewBuffer->Indices = PushArray(Arena, u32, IndexCount);
    gsr_AppendVertexBufferBuffer(Buffer, NewBuffer);
    return NewBuffer;
}
internal void
gsr_VertexBufferSetTriIndices(gsr_vertex_buffer_buffer* Buffer, u32 Tri, u32 V0, u32 V1, u32 V2)
{
    u32 TriBase = Tri * 3;
    Assert(TriBase <= Buffer->IndicesCount);
    Buffer->Indices[TriBase + 0] = V0;
    Buffer->Indices[TriBase + 1] = V1;
    Buffer->Indices[TriBase + 2] = V2;
}
#define gsr_VertexBufferSetVertex(buf,v,o,c,e) gsr_VertexBufferSetVertex_((buf),(v),(o),(c),sizeof((e)[0]),(u8*)(e))
internal void
gsr_VertexBufferSetVertex_(gsr_vertex_buffer_buffer* Buffer, u32 Vertex, u32 Offset, u32 ElementCount, u32 ElementSize, u8* Elements)
{
    Assert(Vertex < Buffer->VertsCount);
    // This assert is just cause I haven't needed anything other than 4 byte
    // vertex attributes yet
    Assert(ElementSize == sizeof(r32));
    u32 TotalOffset = (Buffer->VertexStride * Vertex) + (Offset * sizeof(r32));
    
    // This assert tells you if you're trying to write more than the vertex stride to the vertex.
    // Check the vertex attributes if this fires
    Assert(TotalOffset + (sizeof(r32) * ElementCount) <= (Buffer->VertsCount * Buffer->VertexStride));
    Assert((ElementCount * sizeof(r32)) <= Buffer->VertexStride);
    
    u8* VertexBase = Buffer->Verts + TotalOffset;
    CopyArray(Elements, VertexBase, u8, ElementCount * ElementSize);
}

#define GS_RENDERER_H
#endif // GS_RENDERER_H