/* date = May 10th 2021 9:37 pm */

#ifndef GS_SCRATCH_H
#define GS_SCRATCH_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <gl/gl.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#define ASSERTS_ACTIVE 1
#include "../src/gs_assert.h"

#define GS_GFX_PROFILE_SCOPE PROFILE_SCOPE_G
#include "../src/gs_profiler.h"

#define GS_MEMORY_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_FILE_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_THREAD_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_MATH_PROFILE_FUNC PROFILE_SCOPE_G
#include "../src/memory/gs_memory.h"
#include "../src/file/gs_file.h"
#include "../src/thread/gs_thread.h"
#include "../src/time/gs_time.h"
#include "../src/gs_debug_break.h"
#include "../src/gs_math.h"
#include "../src/gs_printf.h"

#define GFX_BACKEND_GL33
#include "../src/gfx/gs_gfx.h"

#include "../src/gs_profiler.c"

// NOTE(PS): Stuff gs_libs don't rely on
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) Assert(x)
#include "../src/stb/stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "../src/stb/stb_truetype.h"

internal void
DrawTestTri (gs_gfx_handler* GfxH, r32 X, r32 Y, r32 Z, r32 D)
{
  GfxDrawTriangle(GfxH,
                  (v4){X - D, Y - D, Z, 1}, 
                  (v4){X + D, Y - D, Z, 1}, 
                  (v4){X, Y + D, Z, 1},
                  (v2){0, 0}, (v2){1, 0}, (v2){1, 1},
                  RedV4, GreenV4, BlueV4);
}

internal void
PrintFrameStats(gs_memory_arena* Transient, gs_time_handler TimeHandler)
{
  gs_profiler_statistics Stats = ProfilerStatisticsCreate(*GlobalProfiler, GlobalProfiler->CurrentFrameIndex, Transient);
  printf("\n==== FRAME ====\n");
  for (u64 i = 0; i < Stats.ScopeStatsCount; i++)
  {
    gs_profiler_scope_statistics S = Stats.ScopeStats[i];
    if (S.CallCount == 0) continue;
    
    //if (!(CStringsEqual(S.Id.FunctionName, "main") || CStringsEqual(S.Id.FunctionName, "WindowSwapBuffers"))) continue;
    s32 ColumnWidth = 32;
    s32 NameLen = CStringLength(S.Id.FunctionName);
    s32 PrintLen = Min(NameLen, ColumnWidth);
    printf("%.*s", PrintLen, S.Id.FunctionName);
    
    s32 FillLen = ColumnWidth - PrintLen;
    if (FillLen > 0)
    {
      printf("%.*s", FillLen, "...............................");
    }
    
    printf("%lld cy \t\t %f sec\n", S.CyclesTotal, TicksToSeconds(TimeHandler, S.CyclesTotal));
  }
}

internal void
PrintFrameTimeline(gs_memory_arena* Transient)
{
  gs_profiler_timeline Timeline = ProfilerTimelineCreate(*GlobalProfiler, GlobalProfiler->CurrentFrameIndex, Transient);
  char* TimelinePrinted = MPrintF(Transient, "{gs_profiler_timeline}", Timeline);
  printf("%s", TimelinePrinted);
}

internal void
PrintMemoryUsage(gs_allocator Allocator, gs_memory_arena* Transient)
{
  printf("\nALLOCATIONS:\n");
  for (u64 i = 0; i < Allocator.DEBUGAllocList->ArenaHashesCount; i++)
  {
    u64 ArenaHash = Allocator.DEBUGAllocList->ArenaHashes[i];
    if (ArenaHash == 0) continue; 
    gs_debug_arena_info Info = Allocator.DEBUGAllocList->ArenaInfos[i];
    u64 AverageSize = (Info.TotalSize / Info.AllocationsCount);
    char* ArenaPrinted = MPrintF(Transient, "{char*}\n\tSize: {u64} bytes\n\tAvg Alloc Size: {u64} bytes\n\tAllocations:\n",
                                 &Info.ArenaName, &Info.TotalSize, &AverageSize);
    printf("%s", ArenaPrinted);
    
    for (gs_debug_memory_allocation* Allocation = Allocator.DEBUGAllocList->Root;
         Allocation != 0;
         Allocation = Allocation->Next)
    {
      if (Allocation->ArenaHash != ArenaHash) continue;
      
      char* AllocationPrinted = MPrintF(Transient, "\t\tFunction: {char*} - Size: {u64} bytes\n",
                                        &Allocation->Loc.Function, &Allocation->Size);
      printf("%s", AllocationPrinted);
    }
  }
  
}

typedef struct gs_image
{
  s32 Width;
  s32 Height;
  s32 Channels;
  u64 Size;
  u8* Data;
} gs_image;

internal gs_image
LoadImageFromFile(gs_file_handler FH, char* Path, gs_memory_arena* Arena, gs_memory_arena* Scratch)
{
  gs_file_error Error = 0;
  gs_data Image = FileReadAllAndClose(FH, Path, Scratch, &Error);
  
  s32 DesiredChannels = 4;
  
  gs_image Result = {};
  
  Result.Data = stbi_load_from_memory(Image.Memory, Image.Size, &Result.Width, &Result.Height, &Result.Channels, DesiredChannels);
  Result.Size = Result.Width * Result.Height * Result.Channels * sizeof(u8);
  
  return Result;
}

int main(int ArgCount, char** Args)
{
  gs_allocator Allocator = CreatePlatformAllocator();
  gs_thread_manager ThreadManager = CreatePlatformThreadManager(1);
  gs_time_handler TimeHandler = CreatePlatformTimeHandler();
  ProfilerCreateGlobal(Allocator, 32, 2048, &ThreadManager, TimeHandler);
  
  gs_memory_arena Arena = MemoryArenaCreate(4096, 8, Allocator, 0, 0, "Permanent");
  gs_memory_arena Transient = MemoryArenaCreate(4096, 8, Allocator, 0, 0, "Transient");
  
  gs_file_handler FileHandler = CreatePlatformFileHandler(Allocator);
  PrintFInit(Allocator);
  
  gs_gfx_handler GfxHandler = CreatePlatformGfxHandler(Allocator);
  
#define WINDOW_COUNT 1
  gs_window Windows[WINDOW_COUNT];
  
  gs_gfx_context_desc GfxDesc = {
    .ColorBits = 32,
    .AlphaBits = 8,
    .DepthBits = 0,
  };
  
  for (s32 i = 0; i < WINDOW_COUNT; i++)
  {
    gs_window_desc WindowDesc = {
      .WindowName = MPrintF(&Arena, "GS Window {s32}", &i),
      .Width = 1920,
      .Height = 1080,
      // other parameters?
    };
    Windows[i] = WindowOpen(GfxHandler, &WindowDesc);
    WindowCreateGraphicsContext(GfxHandler, Windows[i], &GfxDesc);
  }
  
  // Texture
  gs_image Owlbear = LoadImageFromFile(FileHandler, "./data/snowy_owlbear.png", &Arena, &Transient);
  gs_gfx_texture_handle OwlbearHandle = GfxTextureLoad(&GfxHandler, &(gs_gfx_texture_desc){
                                                         .Data = Owlbear.Data,
                                                         .W = Owlbear.Width,
                                                         .H = Owlbear.Height,
                                                         .C = Owlbear.Channels,
                                                         .BackendTextureFormat = TextureFormat_RGBA8,
                                                       });
  
  // Font
  gs_data TempFontData = FileReadAllAndClose(FileHandler, "./data/liberation-mono.ttf", &Transient, 0);
  Assert(TempFontData.Size > 0);
  gs_image SingleBitFontImage = {
    .Width = 512,
    .Height = 512,
    .Channels = 1,
    .Size = SingleBitFontImage.Width * SingleBitFontImage.Height * SingleBitFontImage.Channels,
    .Data = PushSize(&Arena, SingleBitFontImage.Size).Memory,
  };
  stbtt_bakedchar cdata[96];
  stbtt_BakeFontBitmap(TempFontData.Memory, 0, 32, SingleBitFontImage.Data, SingleBitFontImage.Width, SingleBitFontImage.Height, 32, 96, cdata);
  gs_image FontImage = {
    .Width = 512,
    .Height = 512,
    .Channels = 4,
    .Size = FontImage.Width * FontImage.Height * FontImage.Channels,
    .Data = PushSize(&Arena, FontImage.Size).Memory,
  };
  
  // Copy the single bit data over to a 32bit RGBA image
  u8* FromRow = SingleBitFontImage.Data;
  u8* ToRow = FontImage.Data;
  for (u32 Y = 0; Y < FontImage.Height; Y++)
  {
    u8* From = FromRow;
    u32* To = (u32*)ToRow;
    
    for (u32 X = 0; X < FontImage.Height; X++)
    {
      *To++ = 0x00FFFFFF | (((u32)*From++) << 24);
    }
    
    FromRow += SingleBitFontImage.Width;
    ToRow += FontImage.Width * sizeof(u32);
  }
  
  gs_gfx_texture_handle FontTextureHandle = GfxTextureLoad(&GfxHandler, &(gs_gfx_texture_desc){
                                                             .Data = FontImage.Data,
                                                             .W = FontImage.Width,
                                                             .H = FontImage.Height,
                                                             .C = FontImage.Channels,
                                                             .MinFilter = TextureFilter_Linear,
                                                           });
  
  gs_gfx_vertex_buffer_builder IMMBuilder = VBBCreate(2048, 1024, VertexData_PosTexCol, &Arena);
  // Update the quad verts
  char* Text = "G";
  
  VBBClear(&IMMBuilder);
  
  r32 X = 0;
  r32 Y = 0;
  for (char* At = Text; *At != 0; At++)
  {
    stbtt_aligned_quad Quad;
    stbtt_GetBakedQuad(cdata, FontImage.Width, FontImage.Height, *At - 32, &X, &Y, &Quad, 1);
    
    u32 V0 = VBBPushVertex(&IMMBuilder,
                           (v4){ Quad.x0, Quad.y0, 0, 1 },
                           (v2){ Quad.s0, Quad.t1 },
                           (v4){ 1, 1, 1, 1 });
    
    u32 V1 = VBBPushVertex(&IMMBuilder,
                           (v4){ Quad.x1, Quad.y0, 0, 1 },
                           (v2){ Quad.s1, Quad.t1 },
                           (v4){ 1, 1, 1, 1});
    
    u32 V2 = VBBPushVertex(&IMMBuilder,
                           (v4){ Quad.x1, Quad.y1, 0, 1 },
                           (v2){ Quad.s1, Quad.t0 },
                           (v4){ 1, 1, 1, 1 });
    
    u32 V3 = VBBPushVertex(&IMMBuilder,
                           (v4){ Quad.x0, Quad.y1, 0, 1 },
                           (v2){ Quad.s0, Quad.t0 },
                           (v4){ 1, 1, 1, 1 });
    
    VBBPushTri(&IMMBuilder, V0, V1, V2);
    VBBPushTri(&IMMBuilder, V0, V2, V3);
  }
  
#if 0
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(&GfxHandler);
  GLCtx->glBindBuffer(GL_ARRAY_BUFFER, IMMVBO.VertexBufferID);
  GLCtx->glBufferData(GL_ARRAY_BUFFER, IMMBuilder.VertexDataCount * sizeof(r32), (u8*)IMMBuilder.VertexData, GL_STATIC_DRAW);
  
  GLCtx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IMMVBO.IndicesBufferID);
  GLCtx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, IMMBuilder.IndexDataCount * sizeof(u32), (u8*)IMMBuilder.IndexData, GL_STATIC_DRAW);
#endif
  
  gs_gfx_vertex_buffer_handle IMMVBO = GfxModelCreate(&GfxHandler, &IMMBuilder.ModelDesc);
  
  // Shader
  gs_data VertShaderData = FileReadAllAndClose(FileHandler, "./data/test_vert.vert", &Arena, 0);
  gs_data FragShaderData = FileReadAllAndClose(FileHandler, "./data/test_frag.frag", &Arena, 0);
  
  gs_gfx_shader_program ShaderProgram = GfxShaderProgramCompileAndLink(&GfxHandler, 
                                                                       (char*)VertShaderData.Memory, 
                                                                       (char*)FragShaderData.Memory, 
                                                                       &Arena);
  
  r32 Z = 0;
  r32 CX = 0; //1920 / 2;
  r32 CY = 0; //1080 / 2;
  r32 D = 5;
  
  gs_gfx_vertex_buffer_builder OwlbearBuilder = VBBCreate(2048, 1024, VertexData_PosTexCol, &Arena);
  
  {
    u32 V0 = VBBPushVertex(&OwlbearBuilder, (v4){CX - D, CY - D, Z, 1}, (v2){0, 0}, (v4){0, 1, 1, 1});
    u32 V1 = VBBPushVertex(&OwlbearBuilder, (v4){CX + D, CY - D, Z, 1}, (v2){1, 0}, (v4){1, 0, 1, 1});
    u32 V2 = VBBPushVertex(&OwlbearBuilder, (v4){CX + D, CY + D, Z, 1}, (v2){1, 1}, (v4){1, 1, 0, 1});
    u32 V3 = VBBPushVertex(&OwlbearBuilder, (v4){CX - D, CY + D, Z, 1}, (v2){0, 1}, (v4){1, 1, 1, 1});
    
    VBBPushTri(&OwlbearBuilder, V0, V1, V2);
    VBBPushTri(&OwlbearBuilder, V0, V2, V3);
  }
  
#if 0
  r32 Verts[] = {
    CX - D, CY - D, Z, 1, // pos
    0, 0, // tex
    CX + D, CY - D, Z, 1, // pos
    1, 0, // tex
    CX + D, CY + D, Z, 1, // pos
    1, 1, // tex
    CX - D, CY + D, Z, 1, // pos
    0, 1, // tex
  };
  u32 Ind[] = {
    0, 1, 2,
    0, 2, 3,
  };
  
  gs_gfx_model_desc ModelDesc = GfxModelDescConstruct((r32*)Verts, CArrayLength(Verts), VertexData_PosTex, Ind, CArrayLength(Ind));
#endif
  
  gs_gfx_vertex_buffer_handle VBO = GfxModelCreate(&GfxHandler, &OwlbearBuilder.ModelDesc); 
  
  
  r32 Acc = 0;
  gs_window_events Events = {};
  s32 WindowsClosed = 0;
  bool Running = true;
  u64 LastFrameEnd = TicksGetCurrent(TimeHandler);
  while (Running)
  {
    ProfilerAdvanceFrame(GlobalProfiler);
    {
      PROFILE_SCOPE_G;
      
      
      MemoryArenaClear(&Transient);
      
      Acc += 0.05f;
      if (Acc > TauR32) Acc = 0;
      
      for (s32 i = 0; i < WINDOW_COUNT; i++)
      {
        gs_window* Window = Windows + i;
        WindowPollEvents(GfxHandler, *Window, &Events);
        if (WindowShouldClose(GfxHandler, *Window))
        {
          WindowClose(GfxHandler, Window);
          WindowsClosed++;
          
          if (WindowsClosed >= WINDOW_COUNT) break;
        }
        
        //GfxSetWindowCurrent(GfxHandler, *Window);
        
        gs_window_dim WindowDim = WindowGetDim(GfxHandler, *Window);
        
        GfxSetViewport(&GfxHandler, 0, 0, WindowDim.Width, WindowDim.Height);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        GfxClearBuffer(&GfxHandler, Acc / TauR32, 0, 1, 1, GFXBufferMask_Color | GFXBufferMask_Depth);
        
        // Triangle + Texture
        GfxShaderProgramActivate(&GfxHandler, ShaderProgram);
        {
          m44 Model = M44Identity();
          Model = M44MultiplyM44(Model, M44Translation((v4){0, 0, -25, 0}));
          /*
          Model = M44MultiplyM44(Model, M44Translation((v4){(r32)WindowDim.Width/2, (r32)WindowDim.Height/2, -50, 0}));
*/
          Model = M44MultiplyM44(Model, M44Translation((v4){SinR32(Acc) * 50, 0, 0, 0}));
          
          Model = M44MultiplyM44(Model, M44RotationZ(Acc));
          
          GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "Model", Model);
          GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "View", M44Identity());
          // GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "Projection", M44ProjectionOrthoScreen(WindowDim.Width, WindowDim.Height, 0, 100));
          r32 AspectRatio = (r32)WindowDim.Width / (r32)WindowDim.Height;
          //GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "View", M44ModelView(ForwardV4, RightV4, (v4){0,0,0,1}));
          
          GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "Projection", M44ProjectionPerspective(45, AspectRatio, 0.01f, 100));
          
          GfxUniformTextureSet(&GfxHandler, ShaderProgram, OwlbearHandle, "Texture0");
          
          GfxVertexBufferDraw(&GfxHandler, VBO);
        }
        
        
#if 0
        // ImM font
        {
          
          //GfxUniformTextureSet(&GfxHandler, ShaderProgram, FontTextureHandle, "Texture0");
          
          m44 Model = M44Identity();
          Model = M44MultiplyM44(Model, M44Translation((v4){150, 300, -25, 0}));
          Model = M44MultiplyM44(Model, M44ScaleUniform(25));
          
          GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "Model", Model);
          GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "View", M44Identity());
          GfxUniformMatrixSet(&GfxHandler, ShaderProgram, "Projection", M44ProjectionOrthoScreen(WindowDim.Width, WindowDim.Height, .1f, 100));
          
          GfxVertexBufferDraw(&GfxHandler, IMMVBO);
          
          //GfxVertexBufferDraw(&GfxHandler, VBO);
          
        }
#endif
        
        GfxShaderProgramActivate(&GfxHandler, (gs_gfx_shader_program){});
        
        WindowSwapBuffers(GfxHandler, Windows[i]);
      }
      
      if (WindowsClosed >= WINDOW_COUNT)
      {
        Running = false;
      }
    }
    
    // PrintFrameStats(&Transient, TimeHandler);
    // PrintFrameTimeline(&Transient);
    // PrintMemoryUsage(Allocator, &Transient);
    
    u64 TicksEnd = TicksGetCurrent(TimeHandler);
    r64 SecondsElapsed = TickRangeToSeconds(TimeHandler, LastFrameEnd, TicksEnd);
    r64 TargetSecondsPerFrame = 1.0 / 30.0;
    while (SecondsElapsed < TargetSecondsPerFrame)
    {
      u64 SleepTime = 1000.0 * (TargetSecondsPerFrame - SecondsElapsed);
      SleepForMS(TimeHandler, SleepTime);
      SecondsElapsed = TickRangeToSeconds(TimeHandler, LastFrameEnd, TicksGetCurrent(TimeHandler));
    }
    
    LastFrameEnd = TicksGetCurrent(TimeHandler);
  }
  
  return 0;
}

#endif //GS_SCRATCH_H
