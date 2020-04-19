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
};

static gsr_renderer
gsr_InitRenderer(gs_allocator* Allocator, gsr_renderer_backend Backend, u32 ColorBits, u32 AlphaBits, u32 DepthBits, HDC WinDeviceContext)
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
            printf("Got Contexts\n");
            //Result.WinOpenGLContext.Procs = gswo_GetProcAddresses();
        }break;
    }
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

#if 0
// :NowARenderQueueCommand
static void
gsr_CompileShader(gsr_renderer* Renderer, gsr_render_asset_handle ShaderHandle, char* VertexShader, char* FragmentShader)
{
    u8* AssetData = 0;
    switch (Renderer->Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            gsr_win_opengl_context Context = Renderer->WinOpenGLContext;
            AssetData = gswo_CompileAndLinkProgram(Context.Procs, VertexShader, FragmentShader, &Renderer->Assets.Arena);
        }break;
    }
    
    gsr_render_asset* ShaderAsset = gsr_GetAsset(&Renderer->Assets, ShaderHandle);
    Assert(AssetData);
    Assert(ShaderAsset != 0);
    ShaderAsset->BackendAssetData = AssetData;
}

static void
gsr_CreateTexture(gsr_renderer* Renderer, gsr_render_asset_handle TextureHandle,
                  u8* TextureData, u32 TextureWidth, u32 TextureHeight, u32 TextureChannels)
{
    u8* AssetData = 0;
    switch (Renderer->Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            AssetData = gswo_CreateTexture(TextureData, TextureWidth, TextureHeight, TextureChannels,
                                           &Renderer->Assets.Arena);
        }break;
    }
    gsr_render_asset* TextureAsset = gsr_GetAsset(&Renderer->Assets, TextureHandle);
    Assert(AssetData != 0);
    Assert(TextureAsset != 0);
    
    TextureAsset->BackendAssetData = AssetData;
}

static void
gsr_SetModelData(gsr_renderer* Renderer, gsr_render_asset_handle Model, u32 VertexBufferSize, u8* VertexBuffer, u32 FaceIndexBufferSize, u8* FaceIndexBuffer, u32 VertexStride, u32 PositionOffset, u32 UVOffset, u32 NormalOffset)
{
    gsr_render_asset* Asset = gsr_GetAsset(&Renderer->Assets, Model);
    Assert(Asset != 0);
    
    switch (Renderer->Backend)
    {
        case gsr_RendererBackends_Win_OpenGL:
        {
            Assert(Asset->BackendAssetData != 0);
            
            gswo_SetModelData(Renderer->WinOpenGLContext.Procs, (gswo_model*)Asset->BackendAssetData,
                              VertexBufferSize, VertexBuffer,
                              FaceIndexBufferSize, FaceIndexBuffer,
                              VertexStride,
                              PositionOffset, UVOffset, NormalOffset,
                              &Renderer->Assets.Arena);
        }break;
    }
}
#endif

static void
gsr_RenderQueueOpenGL(gsr_renderer* Renderer, gsr_render_queue* Queue, gswo_render_context RenderContext, gswo_procs Procs, gsr_renderbuffer Renderbuffer)
{
    gswo_procs OpenGL = Renderer->WinOpenGLContext.Procs;
    
    gswo_BindFramebuffer(Procs, Renderbuffer.Framebuffer);
    
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT, GL_FILL);
    
    // TODO(Peter): When you bind a shader, look up all the uniform locations and store them?
    // that way we aren't constantly pulling the shader out of render_assets
    // and looking up the same uniforms (model, view, projection, etc)
    gs_dynarray_handle BoundShader = {0};
    
    // TODO(Peter): This doesn't support multiple textures
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
                gswo_CompileAndLinkProgram(OpenGL, Asset, Entry->CompileShader);
            }break;
            
            case gsr_RenderType_CreateModel:
            {
                gsr_render_asset* Asset = gsr_GetAsset(&Renderer->Assets, Entry->CreateModel.ModelHandle);
                Assert(Asset->BackendAssetData == 0);
                Asset->BackendAssetData = gswo_CreateModel(Renderer->WinOpenGLContext.Procs, &Renderer->Assets.Arena,
                                                           Entry->CreateModel);
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
                s32 UniformLocation = Procs.glGetUniformLocation(Shader->ShaderProgram, Entry->SetMatrixUniform.UniformName);
                if (UniformLocation >= 0)
                {
                    if (Entry->SetMatrixUniform.ValuesCount == 1)
                    {
                        Procs.glUniform1fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if (Entry->SetMatrixUniform.ValuesCount == 2)
                    {
                        Procs.glUniform2fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if (Entry->SetMatrixUniform.ValuesCount == 3)
                    {
                        Procs.glUniform3fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if (Entry->SetMatrixUniform.ValuesCount == 4)
                    {
                        Procs.glUniform4fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, Entry->SetMatrixUniform.Values);
                    }
                    else if(Entry->SetMatrixUniform.ValuesCount == 9)
                    {
                        Procs.glUniformMatrix3fv(UniformLocation, Entry->SetMatrixUniform.EntriesCount, GL_FALSE, Entry->SetMatrixUniform.Values);
                    }
                    else if(Entry->SetMatrixUniform.ValuesCount == 16)
                    {
                        gswo_SetMatrix44(Procs, UniformLocation, Entry->SetMatrixUniform.Values);
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
                        gswo_BindTexture(Procs, ShaderAsset, TextureAsset, TextureIndex, UniformName);
                        BoundTexture[TextureIndex] = Entry->SetTexture.TextureHandle;
                    }
                }
                else
                {
                    gswo_BindTexture(Procs, ShaderAsset, 0, TextureIndex, UniformName);
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
        }
        Entry = Entry->Next;
    }
    Procs.glBindVertexArray(0);
    
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