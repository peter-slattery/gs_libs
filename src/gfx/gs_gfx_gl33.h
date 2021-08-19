/* date = May 22nd 2021 5:42 pm */

#ifndef GS_GFX_GL33_H
#define GS_GFX_GL33_H

#define GL_MAJOR_VERSION                        0x821B
#define GL_MINOR_VERSION                        0x821C
#define GL_SHADING_LANGUAGE_VERSION             0x8B8C

#define GL_DEBUG_OUTPUT_SYNCHRONOUS             0x8242
#define GL_DEBUG_SEVERITY_HIGH                  0x9146
#define GL_DEBUG_SEVERITY_MEDIUM                0x9147
#define GL_DEBUG_SEVERITY_LOW                   0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION          0x826B

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB        0x20A9

#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_RED_BITS_ARB                        0x2015
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS                        0x2023

#define GL_FRAGMENT_SHADER                      0x8B30
#define GL_VERTEX_SHADER                        0x8B31
#define GL_COMPILE_STATUS                       0x8B81
#define GL_LINK_STATUS                          0x8B82
#define GL_INFO_LOG_LENGTH                      0x8B84

#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

#define GL_TEXTURE0                       0x84C0

typedef unsigned int  GLenum;
typedef unsigned int  GLuint;
typedef int  GLsizei;
typedef char  GLchar;
typedef ptrdiff_t  GLintptr;
typedef ptrdiff_t  GLsizeiptr;
typedef double  GLclampd;
typedef unsigned short  GLushort;
typedef unsigned char  GLubyte;
typedef unsigned char  GLboolean;
typedef uint64_t  GLuint64;
typedef double  GLdouble;
typedef unsigned short  GLhalf;
typedef float  GLclampf;
typedef unsigned int  GLbitfield;
typedef signed char  GLbyte;
typedef short  GLshort;
typedef void  GLvoid;
typedef int64_t  GLint64;
typedef float  GLfloat;
typedef struct __GLsync * GLsync;
typedef int  GLint;

typedef HGLRC WINAPI wgl_create_context_attribs_arb (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL WINAPI wgl_choose_pixel_format_arb(HDC hdc,
                                                const int *piAttribIList,
                                                const FLOAT *pfAttribFList,
                                                UINT nMaxFormats,
                                                int *piFormats,
                                                UINT *nNumFormats);
typedef const char *WINAPI wgl_get_extensions_string_ext (void);

// Debug
typedef void APIENTRY GLDEBUGPROC(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
typedef void APIENTRY gl_debug_message_callback (GLDEBUGPROC callback, const void *userParam);

// Swap Interval
typedef BOOL WINAPI wgl_swap_interval_ext (int interval);

// Shaders
typedef GLuint APIENTRY gl_create_shader (GLenum type);
typedef void APIENTRY gl_shader_source (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void APIENTRY gl_compile_shader (GLuint shader);
typedef void APIENTRY gl_attach_shader (GLuint program, GLuint shader);
typedef void APIENTRY gl_detach_shader (GLuint program, GLuint shader);
typedef void APIENTRY gl_delete_shader (GLuint shader);
typedef GLuint APIENTRY gl_create_program (void);
typedef void APIENTRY gl_link_program (GLuint program);
typedef void APIENTRY gl_delete_program (GLuint program);
typedef void APIENTRY gl_use_program (GLuint program);
typedef void APIENTRY gl_get_shaderiv (GLuint shader, GLenum pname, GLint *params);
typedef void APIENTRY gl_get_shader_info_log (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void APIENTRY gl_get_programiv (GLuint program, GLenum pname, GLint *params);
typedef void APIENTRY gl_get_program_info_log (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

// VAO & VBO
typedef void APIENTRY gl_gen_vertex_arrays (GLsizei n, GLuint *arrays);
typedef void APIENTRY gl_bind_vertex_array (GLuint array);
typedef void APIENTRY gl_delete_vertex_arrays (GLsizei n, const GLuint *arrays);
typedef void APIENTRY gl_gen_buffers (GLsizei n, GLuint *buffers);
typedef void APIENTRY gl_bind_buffer (GLenum target, GLuint buffer);
typedef void APIENTRY gl_delete_buffers (GLsizei n, const GLuint *buffers);
typedef void APIENTRY gl_buffer_data (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void APIENTRY gl_enable_vertex_attrib_array (GLuint index);
typedef void APIENTRY gl_vertex_attrib_pointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void APIENTRY gl_draw_arrays (GLenum mode, GLint first, GLsizei count);
typedef void APIENTRY gl_disable_vertex_attrib_array (GLuint index);

// Textures
typedef void APIENTRY gl_active_texture (GLenum texture);

// Uniforms
typedef GLint gl_get_uniform_location(GLuint program, char* uniformName);
typedef void gl_uniform_matrix_3fv(GLuint uniform, GLuint count, GLboolean transpose, GLfloat* data);
typedef void gl_uniform_matrix_4fv(GLuint uniform, GLuint count, GLboolean transpose, GLfloat* data);
typedef void gl_uniform_1fv(GLuint uniform, GLint count, GLfloat* data);
typedef void gl_uniform_2fv(GLuint uniform, GLint count, GLfloat* data);
typedef void gl_uniform_3fv(GLuint uniform, GLint count, GLfloat* data);
typedef void gl_uniform_4fv(GLuint uniform, GLint count, GLfloat* data);
typedef void gl_uniform_1iv(GLuint uniform, GLint count, GLint* data);
typedef void gl_uniform_2iv(GLuint uniform, GLint count, GLint* data);
typedef void gl_uniform_3iv(GLuint uniform, GLint count, GLint* data);
typedef void gl_uniform_4iv(GLuint uniform, GLint count, GLint* data);
typedef void gl_uniform_1i(GLint location, GLint v0);
typedef void gl_uniform_2i(GLint location, GLint v0, GLint v1);
typedef void gl_uniform_3i(GLint location, GLint v0, GLint v1, GLint v2);
typedef void gl_uniform_4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

typedef struct gs_gfx_gl33_ctx
{
  HGLRC RenderContexts[GFX_MAX_WINDOWS];
  
  s32 GLVersionMajor, GLVersionMinor;
  
  wgl_create_context_attribs_arb* wglCreateContextAttribsARB;
  wgl_choose_pixel_format_arb* wglChoosePixelFormatARB;
  wgl_get_extensions_string_ext* wglGetExtensionsStringEXT;
  
  // Debug
  gl_debug_message_callback* glDebugMessageCallback;
  
  // Swap Interval
  wgl_swap_interval_ext* wglSwapIntervalEXT;
  
  // Shaders
  gl_create_shader* glCreateShader;
  gl_shader_source* glShaderSource;
  gl_compile_shader* glCompileShader;
  gl_attach_shader* glAttachShader;
  gl_detach_shader* glDetachShader;
  gl_delete_shader* glDeleteShader;
  gl_create_program* glCreateProgram;
  gl_link_program* glLinkProgram;
  gl_delete_program* glDeleteProgram;
  gl_use_program* glUseProgram;
  gl_get_shaderiv* glGetShaderiv;
  gl_get_shader_info_log* glGetShaderInfoLog;
  gl_get_programiv* glGetProgramiv;
  gl_get_program_info_log* glGetProgramInfoLog;
  
  // VAO & VBO
  gl_gen_vertex_arrays* glGenVertexArrays;
  gl_bind_vertex_array* glBindVertexArray;
  gl_delete_vertex_arrays* glDeleteVertexArrays;
  gl_gen_buffers* glGenBuffers;
  gl_bind_buffer* glBindBuffer;
  gl_delete_buffers* glDeleteBuffers;
  gl_buffer_data* glBufferData;
  gl_enable_vertex_attrib_array* glEnableVertexAttribArray;
  gl_vertex_attrib_pointer* glVertexAttribPointer;
  gl_draw_arrays* glDrawArrays;
  gl_disable_vertex_attrib_array* glDisableVertexAttribArray;
  
  // Textures
  gl_active_texture* glActiveTexture;
  
  // Uniforms
  
  gl_get_uniform_location* glGetUniformLocation;
  gl_uniform_matrix_3fv* glUniformMatrix3fv;
  gl_uniform_matrix_4fv* glUniformMatrix4fv;
  gl_uniform_1fv* glUniform1fv;
  gl_uniform_2fv* glUniform2fv;
  gl_uniform_3fv* glUniform3fv;
  gl_uniform_4fv* glUniform4fv;
  gl_uniform_1iv* glUniform1iv;
  gl_uniform_2iv* glUniform2iv;
  gl_uniform_3iv* glUniform3iv;
  gl_uniform_4iv* glUniform4iv;
  gl_uniform_1i* glUniform1i;
  gl_uniform_2i* glUniform2i;
  gl_uniform_3i* glUniform3i;
  gl_uniform_4i* glUniform4i;
  
} gs_gfx_gl33_ctx;

internal gs_gfx_gl33_ctx*
Win32GLCtx(gs_gfx_handler* GfxH)
{
  GS_GFX_PROFILE_SCOPE;
  
  return (gs_gfx_gl33_ctx*)GfxH->BackendUserData;
}

static void 
APIENTRY Win32GL33DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message, const void* user)
{
  GS_GFX_PROFILE_SCOPE;
  
  char* SeverityStr = 0;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: { SeverityStr = (char*)"HIGH"; } break;
    case GL_DEBUG_SEVERITY_MEDIUM: { SeverityStr = (char*)"MEDIUM"; } break;
    case GL_DEBUG_SEVERITY_LOW: { SeverityStr = (char*)"LOW"; } break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: { SeverityStr = (char*)"NOTIFICATION"; } break;
    default: { SeverityStr = (char*)"UNKNOWN"; } break;
  }
  
  printf("OpenGL Message:\n");
  printf("\tSeverity: %s\n", SeverityStr);
  printf("\t%s\n", message);
  
  if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
  {
    InvalidCodePath;
  }
}

// This just shortens the syntax for loading the proc addresses and assigning them to
// our open gl context struct.
// it expands to things like:
//     GLCtx->glCreateShader = (gl_create_shader*)wglGetProcAddress("glCreateShader");
//     ^ctx   ^proc             ^type                                ^proc
#define LoadExtension(ctx, type, proc) (ctx)->proc = (type*)wglGetProcAddress(Stringify(proc));

internal void
Win32GLLoadExtensions(gs_gfx_gl33_ctx* GLCtx)
{
  GS_GFX_PROFILE_SCOPE;
  
  char* Version = (char*)glGetString(GL_VERSION);
  glGetIntegerv(GL_MAJOR_VERSION, &GLCtx->GLVersionMajor);
  glGetIntegerv(GL_MINOR_VERSION, &GLCtx->GLVersionMinor);
  
  const char* Extensions = GLCtx->wglGetExtensionsStringEXT();
  
  // Debug
  LoadExtension(GLCtx, gl_debug_message_callback, glDebugMessageCallback);
  
  // Swap Interval
  LoadExtension(GLCtx, wgl_swap_interval_ext, wglSwapIntervalEXT);
  
  // Shaders
  LoadExtension(GLCtx, gl_create_shader, glCreateShader);
  LoadExtension(GLCtx, gl_shader_source, glShaderSource);
  LoadExtension(GLCtx, gl_compile_shader, glCompileShader);
  LoadExtension(GLCtx, gl_attach_shader, glAttachShader);
  LoadExtension(GLCtx, gl_detach_shader, glDetachShader);
  LoadExtension(GLCtx, gl_delete_shader, glDeleteShader);
  LoadExtension(GLCtx, gl_create_program, glCreateProgram);
  LoadExtension(GLCtx, gl_link_program, glLinkProgram);
  LoadExtension(GLCtx, gl_delete_program, glDeleteProgram);
  LoadExtension(GLCtx, gl_use_program, glUseProgram);
  LoadExtension(GLCtx, gl_get_shaderiv, glGetShaderiv);
  LoadExtension(GLCtx, gl_get_shader_info_log, glGetShaderInfoLog);
  LoadExtension(GLCtx, gl_get_programiv, glGetProgramiv);
  LoadExtension(GLCtx, gl_get_program_info_log, glGetProgramInfoLog);
  
  // VAO & VBO
  LoadExtension(GLCtx, gl_gen_vertex_arrays, glGenVertexArrays);
  LoadExtension(GLCtx, gl_bind_vertex_array, glBindVertexArray);
  LoadExtension(GLCtx, gl_delete_vertex_arrays, glDeleteVertexArrays);
  LoadExtension(GLCtx, gl_gen_buffers, glGenBuffers);
  LoadExtension(GLCtx, gl_bind_buffer, glBindBuffer);
  LoadExtension(GLCtx, gl_delete_buffers, glDeleteBuffers);
  LoadExtension(GLCtx, gl_buffer_data, glBufferData);
  LoadExtension(GLCtx, gl_enable_vertex_attrib_array, glEnableVertexAttribArray);
  LoadExtension(GLCtx, gl_vertex_attrib_pointer, glVertexAttribPointer);
  LoadExtension(GLCtx, gl_draw_arrays, glDrawArrays);
  LoadExtension(GLCtx, gl_disable_vertex_attrib_array, glDisableVertexAttribArray);
  
  // Textures
  LoadExtension(GLCtx, gl_active_texture, glActiveTexture);
  
  // Uniforms
  LoadExtension(GLCtx, gl_get_uniform_location, glGetUniformLocation);
  LoadExtension(GLCtx, gl_uniform_matrix_3fv, glUniformMatrix3fv);
  LoadExtension(GLCtx, gl_uniform_matrix_4fv, glUniformMatrix4fv);
  LoadExtension(GLCtx, gl_uniform_1fv, glUniform1fv);
  LoadExtension(GLCtx, gl_uniform_2fv, glUniform2fv);
  LoadExtension(GLCtx, gl_uniform_3fv, glUniform3fv);
  LoadExtension(GLCtx, gl_uniform_4fv, glUniform4fv);
  LoadExtension(GLCtx, gl_uniform_1iv, glUniform1iv);
  LoadExtension(GLCtx, gl_uniform_2iv, glUniform2iv);
  LoadExtension(GLCtx, gl_uniform_3iv, glUniform3iv);
  LoadExtension(GLCtx, gl_uniform_4iv, glUniform4iv);
  LoadExtension(GLCtx, gl_uniform_1i, glUniform1i);
  LoadExtension(GLCtx, gl_uniform_2i, glUniform2i);
  LoadExtension(GLCtx, gl_uniform_3i, glUniform3i);
  LoadExtension(GLCtx, gl_uniform_4i, glUniform4i);
}

internal void
Win32GLCtxPushRenderContext(gs_gfx_gl33_ctx* GLCtx, HGLRC RenderContext, gs_window Window)
{
  GS_GFX_PROFILE_SCOPE;
  GLCtx->RenderContexts[Window.WindowId] = RenderContext;
}

internal void
Win32SetPixelFormat(gs_window Window, HDC DeviceContext, gs_gfx_context_desc* Desc, wgl_choose_pixel_format_arb* wglChoosePixelFormatARB, gs_window_context_win32* GfxHandler)
{
  GS_GFX_PROFILE_SCOPE;
  int PixelFormat = 0;
  GLuint ExtendedPick = 0;
  if (wglChoosePixelFormatARB)
  {
    int IntAttribList[] = {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
      WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
      WGL_COLOR_BITS_ARB,     Desc->ColorBits,
      WGL_ALPHA_BITS_ARB,     Desc->AlphaBits,
      WGL_DEPTH_BITS_ARB,     Desc->DepthBits,
      WGL_STENCIL_BITS,       Desc->StencilBits,
      WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
      //WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
      0,
    };
    
    bool Success = wglChoosePixelFormatARB(DeviceContext, IntAttribList, 0, 1, &PixelFormat, &ExtendedPick);
    Assert(Success);
  }
  
  if (!ExtendedPick)
  {
    PIXELFORMATDESCRIPTOR PixelFormatDesc = { 0 };
    PixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    PixelFormatDesc.nVersion = 1;
    PixelFormatDesc.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    PixelFormatDesc.iPixelType = PFD_TYPE_RGBA;// TODO(Peter): include this in win32_opengl_window_info?
    PixelFormatDesc.cColorBits = Desc->ColorBits;
    PixelFormatDesc.cAlphaBits = Desc->AlphaBits;
    PixelFormatDesc.cDepthBits = Desc->DepthBits;
    PixelFormatDesc.dwLayerMask = PFD_MAIN_PLANE; // TODO(Peter): include this in win32_opengl_window_info?
    
    PixelFormat = ChoosePixelFormat(DeviceContext, &PixelFormatDesc);
    if (!PixelFormat) 
    { 
      u32 Error = GetLastError();
      switch (Error)
      {
        InvalidDefaultCase;
      }
    } 
  }
  
  if (PixelFormat) 
  {
    PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
    if (DescribePixelFormat(DeviceContext, PixelFormat, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat))
    {
      if (SetPixelFormat(DeviceContext, PixelFormat, &SuggestedPixelFormat)) 
      {
        // Success
      }
      else
      { 
        u32 Error = GetLastError();
        switch (Error)
        {
          InvalidDefaultCase;
        }
      } 
    }
  }
}

internal void
Win32GLSetWindowCurrent(gs_window Window, gs_gfx_handler GfxHandler)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_window_context_win32* WCtx = (gs_window_context_win32*)GfxHandler.WindowUserData;
  gs_gfx_gl33_ctx* GLCtx = (gs_gfx_gl33_ctx*)GfxHandler.BackendUserData;
  
  HWND WindowHandle = WCtx->WindowHandles[Window.WindowId];
  HDC DeviceContext = GetDC(WindowHandle);
  HGLRC RenderContext = GLCtx->RenderContexts[Window.WindowId];
  
  // if we're trying to set a window's render context to current, it had better
  // HAVE a render context
  Assert(RenderContext != 0);
  
  if (!wglMakeCurrent(DeviceContext, RenderContext))
  {
    s32 Error = GetLastError();
    InvalidCodePath;
  }
}

internal void
Win32WindowCreateGraphicsContext (gs_window Window, gs_gfx_context_desc* Desc, gs_gfx_handler* GfxHandler)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_window_context_win32* WCtx = (gs_window_context_win32*)GfxHandler->WindowUserData;
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxHandler);
  
  // TODO(PS): See if we need to load extensions
  
  // Create a fake render context so we can load the needed OGL Extensions
  // https://www.opengl.org/wiki/Creating_an_OpenGL_Context_(WGL)#Proper_Context_Creation
  wgl_choose_pixel_format_arb* wglChoosePixelFormatARB = 0;
  {
    char Buffer[64];
    SPrintF(Buffer, 64, (char*)"Fake GL Window {s32}", &Window.WindowId);
    gs_window_desc FakeWindowDesc = { .WindowName = Buffer };
    gs_window FakeWindow = Win32WindowOpen(&FakeWindowDesc, (u8*)WCtx);
    HWND FakeWindowHandle = WCtx->WindowHandles[FakeWindow.WindowId];
    HDC FakeDeviceContext = GetDC(FakeWindowHandle);
    Win32SetPixelFormat(FakeWindow, FakeDeviceContext, Desc, 0, WCtx);
    HGLRC FakeRC = wglCreateContext(FakeDeviceContext);
    if (wglMakeCurrent(FakeDeviceContext, FakeRC))
    {
      //Win32GLLoadExtensions(GLCtx);
      LoadExtension(GLCtx, wgl_choose_pixel_format_arb, wglChoosePixelFormatARB);
      LoadExtension(GLCtx, wgl_create_context_attribs_arb, wglCreateContextAttribsARB);
      LoadExtension(GLCtx, wgl_get_extensions_string_ext, wglGetExtensionsStringEXT);
    }
    
    wglDeleteContext(FakeRC);
    ReleaseDC(FakeWindowHandle, FakeDeviceContext);
    
    // Close the fake window
    Win32WindowClose(&FakeWindow, (u8*)WCtx);
  }
  Assert(GLCtx->wglChoosePixelFormatARB != 0);
  
  HWND WindowHandle = WCtx->WindowHandles[Window.WindowId];
  HDC DeviceContext = GetDC(WindowHandle);
  Win32SetPixelFormat(Window, DeviceContext, Desc, GLCtx->wglChoosePixelFormatARB, WCtx);
  
  s32 Flags = 0;
#if DEBUG
  // ask for debug context for non "Release" builds
  // this is so we can enable debug callback
  AddFlag(Flags, WGL_CONTEXT_DEBUG_BIT_ARB);
#endif
  
  const s32 Attributes[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    WGL_CONTEXT_FLAGS_ARB, Flags,
    0, 0, // end of array
  };
  HGLRC RenderContext = 0;
  
  switch (2)
  {
    case 0: { RenderContext = wglCreateContext(DeviceContext); } break;
    case 1: { RenderContext = GLCtx->wglCreateContextAttribsARB(DeviceContext, NULL, 0); } break;
    case 2: { RenderContext = GLCtx->wglCreateContextAttribsARB(DeviceContext, NULL, Attributes); } break;
  }
  
  if (RenderContext == NULL)
  {
    s32 Error = GetLastError();
    InvalidCodePath;
  }
  
  Win32GLCtxPushRenderContext(GLCtx, RenderContext, Window);
  if (wglMakeCurrent(DeviceContext, RenderContext))
  {
    Win32GLLoadExtensions(GLCtx);
    
    if (GLCtx->wglSwapIntervalEXT)
    {
      GLCtx->wglSwapIntervalEXT(1);
    }
  }
  
#if DEBUG
  GLCtx->glDebugMessageCallback(Win32GL33DebugCallback, NULL);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
  
  char* Vendor = (char*)glGetString(GL_VENDOR);
  char* Renderer = (char*)glGetString(GL_RENDERER);
  char* Version = (char*)glGetString(GL_VERSION);
  char* ShadingLanguageVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
  char* Extensions = (char*)GLCtx->wglGetExtensionsStringEXT();
  
  printf("%s\n %s\n %s\n %s\n %s\n", Vendor, Renderer, Version, ShadingLanguageVersion, Extensions);
  
  ReleaseDC(WindowHandle, DeviceContext);
}

internal void
GfxSetViewport(gs_gfx_handler* GfxH, r32 X, r32 Y, r32 W, r32 H)
{
  GS_GFX_PROFILE_SCOPE;
  
  glViewport(X, Y, W, H);
}

internal void
GfxClearBuffer(gs_gfx_handler* GfxH, r32 R, r32 G, r32 B, r32 A, u8 BufferMask)
{
  GS_GFX_PROFILE_SCOPE;
  
  u32 GlBufferMask = 0;
  if (HasFlag(BufferMask, GFXBufferMask_Color))        AddFlag(GlBufferMask, GL_COLOR_BUFFER_BIT);
  if (HasFlag(BufferMask, GFXBufferMask_Depth))        AddFlag(GlBufferMask, GL_DEPTH_BUFFER_BIT);
  if (HasFlag(BufferMask, GFXBufferMask_Accumulation)) AddFlag(GlBufferMask, GL_ACCUM_BUFFER_BIT);
  if (HasFlag(BufferMask, GFXBufferMask_Stencil))      AddFlag(GlBufferMask, GL_STENCIL_BUFFER_BIT);
  
  glClearColor(R, G, B, A);
  glClear(GlBufferMask);
}

internal void
GfxSetMatrix(gs_gfx_handler* GfxH, u8 Matrix, m44 Value)
{
  GS_GFX_PROFILE_SCOPE;
  
  //Win32GL33VersionCheck(GfxH, 0, 0, 3, 2, "Fixed Functions deprecated in 3.2");
  
  switch (Matrix)
  {
    case GfxMatrix_ModelView:
    {
      glMatrixMode(GL_MODELVIEW);
    } break;
    
    case GfxMatrix_Projection:
    {
      glMatrixMode(GL_PROJECTION);
    } break;
    
    default: 
    {
      InvalidCodePath;
    } break;
  }
  
  glLoadMatrixf(M44Transpose(Value).E);
}

internal void
GfxDrawTriangle(gs_gfx_handler* GfxH, v4 P0, v4 P1, v4 P2, v2 UV0, v2 UV1, v2 UV2, v4 C0, v4 C1, v4 C2)
{
  GS_GFX_PROFILE_SCOPE;
  
  // NOTE(PS): I'm not sure this is wrong, but passing in
  // vectors rather than points (ie. w == 0 vs w == 1) 
  // is technically incorrect - and its usually not what I 
  // want, but is really easy to miss as a mistake.
  Assert(P0.w != 0 && P1.w != 0 && P2.w != 0);
  
  glBegin(GL_TRIANGLES);
  {
    glTexCoord2f(UV0.x, UV0.y);
    glColor4f(C0.r, C0.g, C0.b, C0.a);
    glVertex4f(P0.x, P0.y, P0.z, P0.w);
    
    glTexCoord2f(UV1.x, UV1.y);
    glColor4f(C1.r, C1.g, C1.b, C1.a);
    glVertex4f(P1.x, P1.y, P1.z, P1.w);
    
    glTexCoord2f(UV2.x, UV2.y);
    glColor4f(C2.r, C2.g, C2.b, C2.a);
    glVertex4f(P2.x, P2.y, P2.z, P2.w);
  }
  glEnd();
}

internal void
GfxDrawTriangleList(gs_gfx_handler* GfxH, v4* Vertices, v2* UVs, v4* Colors, r32 VerticesCount)
{
  GS_GFX_PROFILE_SCOPE;
  
  r32 VertexElements = 4;
  r32 VertexStride = VertexElements * sizeof(r32);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(VertexElements, GL_FLOAT, VertexStride, Vertices);
  
  if (UVs != 0)
  {
    r32 UVElements = 2;
    r32 UVStride = UVElements * sizeof(r32);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(UVElements, GL_FLOAT, UVStride, UVs);
  }
  
  if (Colors != 0)
  {
    r32 ColorElements = 4;
    r32 ColorStride = ColorElements * sizeof(r32);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(ColorElements, GL_FLOAT, ColorStride, Colors);
  }
  
  glDrawArrays(GL_TRIANGLES, 0, VerticesCount);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

typedef struct gs_gfx_shader_compilation_result
{
  gs_gfx_shader_type ShaderType;
  b32 IsValid;
  u32 BackendHandle;
  char* InfoLog;
  s32 InfoLogLength;
} gs_gfx_shader_compilation_result;

internal gs_gfx_shader_compilation_result
GfxCompileShader(gs_gfx_handler* GfxH, gs_gfx_shader_type ShaderType, char* ShaderSource, gs_memory_arena* Temp)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxH);
  
  gs_gfx_shader_compilation_result Result = {
    .ShaderType = ShaderType,
  };
  
  GLenum GlShaderType = 0;
  switch (ShaderType)
  {
    case GfxShader_Vertex: { GlShaderType = GL_VERTEX_SHADER; } break;
    case GfxShader_Fragment: { GlShaderType = GL_FRAGMENT_SHADER; } break;
    InvalidDefaultCase;
  }
  
  Result.BackendHandle = GLCtx->glCreateShader(GlShaderType);
  
  GLCtx->glShaderSource(Result.BackendHandle, 1, (const GLchar* const*)&ShaderSource, NULL);
  GLCtx->glCompileShader(Result.BackendHandle);
  
  Result.IsValid = GL_FALSE;
  Result.InfoLogLength = 0;
  GLCtx->glGetShaderiv(Result.BackendHandle, GL_COMPILE_STATUS, &Result.IsValid);
  GLCtx->glGetShaderiv(Result.BackendHandle, GL_INFO_LOG_LENGTH, &Result.InfoLogLength);
  if (Result.InfoLogLength > 0)
  {
    Result.InfoLog = PushArray(Temp, char, Result.InfoLogLength);
    GLCtx->glGetShaderInfoLog(Result.BackendHandle, Result.InfoLogLength, NULL, Result.InfoLog);
  }
  return Result;
}

typedef struct gs_gfx_shader_link_result
{
  u32 BackendHandleProgram;
  u32 BackendHandleVertexShader;
  u32 BackendHandleFragmentShader;
  
  b32 IsValid;
  s32 InfoLogLength;
  char* InfoLog;
} gs_gfx_shader_link_result;

internal gs_gfx_shader_link_result
GfxLinkShaderProgram(gs_gfx_handler* GfxH, u32 BackendHandleVertexShader, u32 BackendHandleFragmentShader, gs_memory_arena* Temp)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxH);
  
  gs_gfx_shader_link_result Result = {
    .BackendHandleVertexShader = BackendHandleVertexShader,
    .BackendHandleFragmentShader = BackendHandleFragmentShader,
  };
  
  Result.BackendHandleProgram = GLCtx->glCreateProgram(),
  GLCtx->glAttachShader(Result.BackendHandleProgram, BackendHandleVertexShader);
  GLCtx->glAttachShader(Result.BackendHandleProgram, BackendHandleFragmentShader);
  GLCtx->glLinkProgram(Result.BackendHandleProgram);
  
  Result.IsValid = GL_FALSE;
  Result.InfoLogLength = 0;
  GLCtx->glGetProgramiv(Result.BackendHandleProgram, GL_LINK_STATUS, &Result.IsValid);
  GLCtx->glGetProgramiv(Result.BackendHandleProgram, GL_INFO_LOG_LENGTH, &Result.InfoLogLength);
  if (Result.InfoLogLength > 0)
  {
    Result.InfoLog = PushArray(Temp, char, Result.InfoLogLength);
    GLCtx->glGetProgramInfoLog(Result.BackendHandleProgram, Result.InfoLogLength, NULL, Result.InfoLog);
  }
  
  //GLCtx->glDetachShader(Result.BackendHandleProgram, BackendHandleVertexShader);
  //GLCtx->glDetachShader(Result.BackendHandleProgram, BackendHandleFragmentShader);
  return Result;
}

internal void
GfxDeleteShader(gs_gfx_handler* GfxH, u32 BackendShaderHandle)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxH);
  GLCtx->glDeleteShader(BackendShaderHandle);
}

// TODO(PS): This can probably live at the gs_gfx.h level - it doesn't call any system specific procedures
internal gs_gfx_shader_program
GfxShaderProgramCompileAndLink(gs_gfx_handler* GfxH, char* VertexShaderSrc, char* FragmentShaderSrc, gs_memory_arena* Temp)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_shader_compilation_result VertexShader = GfxCompileShader(GfxH, GfxShader_Vertex, VertexShaderSrc, Temp);
  gs_gfx_shader_compilation_result FragmentShader = GfxCompileShader(GfxH, GfxShader_Fragment, FragmentShaderSrc, Temp);
  gs_gfx_shader_link_result ProgramResult = GfxLinkShaderProgram(GfxH, 
                                                                 VertexShader.BackendHandle,
                                                                 FragmentShader.BackendHandle,
                                                                 Temp);
  GfxDeleteShader(GfxH, VertexShader.BackendHandle);
  GfxDeleteShader(GfxH, FragmentShader.BackendHandle);
  
  gs_gfx_shader_program Result = { 
    .BackendHandle = ProgramResult.BackendHandleProgram,
  };
  return Result;
}

internal void
GfxShaderProgramActivate(gs_gfx_handler* GfxH, gs_gfx_shader_program Program)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxH);
  GLCtx->glUseProgram(Program.BackendHandle);
}

internal void
GfxUniformMatrixSet(gs_gfx_handler* GfxH, gs_gfx_shader_program Program, char* UniformName, m44 Matrix)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxH);
  GLint MatrixLocation = GLCtx->glGetUniformLocation(Program.BackendHandle, UniformName);
  Assert(MatrixLocation >= 0);
  GLboolean glTranspose = GL_TRUE;
  GLCtx->glUniformMatrix4fv(MatrixLocation, 1, glTranspose, Matrix.E);
}

internal gs_gfx_vertex_buffer_handle
GfxModelCreate(gs_gfx_handler* GfxH, gs_gfx_model_desc* Desc)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxH);
  gs_gfx_vertex_buffer_handle Result = {
    .VertexCount = Desc->VertexCount,
    .IndicesCount = Desc->IndicesCount,
  };
  
  GLCtx->glGenVertexArrays(1, &Result.VertexArrayID);
  Assert(Result.VertexArrayID != GL_INVALID_VALUE);
  
  GLCtx->glGenBuffers(1, &Result.VertexBufferID);
  Assert(Result.VertexBufferID != GL_INVALID_VALUE);
  
  GLCtx->glGenBuffers(1, &Result.IndicesBufferID);
  Assert(Result.IndicesBufferID != GL_INVALID_VALUE);
  
  GLCtx->glBindVertexArray(Result.VertexArrayID);
  {
    GLCtx->glBindBuffer(GL_ARRAY_BUFFER, Result.VertexBufferID);
    GLCtx->glBufferData(GL_ARRAY_BUFFER, Desc->VertexDataSize, (u8*)Desc->VertexData, GL_STATIC_DRAW);
    
    GLCtx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Result.IndicesBufferID);
    GLCtx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, Desc->IndicesCount * sizeof(u32), (u8*)Desc->Indices, GL_STATIC_DRAW);
    
    bool HasPosData = false;
    bool HasTexData = false;
    bool HasColorData = false;
    switch (Desc->DataOrder)
    {
      case VertexData_Pos: 
      {
        HasPosData = true;
      } break;
      
      case VertexData_PosTex: 
      {
        HasPosData = true;
        HasTexData = true;
      }break;
      
      case VertexData_PosCol: 
      {
        HasPosData = true;
        HasColorData = true;
      }break;
      
      case VertexData_PosTexCol: 
      case VertexData_PosColTex: 
      {
        HasPosData = true;
        HasTexData = true;
        HasColorData = true;
      }break;
      
      InvalidDefaultCase;
    }
    
    s32 AttrIndex_Position = 0;
    s32 AttrIndex_TexCoord = 1;
    s32 AttrIndex_Color    = 2;
    
    if (HasPosData)
    {
      GLCtx->glEnableVertexAttribArray(AttrIndex_Position);
      GLCtx->glVertexAttribPointer(AttrIndex_Position, // position in layout (matches layout position in shader)
                                   4, // vertex elements
                                   GL_FLOAT, // type
                                   GL_FALSE, // normalize?
                                   Desc->VertexStride * sizeof(r32), // stride
                                   (void*)(Desc->VertexOffset * sizeof(r32)) // array buffer offset
                                   );
    }
    
    if (HasTexData)
    {
      GLCtx->glEnableVertexAttribArray(AttrIndex_TexCoord);
      GLCtx->glVertexAttribPointer(AttrIndex_TexCoord, // position in layout (matches layout position in shader)
                                   2, // vertex elements
                                   GL_FLOAT, // type
                                   GL_FALSE, // normalize?
                                   Desc->TexCoordStride * sizeof(r32), // stride
                                   (void*)(Desc->TexCoordOffset * sizeof(r32)) // array buffer offset
                                   );
    }
    
    if (HasColorData)
    {
      GLCtx->glEnableVertexAttribArray(AttrIndex_Color);
      GLCtx->glVertexAttribPointer(AttrIndex_Color, // position in layout (matches layout position in shader)
                                   4, // vertex elements
                                   GL_FLOAT, // type
                                   GL_FALSE, // normalize?
                                   Desc->VertexColorStride * sizeof(r32), // stride
                                   (void*)(Desc->VertexColorOffset * sizeof(r32)) // array buffer offset
                                   );
    }
  }
  
  return Result;
}

internal void
GfxVertexBufferDraw(gs_gfx_handler* GfxH, gs_gfx_vertex_buffer_handle Buffer)
{
  GS_GFX_PROFILE_SCOPE;
  
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GfxH);
  
  GLCtx->glBindVertexArray(Buffer.VertexArrayID);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Textures

// TODO(PS): This is really slow when we could just, per backend,
// define these enums with values that match the system values
// Actually, at that point, they don't even have to be enums
//
internal GLenum
Win32GL33TextureFilter(gs_gfx_texture_filter F)
{
  GLenum Result = 0;
  
  switch (F)
  {
    case TextureFilter_Nearest: { Result = GL_NEAREST; } break;
    case TextureFilter_Linear: { Result = GL_LINEAR; } break;
    InvalidDefaultCase;
  }
  
  return Result;
}

internal gs_gfx_texture_handle
GfxTextureLoad(gs_gfx_handler* GH, gs_gfx_texture_desc* Desc)
{
  GS_GFX_PROFILE_SCOPE;
  gs_gfx_texture_handle Result = {};
  
  glGenTextures(1, &Result.Handle);
  glBindTexture(GL_TEXTURE_2D, Result.Handle);
  
  GLenum TextureFormat;
  switch (Desc->BackendTextureFormat)
  {
    case TextureFormat_RGBA8: { TextureFormat = GL_RGBA8; } break;
    case TextureFormat_Alpha: { TextureFormat = GL_ALPHA; } break;
    InvalidDefaultCase;
  }
  
  glTexImage2D(GL_TEXTURE_2D, 
               0, // level of detail number, zero is base
               TextureFormat, // opengl internal format
               Desc->W,
               Desc->H,
               0, // border - must be zero
               GL_RGBA, // format of pixel data
               GL_UNSIGNED_BYTE, // format of channel data
               Desc->Data);
  
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Win32GL33TextureFilter(Desc->MagFilter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Win32GL33TextureFilter(Desc->MinFilter));
  
  return Result;
}

internal void 
GfxUniformTextureSet(gs_gfx_handler* GH, gs_gfx_shader_program Program, gs_gfx_texture_handle Handle, char* UniformName)
{
  GS_GFX_PROFILE_SCOPE;
  gs_gfx_gl33_ctx* GLCtx = Win32GLCtx(GH);
  
  GLint TextureLocation = GLCtx->glGetUniformLocation(Program.BackendHandle, UniformName);
  Assert(TextureLocation >= 0);
  
  // TODO(PS): this means we're always binding every texture to TEXTURE0, fix this!
  s32 TextureIndex = 0;
  GLCtx->glUniform1i(TextureLocation, TextureIndex); 
  GLCtx->glActiveTexture(GL_TEXTURE0 + TextureIndex);
  glBindTexture(GL_TEXTURE_2D, Handle.Handle);
}

//
// Backend
//

internal void
GfxHandlerGetBackendCmds(gs_gfx_handler* GfxHandler, gs_allocator A)
{
  GS_GFX_PROFILE_SCOPE;
  
  GfxHandler->PWindowCreateGraphicsContext = (platform_window_create_graphics_context*)Win32WindowCreateGraphicsContext;
  GfxHandler->PBackendSetWindowCurrent = (platform_backend_set_window_current*)Win32GLSetWindowCurrent;
  
  gs_gfx_gl33_ctx* GLCtx = AllocStruct(A, gs_gfx_gl33_ctx, "root");
  GfxHandler->BackendUserData = (u8*)GLCtx;
}

#endif //GS_GFX_GL33_H
