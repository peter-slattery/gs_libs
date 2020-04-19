//
// File: gs_render_queue.h
// Author: Peter Slattery
// Creation Date: 2020-04-11
//
#ifndef GS_RENDER_QUEUE_H

enum gsr_render_type
{
    gsr_RenderType_CreateShader,
    gsr_RenderType_CompileShader,
    
    gsr_RenderType_CreateModel,
    gsr_RenderType_SetModelData,
    
    gsr_RenderType_CreateTexture,
    gsr_RenderType_SetTextureData,
    
    gsr_RenderType_Clear,
    gsr_RenderType_SetShader,
    gsr_RenderType_SetMatrixUniform,
    gsr_RenderType_SetTexture,
    gsr_RenderType_RenderModel,
    
    gsr_RenderType_Count,
};

struct gsr_compile_shader
{
    gs_dynarray_handle ShaderHandle;
    char* VertexShader;
    char* FragmentShader;
};

enum gsr_vertex_element_type
{
    gsr_VertexElement_Int32,
    gsr_VertexElement_Float32,
    
    gsr_VertexElement_Count,
};

struct gsr_vertex_attribute
{
    char* Name;
    
    gsr_vertex_element_type ElementType;
    // This is the number of elements per entry. ie. a v3 is ElementType = Float32, ElementCount = 3
    u32 ElementCount;
    
    s64 Offset;
    b32 Normalize;
};

struct gsr_create_model
{
    gs_dynarray_handle ModelHandle;
    
    gsr_vertex_attribute* VertexAttributes;
    u32 VertexAttributesCount;
    
    u32 VertexStride;
};

struct gsr_set_model_data
{
    gs_dynarray_handle ModelHandle;
    u32 VertexBufferSize;
    u32 IndexBufferSize;
    
    u8* VertexBuffer;
    u8* IndexBuffer;
    
    u32 VertexStride;
    u32 PositionOffset;
    u32 UVOffset;
    u32 NormalOffset;
};

struct gsr_set_texture_data
{
    gs_dynarray_handle TextureHandle;
    u8* Data;
    u32 Width;
    u32 Height;
    u32 Channels;
};

struct gsr_clear
{
    r32 R;
    r32 G;
    r32 B;
    r32 A;
};

struct gsr_set_shader
{
    gs_dynarray_handle ShaderHandle;
};

// NOTE: We don't support Matrices bigger than 4x4 rn.
#define GSR_MATRIX_MAX_SIZE 16

// NOTE: We don't support Uniforms bigger than 128
#define GSR_UNIFORM_NAME_MAX_LENGTH 128

struct gsr_set_matrix_uniform
{
    u32 ValuesCount;
    u32 EntriesCount;
    r32* Values;
    char UniformName[GSR_UNIFORM_NAME_MAX_LENGTH];
};

struct gsr_set_texture
{
    gs_dynarray_handle TextureHandle;
    u32 TextureIndex;
    char UniformName[256];
};

struct gsr_render_model
{
    gs_dynarray_handle ModelHandle;
};

struct gsr_render_entry
{
    gsr_render_type Type;
    union
    {
        gsr_compile_shader CompileShader;
        gsr_create_model CreateModel;
        gsr_set_model_data SetModelData;
        gsr_set_texture_data SetTextureData;
        
        gsr_clear Clear;
        gsr_set_shader SetShader;
        gsr_set_matrix_uniform SetMatrixUniform;
        gsr_set_texture SetTexture;
        gsr_render_model Model;
    };
    
    // NOTE(Peter): I'm ok with a linked list here because we will only ever traverse the
    // list sequentially, first to last.
    gsr_render_entry* Next;
};

struct gsr_render_queue
{
    gs_memory_arena Arena;
    gsr_render_entry* EntriesFirst;
    gsr_render_entry* EntriesLast;
    u32 EntriesCount;
    
    gsr_render_asset_list* Assets;
};

//
// Render Queue
//

static gsr_render_entry*
gsr_TakeRenderEntry(gsr_render_queue* Queue, gsr_render_type Type)
{
    gsr_render_entry* Result = PushStruct(&Queue->Arena, gsr_render_entry);
    if (Queue->EntriesLast)
    {
        Queue->EntriesLast->Next = Result;
    }
    else
    {
        Queue->EntriesFirst = Result;
    }
    Queue->EntriesLast = Result;
    Result->Next = 0;
    Result->Type = Type;
    Queue->EntriesCount += 1;
    return Result;
}

static void
gsr_ResetRenderQueue(gsr_render_queue* Queue)
{
    Queue->EntriesFirst = 0;
    Queue->EntriesLast = 0;
    Queue->EntriesCount = 0;
}

static gs_dynarray_handle
gsr_CreateShader(gsr_render_queue* Queue)
{
    gs_dynarray_handle Result = gsr_TakeAssetHandle(Queue->Assets, gsr_RenderAsset_ShaderProgram);
    return Result;
}

static u32
gsr_CalculateAttributeSize(gsr_vertex_attribute Attr)
{
    u32 Result = 0;
    switch (Attr.ElementType)
    {
        case gsr_VertexElement_Int32:
        {
            Result = sizeof(s32);
        }break;
        
        case gsr_VertexElement_Float32:
        {
            Result = sizeof(r32);
        }break;
        
        InvalidDefaultCase;
    }
    
    Result = Result * Attr.ElementCount;
    return Result;
}

static gs_dynarray_handle
gsr_CreateModel(gsr_render_queue* Queue, gsr_vertex_attribute* VertexAttributes, u32 VertexStride = 0)
{
    gs_dynarray_handle Result = gsr_TakeAssetHandle(Queue->Assets, gsr_RenderAsset_Model);
    
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_CreateModel);
    Entry->CreateModel.ModelHandle = Result;
    
    // Count Vertex Attributes
    u32 DefaultVertexStride = 0;
    u32 AttrCount = 0;
    gsr_vertex_attribute* AttrAt = VertexAttributes;
    while (AttrAt && AttrAt->Name != 0)
    {
        Assert(AttrAt->Name != 0);
        DefaultVertexStride += gsr_CalculateAttributeSize(*AttrAt);
        AttrCount++;
        AttrAt++;
    }
    
    if (VertexStride == 0)
    {
        Entry->CreateModel.VertexStride = DefaultVertexStride;
    }
    else
    {
        Entry->CreateModel.VertexStride = VertexStride;
    }
    
    Entry->CreateModel.VertexAttributesCount = AttrCount;
    Entry->CreateModel.VertexAttributes = PushArray(&Queue->Arena, gsr_vertex_attribute, AttrCount);
    
    // NOTE(Peter): ATTRIBUTE OFFSETS
    // Use this attr's offset to get the offset for the next attribute,
    // We only use this offset in the event that an attribute doesn't specify
    // one. So if this attr does, and the next one doesn't, the next attr
    // will be Attribute[n] = Attribute[n - 1].Offset + Attribute[n - 1].Size;
    // If it does specify an offset, this is ignored.
    //
    // Basically, if you specify offsets, we assume you know what you are doing,
    // and don't mess with that. If you don't specify offsets, we assume the
    // attributes are tightly packed. If you interleave, we defer when you
    // specify an offset, and assume tight packing when you dont.
    u32 AttrOffset = 0;
    for (u32 i = 0; i < Entry->CreateModel.VertexAttributesCount; i++)
    {
        gsr_vertex_attribute SrcAttr = VertexAttributes[i];
        Assert(SrcAttr.Name != 0);
        
        gsr_vertex_attribute* DestAttr = Entry->CreateModel.VertexAttributes + i;
        *DestAttr = SrcAttr;
        
        u32 AttrSize = gsr_CalculateAttributeSize(*DestAttr);
        if (DestAttr->Offset == 0)
        {
            DestAttr->Offset = AttrOffset;
            AttrOffset += AttrSize;
        }
        else
        {
            AttrOffset = DestAttr->Offset + AttrSize;
        }
    }
    
    return Result;
}

static gs_dynarray_handle
gsr_CreateTexture(gsr_render_queue* Queue)
{
    gs_dynarray_handle Result = gsr_TakeAssetHandle(Queue->Assets, gsr_RenderAsset_Texture);
    return Result;
}

static void
gsr_CompileShader(gsr_render_queue* Queue, gs_dynarray_handle Shader, char* VertexShader, char* FragmentShader)
{
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_CompileShader);
    Entry->CompileShader.ShaderHandle = Shader;
    Entry->CompileShader.VertexShader = VertexShader;
    Entry->CompileShader.FragmentShader = FragmentShader;
}

static void
gsr_SetModelData(gsr_render_queue* Queue, gs_dynarray_handle Model,
                 u32 VertexBufferSize, u8* VertexBuffer,
                 u32 IndexBufferSize, u8* IndexBuffer,
                 u32 VertexStride,
                 u32 PositionOffset, u32 UVOffset, u32 NormalOffset)
{
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_SetModelData);
    Entry->SetModelData.ModelHandle = Model;
    Entry->SetModelData.VertexBufferSize = VertexBufferSize;
    Entry->SetModelData.IndexBufferSize = IndexBufferSize;
    Entry->SetModelData.VertexBuffer = VertexBuffer;
    Entry->SetModelData.IndexBuffer = IndexBuffer;
    Entry->SetModelData.VertexStride = VertexStride;
    Entry->SetModelData.PositionOffset = PositionOffset;
    Entry->SetModelData.UVOffset = UVOffset;
    Entry->SetModelData.NormalOffset = NormalOffset;
}

static void
gsr_SetTextureData(gsr_render_queue* Queue, gs_dynarray_handle Texture, u8* Data, u32 Width, u32 Height, u32 Channels)
{
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_SetTextureData);
    Entry->SetTextureData.TextureHandle = Texture;
    Entry->SetTextureData.Data = Data;
    Entry->SetTextureData.Width = Width;
    Entry->SetTextureData.Height = Height;
    Entry->SetTextureData.Channels = Channels;
}

static void
gsr_Clear(gsr_render_queue* Queue, r32 R, r32 G, r32 B, r32 A)
{
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_Clear);
    Entry->Clear.R = R;
    Entry->Clear.G = G;
    Entry->Clear.B = B;
    Entry->Clear.A = A;
}

static void
gsr_SetShader(gsr_render_queue* Queue, gs_dynarray_handle ShaderHandle)
{
    gsr_AssertAssetTypeEquals(*Queue->Assets, ShaderHandle, gsr_RenderAsset_ShaderProgram);
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_SetShader);
    Entry->SetShader.ShaderHandle = ShaderHandle;
}

static void
gsr_SetMatrixUniform(gsr_render_queue* Queue, u32 ValuesCount, u32 EntriesCount, r32* Values, char* UniformName)
{
    Assert(ValuesCount <= GSR_MATRIX_MAX_SIZE);
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_SetMatrixUniform);
    Entry->SetMatrixUniform.ValuesCount = ValuesCount;
    Entry->SetMatrixUniform.EntriesCount = EntriesCount;
    
    Entry->SetMatrixUniform.Values = (r32*)malloc(sizeof(r32) * ValuesCount * EntriesCount);
    for (u32 E = 0; E < EntriesCount; E++)
    {
        for (u32 V = 0; V < ValuesCount; V++)
        {
            u32 ValueIndex = (E * ValuesCount) + V;
            Entry->SetMatrixUniform.Values[ValueIndex] = Values[ValueIndex];
        }
    }
    
    // Copy Uniform Name
    char* C = UniformName;
    s32 i = 0;
    while (*C)
    {
        Assert(i < GSR_UNIFORM_NAME_MAX_LENGTH);
        Entry->SetMatrixUniform.UniformName[i++] = *C++;
    }
    Entry->SetMatrixUniform.UniformName[i] = 0;
}

static void
gsr_SetTexture(gsr_render_queue* Queue, gs_dynarray_handle TextureHandle, u32 TextureIndex, char* UniformName)
{
    gsr_AssertAssetTypeEquals(*Queue->Assets, TextureHandle, gsr_RenderAsset_Texture);
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_SetTexture);
    Entry->SetTexture.TextureHandle = TextureHandle;
    Entry->SetTexture.TextureIndex = TextureIndex;
    
    char* Src = UniformName;
    char* Dst = Entry->SetTexture.UniformName;
    while (*Src)
    {
        *Dst++ = *Src++;
    }
    *Dst = 0;
}

static void
gsr_RenderModel(gsr_render_queue* Queue, gs_dynarray_handle ModelHandle)
{
    gsr_AssertAssetTypeEquals(*Queue->Assets, ModelHandle, gsr_RenderAsset_Model);
    gsr_render_entry* Entry = gsr_TakeRenderEntry(Queue, gsr_RenderType_RenderModel);
    Entry->Model.ModelHandle = ModelHandle;
}

#if 0
// TODO(Peter): Make this into a render command
static gsr_renderbuffer
gsr_CreateRenderbuffer(gsr_renderer* Renderer, u32 Width, u32 Height)
{
    gsr_renderbuffer Result = {0};
    switch(Renderer->Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            gswo_procs OpenGL = Renderer->WinOpenGLContext.Procs;
            if (!gswo_CreateRenderbuffer(OpenGL, Width, Height, &Result.Framebuffer, &Result.ColorTexture, &Result.DepthStencilTexture))
            {
                InvalidCodePath;
            }
        }break;
        
        InvalidDefaultCase;
    }
    return Result;
}
#endif

#define GS_RENDER_QUEUE_H
#endif // GS_RENDER_QUEUE_H