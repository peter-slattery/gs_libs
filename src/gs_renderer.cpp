//
// File: gs_renderer.cpp
// Author: Peter Slattery
// Creation Date: 2020-04-11
//
#ifndef GS_RENDERER_CPP

#include "gs_win32_opengl.h"

struct gsr_renderbuffer
{
    u32 Framebuffer;
    u32 ColorTexture;
    u32 DepthStencilTexture;
};

enum gsr_renderer_backend
{
    gsr_RendererBackends_Win_OpenGL,
    gsr_RendererBackends_Count
};

struct gsr_win_opengl_context
{
    gswo_render_context RenderContext;
    gswo_procs Procs;
};

struct gsr_renderer
{
    gs_memory_arena RenderMemory;
    gsr_render_asset_list Assets;
    gsr_renderer_backend Backend;
    union
    {
        gsr_win_opengl_context WinOpenGLContext;
    };
    
    gsr_render_asset_handle DefaultShader;
    gsr_render_asset_handle DefaultTexture;
    gsr_render_asset_handle BTexture;
    
    gs_string ErrorLog;
};

static gsr_renderer
gsr_InitRenderer(gs_allocator* Allocator, gsr_renderer_backend Backend, u32 ColorBits, u32 AlphaBits, u32 DepthBits, HDC WinDeviceContext, gs_string* ErrorLog)
{
    gsr_renderer Result = {0};
    Result.RenderMemory = CreateMemoryArena(Allocator);
    Result.Assets = CreateRenderAssetList(Allocator);
    Result.Backend = Backend;
    
    switch (Result.Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            Result.WinOpenGLContext.RenderContext = gswo_Init(WinDeviceContext,
                                                              ColorBits, AlphaBits, DepthBits,
                                                              &Result.WinOpenGLContext.Procs);
        }break;
    }
    
    // TODO(Peter): NOT CROSS BACKEND COMPATIBLE
    // TODO(Peter): Also duplicates code in the render loop
    Result.DefaultShader = gsr_TakeAssetHandle(&Result.Assets, gsr_RenderAsset_ShaderProgram);
    gsr_render_asset* DefaultShaderAsset = gsr_GetAsset(&Result.Assets, Result.DefaultShader);
    DefaultShaderAsset->BackendAssetData = gswo_CreateShaderProgram(Result.WinOpenGLContext.Procs, &Result.Assets.Arena);
    gswo_CompileAndLinkProgram(Result.WinOpenGLContext.Procs, DefaultShaderAsset,
                               gsr_StandardVertexShader, gsr_StandardFragmentShader,
                               ErrorLog);
    
    
    Result.DefaultTexture = gsr_TakeAssetHandle(&Result.Assets, gsr_RenderAsset_Texture);
    gsr_render_asset* DefaultTextureAsset = gsr_GetAsset(&Result.Assets, Result.DefaultTexture);
    DefaultTextureAsset->BackendAssetData = gswo_CreateTexture(&Result.Assets.Arena);
    gswo_SetTextureData(DefaultTextureAsset, gsr_DefaultTextureData, gsr_DefaultTextureWidth, gsr_DefaultTextureHeight, gsr_DefaultTextureChannels);
    
    Result.BTexture = gsr_TakeAssetHandle(&Result.Assets, gsr_RenderAsset_Texture);
    gsr_render_asset* BTextureAsset = gsr_GetAsset(&Result.Assets, Result.BTexture);
    BTextureAsset->BackendAssetData = gswo_CreateTexture(&Result.Assets.Arena);
    gswo_SetTextureData(BTextureAsset, gsr_BTextureData, gsr_BTextureWidth, gsr_BTextureHeight, gsr_BTextureChannels);
    
    Result.ErrorLog = AllocatorAllocString(Allocator, 512);
    return Result;
}

static void
gsr_CleanupRenderer(gsr_renderer* Renderer)
{
    switch (Renderer->Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            gswo_CleanupRenderContext(&Renderer->WinOpenGLContext.RenderContext);
        }break;
    }
}

static gsr_render_queue
gsr_CreateRenderQueue(gsr_renderer* Renderer)
{
    gsr_render_queue Result = {0};
    Result.Arena = CreateMemoryArena(Renderer->RenderMemory.Allocator);
    Result.Assets = &Renderer->Assets;
    return Result;
}

static void
gsr_RenderQueueOpenGL(gsr_renderer* Renderer, gsr_render_queue* Queue, gswo_render_context RenderContext, gswo_procs OpenGL, gsr_renderbuffer Renderbuffer)
{
    gswo_BindFramebuffer(OpenGL, Renderbuffer.Framebuffer);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT, GL_FILL);
    
    // TODO(Peter): When you bind a shader, look up all the uniform locations and store them?
    // that way we aren't constantly pulling the shader out of render_assets
    // and looking up the same uniforms (model, view, projection, etc)
    gs_dynarray_handle BoundShader = {0};
    
    gs_dynarray_handle BoundTexture[12];
    gs_dynarray_handle BoundModel = {0};
    
    gsr_render_entry* Entry = Queue->EntriesFirst;
    while(Entry)
    {
        switch (Entry->Type)
        {
            case gsr_RenderType_CompileShader:
            {
                gsr_render_asset* Asset = gsr_GetAsset(&Renderer->Assets, Entry->CompileShader.ShaderHandle);
                Assert(Asset != 0);
                if (Asset->BackendAssetData == 0)
                {
                    Asset->BackendAssetData = gswo_CreateShaderProgram(OpenGL, &Renderer->Assets.Arena);
                }
                gswo_CompileAndLinkProgram(OpenGL, Asset, Entry->CompileShader,
                                           &Renderer->ErrorLog);
            }break;
            
            case gsr_RenderType_CreateModel:
            {
                gsr_render_asset* Asset = gsr_GetAsset(&Renderer->Assets, Entry->CreateModel.ModelHandle);
                Assert(Asset->BackendAssetData == 0);
                Asset->BackendAssetData = gswo_CreateModel(OpenGL, &Renderer->Assets.Arena, Asset->Model);
            }break;
            
            case gsr_RenderType_SetModelData:
            {
                gsr_render_asset* Asset = gsr_GetAsset(&Renderer->Assets, Entry->SetModelData.ModelHandle);
                Assert(Asset != 0);
                gswo_SetModelData(OpenGL, Asset, Entry->SetModelData);
            }break;
            
            case gsr_RenderType_SetTextureData:
            {
                gsr_render_asset* Asset = gsr_GetAsset(&Renderer->Assets, Entry->SetTextureData.TextureHandle);
                Assert(Asset != 0);
                if (Asset->BackendAssetData == 0)
                {
                    Asset->BackendAssetData = gswo_CreateTexture(&Renderer->Assets.Arena);
                }
                gswo_SetTextureData(Asset, Entry->SetTextureData);
            }break;
            
            case gsr_RenderType_Clear:
            {
                gswo_Clear(Entry->Clear);
            }break;
            
            case gsr_RenderType_SetShader:
            {
                gsr_render_asset* ShaderAsset = gsr_GetAsset(&Renderer->Assets, Entry->SetShader.ShaderHandle);
                gswo_UseShader(OpenGL, ShaderAsset);
                BoundShader = Entry->SetShader.ShaderHandle;
            }break;
            
            case gsr_RenderType_SetMatrixUniform:
            {
                gsr_render_asset* ShaderAsset = gsr_GetAsset(&Renderer->Assets, BoundShader);
                gswo_shader_program* Shader = (gswo_shader_program*)ShaderAsset->BackendAssetData;
                s32 UniformLocation = OpenGL.glGetUniformLocation(Shader->ShaderProgram, Entry->SetMatrixUniform.UniformName);
                if (UniformLocation >= 0)
                {
                    if (Entry->SetMatrixUniform.ValuesCount == 1)
                    {
                        OpenGL.glUniform1fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if (Entry->SetMatrixUniform.ValuesCount == 2)
                    {
                        OpenGL.glUniform2fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if (Entry->SetMatrixUniform.ValuesCount == 3)
                    {
                        OpenGL.glUniform3fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if (Entry->SetMatrixUniform.ValuesCount == 4)
                    {
                        OpenGL.glUniform4fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if(Entry->SetMatrixUniform.ValuesCount == 9)
                    {
                        OpenGL.glUniformMatrix3fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, GL_FALSE, Entry->SetMatrixUniform.Values);
                    }
                    else if(Entry->SetMatrixUniform.ValuesCount == 16)
                    {
                        gswo_SetMatrix44(OpenGL, UniformLocation, Entry->SetMatrixUniform.Values);
                    }
                }
            }break;
            
            case gsr_RenderType_SetTexture:
            {
                u32 TextureIndex = Entry->SetTexture.TextureIndex;
                char* UniformName = Entry->SetTexture.UniformName;
                gsr_render_asset* ShaderAsset = gsr_GetAsset(&Renderer->Assets, BoundShader);
                if (HandleIsValid(Renderer->Assets.AssetHeaders, Entry->SetTexture.TextureHandle))
                {
                    if (!HandlesAreEqual(BoundTexture[TextureIndex], Entry->SetTexture.TextureHandle))
                    {
                        gsr_render_asset* TextureAsset = gsr_GetAsset(&Renderer->Assets, Entry->SetTexture.TextureHandle);
                        gswo_BindTexture(OpenGL, ShaderAsset, TextureAsset, TextureIndex, UniformName);
                        BoundTexture[TextureIndex] = Entry->SetTexture.TextureHandle;
                    }
                }
                else
                {
                    gswo_BindTexture(OpenGL, ShaderAsset, 0, TextureIndex, UniformName);
                    BoundTexture[TextureIndex] = {0};
                }
            }break;
            
            case gsr_RenderType_RenderModel:
            {
                gsr_render_asset* ModelAsset = gsr_GetAsset(&Renderer->Assets, Entry->Model.ModelHandle);
                
                if (!HandlesAreEqual(BoundModel, Entry->Model.ModelHandle))
                {
                    gswo_BindModel(OpenGL, ModelAsset);
                    BoundModel = Entry->Model.ModelHandle;
                }
                
                gswo_RenderModel(ModelAsset);
            }break;
            
            case gsr_RenderType_ConsolidateVertexBuffer:
            {
                gsr_consolidate_vertex_buffer Consolidate = Entry->ConsolidateVertexBuffer;
                gsr_vertex_buffer* Buffer = gsr_GetAssetDataOfType(&Renderer->Assets, Consolidate.Buffer, gsr_vertex_buffer);
                Assert(AssetHandleIsValid(Renderer->Assets, Buffer->ModelHandle));
                
                gsr_set_model_data SetModelData = {0};
                SetModelData.ModelHandle = Buffer->ModelHandle;
                
                gsr_render_asset* ModelAsset = gsr_GetAsset(&Renderer->Assets, Buffer->ModelHandle);
                Assert(ModelAsset->Type == gsr_RenderAsset_Model);
                
                // Not sure this is needed, but I don't know why you'd ever get here and not have set indices
                // and therefore, I'm not sure how I want to handle that case.
                // These asserts are here to let me know when it happens, so I can decide then, they aren't
                // absolutely the worst and can be removed
                Assert(Buffer->TotalVertsCount > 0);
                Assert(Buffer->TotalIndicesCount > 0);
                SetModelData.VertexBufferSize = ModelAsset->Model.VertexStride * Buffer->TotalVertsCount;
                SetModelData.IndexBufferSize = Buffer->TotalIndicesCount * sizeof(u32);
                SetModelData.VertexBuffer = PushSize(&Queue->Arena, SetModelData.VertexBufferSize);
                SetModelData.IndexBuffer = PushSize(&Queue->Arena, SetModelData.IndexBufferSize);
                
                u8* DestVertsAt = SetModelData.VertexBuffer;
                u8* DestIndicesAt = SetModelData.IndexBuffer;
                u32 VertexIndexAt = 0;
                gsr_vertex_buffer_buffer* BufferAt = Buffer->BuffersRoot;
                while (BufferAt != 0)
                {
                    u32 CopySize = ModelAsset->Model.VertexStride * BufferAt->VertsCount;
                    CopyArray(BufferAt->Verts, DestVertsAt, u8, CopySize);
                    for (u32 i = 0; i < BufferAt->IndicesCount; i++)
                    {
                        *((u32*)DestIndicesAt) = VertexIndexAt + BufferAt->Indices[i];
                        DestIndicesAt += sizeof(u32);
                    }
                    DestVertsAt += CopySize;
                    VertexIndexAt += BufferAt->VertsCount;
                    BufferAt = BufferAt->Next;
                }
                
                gswo_SetModelData(OpenGL, ModelAsset, SetModelData);
            }break;
            
            InvalidDefaultCase;
        }
        Entry = Entry->Next;
    }
    OpenGL.glBindVertexArray(0);
    
}

static void
gsr_RenderQueue(gsr_renderer* Renderer, gsr_render_queue* Queue, gsr_renderbuffer RenderbufferTarget = {0})
{
    switch(Renderer->Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            gsr_win_opengl_context Context = Renderer->WinOpenGLContext;
            gsr_RenderQueueOpenGL(Renderer, Queue, Context.RenderContext, Context.Procs, RenderbufferTarget);
        }break;
    }
}

static void
gsr_DisplayBackbuffer(gsr_renderer* Renderer)
{
    switch (Renderer->Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            gsr_win_opengl_context Context = Renderer->WinOpenGLContext;
            gswo_Render(Context.RenderContext);
        }break;
    }
}

static void
gsr_ResetRenderer(gsr_renderer* Renderer)
{
    ClearArena(&Renderer->RenderMemory);
}

#define GS_RENDERER_CPP
#endif // GS_RENDERER_CPP