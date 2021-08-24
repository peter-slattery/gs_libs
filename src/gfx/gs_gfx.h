/* date = May 22nd 2021 2:21 pm */

#ifndef GS_GFX_H
#define GS_GFX_H

#ifndef GS_GFX_PROFILE_SCOPE
# define GS_GFX_PROFILE_SCOPE
#endif

enum
{
  WindowStyle_Maximized = 0x1,
} gs_window_style;

typedef struct gs_window_desc
{
  char* WindowName;
  u64 Width, Height;
  u64 StyleFlags;
} gs_window_desc;

typedef struct gs_window
{
  u32 WindowId;
} gs_window;

enum
{
  ButtonState_IsDown = 0x1,
  ButtonState_WasDown = 0x2,
} gs_input_btn_flags;

typedef struct gs_input_btn_state
{
  u8 Flags;
} gs_input_btn_state;

enum
{
  MouseButton_Left,
  MouseButton_Middle,
  MouseButton_Right,
  MouseButton_Count,
} gs_mouse_button;

typedef enum
{
  KeyCode_Invalid,
  
  KeyCode_Esc,
  
  KeyCode_Space,
  KeyCode_Tab,
  KeyCode_CapsLock,
  KeyCode_LeftShift, KeyCode_RightShift,
  KeyCode_LeftCtrl, KeyCode_RightCtrl,
  KeyCode_Fn,
  KeyCode_Alt,
  KeyCode_PageUp, KeyCode_PageDown,
  KeyCode_Backspace, KeyCode_Delete,
  KeyCode_Enter,
  
  // Function Keys
  KeyCode_F0, KeyCode_F1, KeyCode_F2, KeyCode_F3, KeyCode_F4, KeyCode_F5, KeyCode_F6, KeyCode_F7,
  KeyCode_F8, KeyCode_F9, KeyCode_F10, KeyCode_F11, KeyCode_F12,
  
  // Letters
  KeyCode_a, KeyCode_b, KeyCode_c, KeyCode_d, KeyCode_e, KeyCode_f, KeyCode_g, KeyCode_h,
  KeyCode_i, KeyCode_j, KeyCode_k, KeyCode_l, KeyCode_m, KeyCode_n, KeyCode_o, KeyCode_p,
  KeyCode_q, KeyCode_r, KeyCode_s, KeyCode_t, KeyCode_u, KeyCode_v, KeyCode_w, KeyCode_x,
  KeyCode_y, KeyCode_z,
  
  KeyCode_A, KeyCode_B, KeyCode_C, KeyCode_D, KeyCode_E, KeyCode_F, KeyCode_G, KeyCode_H,
  KeyCode_I, KeyCode_J, KeyCode_K, KeyCode_L, KeyCode_M, KeyCode_N, KeyCode_O, KeyCode_P,
  KeyCode_Q, KeyCode_R, KeyCode_S, KeyCode_T, KeyCode_U, KeyCode_V, KeyCode_W, KeyCode_X,
  KeyCode_Y, KeyCode_Z,
  
  // Numbers
  KeyCode_0, KeyCode_1, KeyCode_2, KeyCode_3, KeyCode_4, KeyCode_5, KeyCode_6, KeyCode_7,
  KeyCode_8, KeyCode_9,
  
  KeyCode_Num0, KeyCode_Num1, KeyCode_Num2, KeyCode_Num3, KeyCode_Num4, KeyCode_Num5,
  KeyCode_Num6, KeyCode_Num7, KeyCode_Num8, KeyCode_Num9,
  
  // Symbols
  KeyCode_Bang, KeyCode_At, KeyCode_Pound, KeyCode_Dollar, KeyCode_Percent, KeyCode_Carrot,
  KeyCode_Ampersand, KeyCode_Star, KeyCode_LeftParen, KeyCode_RightParen, KeyCode_Minus, KeyCode_Plus,
  KeyCode_Equals, KeyCode_Underscore, KeyCode_LeftBrace, KeyCode_RightBrace, KeyCode_LeftBracket,
  KeyCode_RightBracket, KeyCode_Colon, KeyCode_SemiColon, KeyCode_SingleQuote, KeyCode_DoubleQuote,
  KeyCode_ForwardSlash, KeyCode_Backslash, KeyCode_Pipe, KeyCode_Comma, KeyCode_Period,
  KeyCode_QuestionMark, KeyCode_LessThan, KeyCode_GreaterThan, KeyCode_Tilde, KeyCode_BackQuote,
  
  // Arrows
  KeyCode_UpArrow,
  KeyCode_DownArrow,
  KeyCode_LeftArrow,
  KeyCode_RightArrow,
  
  KeyCode_Count,
} gs_key_code;

#define MAX_TYPED_STRING 256
typedef struct gs_window_events
{
  s32 MouseX, MouseY;
  s32 LastMouseX, LastMouseY;
  
  s32 MouseScroll;
  gs_input_btn_state MouseButtonStates[MouseButton_Count];
  
  u8 LastButtonPressed;
  s32 MouseDownX, MouseDownY;
  
  gs_input_btn_state KeyStates[KeyCode_Count];
  
  u64 TypedStringLength;
  char TypedString[MAX_TYPED_STRING];
} gs_window_events;

typedef struct gs_window_dim
{
  s32 Width, Height;
} gs_window_dim;

enum {
  GFXBufferMask_Color = 0x1,
  GFXBufferMask_Depth = 0x2,
  GFXBufferMask_Accumulation = 0x4,
  GFXBufferMask_Stencil = 0x8,
} gs_gfx_buffer_mask;

enum {
  GFXMatrix_Invalid,
  
  GfxMatrix_ModelView,
  GfxMatrix_Projection,
  
  GFXMatrix_Count,
} gs_gfx_matrix_mode;

typedef struct gs_gfx_context_desc
{
  s32 ColorBits;
  s32 AlphaBits;
  s32 DepthBits;
  s32 StencilBits;
} gs_gfx_context_desc;

//
// Platform Gfx Handler
//

typedef struct gs_gfx_handler gs_gfx_handler;

// TODO(PS): do the whole NoOp thing
#define PLATFORM_WINDOW_OPEN(name) gs_window name(gs_window_desc* Desc, u8* UserData)
#define PLATFORM_WINDOW_CLOSE(name) bool name(gs_window* Window, u8* UserData)
#define PLATFORM_WINDOW_SHOULD_CLOSE(name) bool name(gs_window Window, u8* UserData)
#define PLATFORM_WINDOW_GET_DIM(name) gs_window_dim name(gs_window Window, u8* UserData)
#define PLATFORM_WINDOW_POLL_EVENTS(name) bool name(gs_window Window, gs_window_events* Events, u8* UserData)
#define PLATFORM_WINDOW_SWAP_BUFFERS(name) void name(gs_window Window, u8* UserData);

// Graphics Context
#define PLATFORM_WINDOW_CREATE_GRAPHICS_CONTEXT(name) void name(gs_window Window, gs_gfx_context_desc* Desc, gs_gfx_handler GfxHandler)
#define PLATFORM_BACKEND_SET_WINDOW_CURRENT(name) void name(gs_window Window, gs_gfx_handler GfxHandler)

typedef PLATFORM_WINDOW_OPEN(platform_window_open);
typedef PLATFORM_WINDOW_CLOSE(platform_window_close);
typedef PLATFORM_WINDOW_SHOULD_CLOSE(platform_window_should_close);
typedef PLATFORM_WINDOW_GET_DIM(platform_window_get_dim);
typedef PLATFORM_WINDOW_POLL_EVENTS(platform_window_poll_events);
typedef PLATFORM_WINDOW_SWAP_BUFFERS(platform_window_swap_buffers);

// Graphics Context
typedef PLATFORM_WINDOW_CREATE_GRAPHICS_CONTEXT(platform_window_create_graphics_context);
typedef PLATFORM_BACKEND_SET_WINDOW_CURRENT(platform_backend_set_window_current);

typedef struct gs_gfx_handler
{
  // Window
  platform_window_open* PWindowOpen;
  platform_window_close* PWindowClose;
  platform_window_should_close* PWindowShouldClose;
  platform_window_get_dim* PWindowGetDim;
  platform_window_poll_events* PWindowPollEvents;
  platform_window_swap_buffers* PWindowSwapBuffers;
  u8* WindowUserData;
  
  // Backend
  platform_window_create_graphics_context* PWindowCreateGraphicsContext;
  platform_backend_set_window_current* PBackendSetWindowCurrent;
  
  u8* BackendUserData;
} gs_gfx_handler;

//
// Window
//

internal gs_window
WindowOpen(gs_gfx_handler WH, gs_window_desc* Desc)
{
  GS_GFX_PROFILE_SCOPE;
  return WH.PWindowOpen(Desc, WH.WindowUserData);
}

internal bool
WindowClose(gs_gfx_handler WH, gs_window* Window)
{
  GS_GFX_PROFILE_SCOPE;
  return WH.PWindowClose(Window, WH.WindowUserData);
}

internal bool
WindowShouldClose(gs_gfx_handler WH, gs_window Window)
{
  GS_GFX_PROFILE_SCOPE;
  return WH.PWindowShouldClose(Window, WH.WindowUserData);;
}

internal gs_window_dim
WindowGetDim(gs_gfx_handler WH, gs_window Window)
{
  GS_GFX_PROFILE_SCOPE;
  return WH.PWindowGetDim(Window, WH.WindowUserData);;
}


internal bool
WindowPollEvents(gs_gfx_handler WH, gs_window Window, gs_window_events* Events)
{
  GS_GFX_PROFILE_SCOPE;
  return WH.PWindowPollEvents(Window, Events, WH.WindowUserData);
}

internal void
WindowSwapBuffers(gs_gfx_handler WH, gs_window Window)
{
  GS_GFX_PROFILE_SCOPE;
  WH.PWindowSwapBuffers(Window, WH.WindowUserData);
}

//
// Event
//

internal void
InputButtonSetDown(gs_input_btn_state* StatesList, u64 Index)
{
  GS_GFX_PROFILE_SCOPE;
  
  u8 Flags = StatesList[Index].Flags;
  if (HasFlag(Flags, ButtonState_IsDown))
  {
    AddFlag(Flags, ButtonState_WasDown);
  }
  AddFlag(Flags, ButtonState_IsDown);
  StatesList[Index].Flags = Flags;
}

internal void
InputButtonSetUp(gs_input_btn_state* StatesList, u64 Index)
{
  GS_GFX_PROFILE_SCOPE;
  
  u8 Flags = StatesList[Index].Flags;
  if (HasFlag(Flags, ButtonState_IsDown))
  {
    AddFlag(Flags, ButtonState_WasDown);
  } else {
    RemoveFlag(Flags, ButtonState_WasDown);
  }
  RemoveFlag(Flags, ButtonState_IsDown);
  StatesList[Index].Flags = Flags;
}

internal bool
InputButtonIsUp(gs_input_btn_state* StatesList, u64 Index)
{
  GS_GFX_PROFILE_SCOPE;
  
  u8 Flags = StatesList[Index].Flags;
  bool Result = !HasFlag(Flags, ButtonState_IsDown);
  return Result;
}

internal bool
InputButtonIsDown(gs_input_btn_state* StatesList, u64 Index)
{
  GS_GFX_PROFILE_SCOPE;
  
  u8 Flags = StatesList[Index].Flags;
  bool Result = HasFlag(Flags, ButtonState_IsDown);
  return Result;
}

internal bool
InputButtonTransitionedUp(gs_input_btn_state* StatesList, u64 Index)
{
  GS_GFX_PROFILE_SCOPE;
  
  u8 Flags = StatesList[Index].Flags;
  bool Result = (!HasFlag(Flags, ButtonState_IsDown) &&
                 HasFlag(Flags, ButtonState_WasDown));;
  return Result;
}

internal bool
InputButtonTransitionedDown(gs_input_btn_state* StatesList, u64 Index)
{
  GS_GFX_PROFILE_SCOPE;
  
  u8 Flags = StatesList[Index].Flags;
  bool Result = (HasFlag(Flags, ButtonState_IsDown) &&
                 !HasFlag(Flags, ButtonState_WasDown));;
  return Result;
}

internal void
InputAppendTypedChar(gs_window_events* Events, char C)
{
  GS_GFX_PROFILE_SCOPE;
  
  u64 Index = Events->TypedStringLength++;
  if (Index < MAX_TYPED_STRING)
  {
    Events->TypedString[Index] = C;
  }
}


//
// Graphics Functions
//

#if 0
typedef enum
{
  GfxBlend_Disabled,
  GfxBlend_SrcAlpha_OneMinusSrcAlpha,
} gs_gfx_blend_mode;

typedef struct gs_gfx_frame_settings
{
  
} gs_gfx_frame_settings;
#endif

typedef enum 
{
  VertexData_Invalid,
  
  VertexData_Pos,
  VertexData_PosTex,
  VertexData_PosCol,
  VertexData_PosTexCol,
  VertexData_PosColTex,
  
  VertexData_Count,
} gs_gfx_vertex_data_order;

// Sizes in bytes
global u64 GfxVertexSize = 4;
global u64 GfxTexCoordSize = 2;
global u64 GfxVertexColorSize = 4;

typedef struct gs_gfx_model_desc
{
  gs_gfx_vertex_data_order DataOrder;
  
  u32 VertexChannelCount;
  u32 VertexCount;
  
  r32* VertexData;
  u64 VertexDataSize;
  
  u64 VertexOffset;
  u64 VertexStride;
  u64 TexCoordOffset;
  u64 TexCoordStride;
  u64 VertexColorOffset;
  u64 VertexColorStride;
  
  u32 IndicesCount;
  u32* Indices;
} gs_gfx_model_desc;

typedef enum
{
  GfxShader_Invalid,
  
  GfxShader_Vertex,
  GfxShader_Fragment,
  
  GfxShader_Count,
} gs_gfx_shader_type;

typedef struct gs_gfx_vertex_buffer_handle
{
  u32 VertexArrayID;
  u32 VertexBufferID;
  u32 IndicesBufferID;
  
  u32 VertexCount;
  u32 IndicesCount;
} gs_gfx_vertex_buffer_handle;

// Backend Function Prototypes
internal void GfxSetViewport(gs_gfx_handler* GfxH, r32 X, r32 Y, r32 W, r32 H);
internal void GfxClearBuffer(gs_gfx_handler* GfxH, r32 R, r32 G, r32 B, r32 A, u8 BufferMask);
internal void GfxSetMatrix(gs_gfx_handler* GfxH, u8 Matrix, m44 Value);
internal void GfxDrawTriangle(gs_gfx_handler* GfxH, v4 P0, v4 P1, v4 P2, v2 UV0, v2 UV1, v2 UV2, v4 C0, v4 C1, v4 C2);

internal gs_gfx_vertex_buffer_handle GfxModelCreate(gs_gfx_handler* GfxH, gs_gfx_model_desc* Desc);

internal gs_gfx_model_desc
GfxModelDescConstruct(r32* VertexData, u32 VertexCount, gs_gfx_vertex_data_order DataOrder, u32* Indices, u32 IndexCount)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_model_desc Result = {
    .DataOrder = DataOrder,
    .VertexCount = VertexCount,
    .VertexData = VertexData,
    .IndicesCount = IndexCount,
    .Indices = Indices,
  };
  
  u64 Stride = 0;
  switch (DataOrder)
  {
    case VertexData_Pos: 
    {
      Stride = GfxVertexSize;
      Result.VertexOffset = 0;
      Result.VertexStride = Stride;
      Result.VertexChannelCount = 4;
    } break;
    
    case VertexData_PosTex: 
    {
      Stride = GfxVertexSize + GfxTexCoordSize;
      Result.VertexOffset = 0;
      Result.VertexStride = Stride;
      Result.TexCoordOffset = GfxVertexSize;
      Result.TexCoordStride = Stride;
      Result.VertexChannelCount = 6;
    }break;
    
    case VertexData_PosCol: 
    {
      Stride = GfxVertexSize + GfxVertexColorSize;
      Result.VertexOffset = 0;
      Result.VertexStride = Stride;
      Result.VertexColorOffset = GfxVertexSize;
      Result.VertexColorStride = Stride;
      Result.VertexChannelCount = 8;
    }break;
    
    case VertexData_PosTexCol: 
    {
      Stride = GfxVertexSize + GfxTexCoordSize + GfxVertexColorSize;
      Result.VertexOffset = 0;
      Result.VertexStride = Stride;
      Result.TexCoordOffset = GfxVertexSize;
      Result.TexCoordStride = Stride;
      Result.VertexColorOffset = GfxVertexSize + GfxTexCoordSize;
      Result.VertexColorStride = Stride;
      Result.VertexChannelCount = 10;
    }break;
    
    case VertexData_PosColTex: 
    {
      Stride = GfxVertexSize + GfxTexCoordSize + GfxVertexColorSize;
      Result.VertexOffset = 0;
      Result.VertexStride = Stride;
      Result.VertexColorOffset = GfxVertexSize;
      Result.VertexColorStride = Stride;
      Result.TexCoordOffset = GfxVertexSize + GfxVertexColorSize;
      Result.TexCoordStride = Stride;
      Result.VertexChannelCount = 10;
    }break;
    
    InvalidDefaultCase;
  }
  
  Result.VertexDataSize = Stride * VertexCount;
  
  return Result;
}

// Shaders

typedef struct gs_gfx_shader_program
{
  u32 BackendHandle;
} gs_gfx_shader_program;

// Textures

typedef enum gs_gfx_texture_format
{
  TextureFormat_RGBA8,
  TextureFormat_Alpha,
} gs_gfx_texture_format;

typedef enum gs_gfx_texture_filter
{
  TextureFilter_Nearest,
  TextureFilter_Linear,
} gs_gfx_texture_filter;

typedef struct gs_gfx_texture_desc
{
  u8* Data;
  s32 W, H, C;
  gs_gfx_texture_format BackendTextureFormat;
  gs_gfx_texture_filter MinFilter, MagFilter;
} gs_gfx_texture_desc;

typedef struct gs_gfx_texture_handle
{
  u32 Handle;
} gs_gfx_texture_handle;

gs_gfx_texture_handle GfxTextureLoad(gs_gfx_handler* GH, gs_gfx_texture_desc* Desc);
void GfxUniformTextureSet(gs_gfx_handler* GH, gs_gfx_shader_program Program, gs_gfx_texture_handle Handle, char* UniformName);

//
// Graphics Context
//

internal void
WindowCreateGraphicsContext(gs_gfx_handler WH, gs_window Window, gs_gfx_context_desc* Desc)
{
  GS_GFX_PROFILE_SCOPE;
  
  WH.PWindowCreateGraphicsContext(Window, Desc, WH);
}

internal void
GfxSetWindowCurrent(gs_gfx_handler WH, gs_window Window)
{
  GS_GFX_PROFILE_SCOPE;
  
  WH.PBackendSetWindowCurrent(Window, WH);
}

#ifdef GS_PLATFORM_IMPLEMENTATION

//
// Graphics Utilities
//


typedef struct gs_gfx_vertex_buffer_builder
{
  gs_gfx_model_desc ModelDesc;
  u32 VertexDataCountMax;
  u32 IndexDataCountMax;
} gs_gfx_vertex_buffer_builder;

internal gs_gfx_vertex_buffer_builder
VBBCreate(u32 VertexDataCountMax, u32 IndexDataCountMax, gs_gfx_vertex_data_order VertDataOrder, gs_memory_arena* Arena)
{
  r32* VertexData = PushArray(Arena, r32, VertexDataCountMax);
  u32* IndexData = PushArray(Arena, u32, IndexDataCountMax);
  gs_gfx_vertex_buffer_builder Result = {};
  Result.VertexDataCountMax = VertexDataCountMax;
  Result.IndexDataCountMax = IndexDataCountMax;
  Result.ModelDesc = GfxModelDescConstruct(VertexData, 0, VertDataOrder, IndexData, 0);
  
  return Result;
}

internal u32
VBBVertexBaseGet(gs_gfx_vertex_buffer_builder* B)
{
  u32 Result = B->ModelDesc.VertexDataSize;
  Assert(Result % B->ModelDesc.VertexChannelCount == 0);
  return Result;
}

internal u32
VBBAdvanceVertex(gs_gfx_vertex_buffer_builder* B)
{
  Assert(B->ModelDesc.VertexDataSize < B->VertexDataCountMax);
  // we should never be advancing in the middle of filling out a vertex
  Assert(B->ModelDesc.VertexDataSize % B->ModelDesc.VertexStride == 0);
  u32 Result = B->ModelDesc.VertexDataSize / B->ModelDesc.VertexStride;
  Assert(Result == B->ModelDesc.VertexCount);
  
  B->ModelDesc.VertexCount += 1;
  B->ModelDesc.VertexDataSize += B->ModelDesc.VertexStride;
  return Result;
}

internal void
VBBPushFloat(gs_gfx_vertex_buffer_builder* B, u32 VertexIndex, r32 V)
{
  Assert(VertexIndex < B->VertexDataCountMax);
  B->ModelDesc.VertexData[VertexIndex] = V;
}

internal void
VBBSetV2(gs_gfx_vertex_buffer_builder* B, u32 Base, v2 P)
{
  VBBPushFloat(B, Base + 0, P.x);
  VBBPushFloat(B, Base + 1, P.y);
}

internal void
VBBSetV4(gs_gfx_vertex_buffer_builder* B, u32 Base, v4 P)
{
  VBBPushFloat(B, Base + 0, P.x);
  VBBPushFloat(B, Base + 1, P.y);
  VBBPushFloat(B, Base + 2, P.z);
  VBBPushFloat(B, Base + 3, P.w);
}

internal u32
VBBPushVertex(gs_gfx_vertex_buffer_builder* B, v4 Pos, v2 Tex, v4 Color)
{
  u32 Result = 0;
  
  u32 Base = VBBVertexBaseGet(B);
  VBBSetV4(B, Base + B->ModelDesc.VertexOffset, Pos);
  
  if (B->ModelDesc.DataOrder == VertexData_PosTex ||
      B->ModelDesc.DataOrder == VertexData_PosTexCol ||
      B->ModelDesc.DataOrder == VertexData_PosColTex)
  {
    VBBSetV2(B, Base + B->ModelDesc.TexCoordOffset, Tex);
  }
  
  if (B->ModelDesc.DataOrder == VertexData_PosCol ||
      B->ModelDesc.DataOrder == VertexData_PosTexCol ||
      B->ModelDesc.DataOrder == VertexData_PosColTex)
  {
    VBBSetV4(B, Base + B->ModelDesc.VertexColorOffset, Color);
  }
  
  Result = VBBAdvanceVertex(B);
  return Result;
}

internal void
VBBPushIndex(gs_gfx_vertex_buffer_builder* B, u32 Index)
{
  Assert(B->ModelDesc.IndicesCount < B->IndexDataCountMax);
  B->ModelDesc.Indices[B->ModelDesc.IndicesCount++] = Index;
}

internal void
VBBPushTri(gs_gfx_vertex_buffer_builder* B, u32 V0, u32 V1, u32 V2)
{
  VBBPushIndex(B, V0);
  VBBPushIndex(B, V1);
  VBBPushIndex(B, V2);
}

internal void
VBBClear(gs_gfx_vertex_buffer_builder* B)
{
  B->ModelDesc.VertexDataSize = 0;
  B->ModelDesc.IndicesCount = 0;
}


//
// Platforms
//
internal void GfxHandlerGetPlatformWindowCmds(gs_gfx_handler* GfxHandler, gs_allocator A);

# if PLATFORM_WINDOWS
#  include "./gs_gfx_win32.h"
# elif PLATFORM_OSX
#  include "./gs_gfx_osx.h"
# elif PLATFORM_LINUX
#  include "./gs_gfx_linux.h"
# endif

//
// GFX Backends
//
internal void GfxHandlerGetBackendCmds(gs_gfx_handler* GfxHandler, gs_allocator A);

#if   defined(GFX_BACKEND_GL33)
# include "./gs_gfx_gl33.h"
#elif defined(GFX_BACKEND_D3D11)
# include "./gs_gfx_d3d11.h"
#else
# error "You must specify a backend to use gs_gfx.h if you have also defined GS_PLATFORM_IMPLEMENTATION"
#endif

#endif // GS_PLATFORM_IMPLEMENTATION

internal gs_gfx_handler
CreatePlatformGfxHandler(gs_allocator A)
{
  gs_gfx_handler Result = {};
  GfxHandlerGetPlatformWindowCmds(&Result, A);
  GfxHandlerGetBackendCmds(&Result, A);
  return Result;
}

#endif //GS_GFX_H
