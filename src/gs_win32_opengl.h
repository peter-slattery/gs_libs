//
// File: win32_opengl.h
// Author: Peter Slattery
// Creation Date: 2020-03-14
//
// USAGE:
//
// NOTE: Must #include <gl/gl.h> before this file
// NOTE: Must link with opengl32.lib, gdi32.lib
//
#ifndef WIN32_OPENGL_H

#ifndef GS_TYPES_H
# error "gs_win32_opengl.h relies on gs_types.h. Please include it first."
#endif

#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

#define GL_TEXTURE0                       0x84C0

#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_SUPPORT_GDI_ARB               0x200F
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_FULL_ACCELERATION_ARB         0x2027


typedef signed long long int GLsizeiptr;

typedef void gl_gen_buffers(GLsizei NumBufferObjects, GLuint* Buffers);
typedef void gl_bind_buffer(GLenum Target, GLuint Buffer);
typedef void gl_buffer_data(GLenum target, GLsizeiptr size, const void * data, GLenum usage);
typedef GLuint gl_create_shader(GLenum shaderType);
typedef void gl_shader_source(GLuint shader, GLsizei count, const char **string, const GLint *length);
typedef void gl_compile_shader(GLuint shader);
typedef void gl_get_shader_iv(GLuint shader, GLenum pname, GLint *params);
typedef void gl_get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei *length, char *infoLog);
typedef void gl_attach_shader(GLuint program, GLuint shader);
typedef void gl_delete_shader(GLuint shader);
typedef GLuint gl_create_program();
typedef void gl_delete_program(GLuint);
typedef void gl_link_program(GLuint program);
typedef void gl_get_program_iv(GLuint program, GLenum pname, GLint *params);
typedef void gl_get_program_info_log(GLuint program, GLsizei maxLength, GLsizei *length, char *infoLog);
typedef void gl_use_program(GLuint program);
typedef void gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
typedef void gl_vertex_attrib_ipointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void gl_enable_vertex_attrib_array(GLuint index);
typedef void gl_disable_vertex_attrib_array(GLuint index);
typedef void gl_gen_vertex_arrays(GLsizei n, GLuint *arrays);
typedef void gl_bind_vertex_array(GLuint array);
typedef void gl_active_texture(GLenum texture);

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

// FrameBuffers
typedef void gl_gen_framebuffers(GLuint count, GLuint* buffer);
typedef GLenum gl_check_framebuffer_status(GLuint target);
typedef void gl_bind_framebuffer(GLenum target, GLuint buffer);
typedef void gl_delete_framebuffers(GLuint count, GLuint* buffers);
typedef void gl_framebuffer_texture_2d(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

typedef BOOL wgl_choose_pixel_format_arb(HDC Hdc, const int* AttributeList, const FLOAT* AttributeFList, UINT MaxFormats, int* Formats, UINT* NumFormats);
typedef HGLRC wgl_create_context_attribs_arb(HDC Hdc, HGLRC ShareContext, const int* AttribsList);
typedef const char* WINAPI wgl_get_extensions_string_ext(void);

struct gswo_procs
{
    wgl_choose_pixel_format_arb* wglChoosePixelFormatARB;
    wgl_create_context_attribs_arb* wglCreateContextAttribsARB;
    wgl_get_extensions_string_ext* wglGetExtensionsStringEXT;
    
    gl_gen_buffers* glGenBuffers;
    gl_bind_buffer* glBindBuffer;
    gl_buffer_data* glBufferData;
    gl_create_shader* glCreateShader;
    gl_shader_source* glShaderSource;
    gl_compile_shader* glCompileShader;
    gl_get_shader_iv* glGetShaderiv;
    gl_get_shader_info_log* glGetShaderInfoLog;
    gl_attach_shader* glAttachShader;
    gl_delete_shader* glDeleteShader;
    gl_create_program* glCreateProgram;
    gl_delete_program* glDeleteProgram;
    gl_link_program* glLinkProgram;
    gl_get_program_iv* glGetProgramiv;
    gl_get_program_info_log* glGetProgramInfoLog;
    gl_use_program* glUseProgram;
    gl_vertex_attrib_pointer* glVertexAttribPointer;
    gl_vertex_attrib_ipointer* glVertexAttribIPointer;
    gl_enable_vertex_attrib_array* glEnableVertexAttribArray;
    gl_disable_vertex_attrib_array* glDisableVertexAttribArray;
    gl_gen_vertex_arrays* glGenVertexArrays;
    gl_bind_vertex_array* glBindVertexArray;
    gl_active_texture* glActiveTexture;
    
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
    
    gl_gen_framebuffers* glGenFramebuffers;
    gl_check_framebuffer_status* glCheckFramebufferStatus;
    gl_bind_framebuffer* glBindFramebuffer;
    gl_delete_framebuffers* glDeleteFramebuffers;
    gl_framebuffer_texture_2d* glFramebufferTexture2D;
};

struct gswo_render_context
{
    HGLRC RenderContext;
    HDC DeviceContext;
};

struct gswo_shader_program
{
    u32 ShaderProgram;
};

struct gswo_model
{
    u32 VertexArray;
    u32 VertexBuffer;
    u32 IndexBuffer;
    u32 IndexCount;
};

struct gswo_texture
{
    u32 Handle;
};

//
// Fixed Function Pipeline
//

// TODO(Peter): I want some way of knowing that OpenGL version being used is below 3.2
// because OpenGL just fails silently if you use fixed-function calls on > 3.1


//
// Model
//

static void
gswo_SetEnableVertexAttribute(gswo_procs OpenGL, u32 VertexStride, u32 AttrIndex, gsr_vertex_attribute Attr)
{
    switch (Attr.ElementType)
    {
        case gs_BasicType_r32:
        {
            b32 Normalize = Attr.Normalize ? GL_TRUE : GL_FALSE;
            OpenGL.glVertexAttribPointer(AttrIndex, Attr.ElementCount, GL_FLOAT, Normalize, VertexStride, (void*)Attr.Offset);
        }break;
        
        case gs_BasicType_s32:
        {
            OpenGL.glVertexAttribIPointer(AttrIndex, Attr.ElementCount, GL_INT, VertexStride, (void*)Attr.Offset);
        }break;
        
        case gs_BasicType_u32:
        {
            OpenGL.glVertexAttribIPointer(AttrIndex, Attr.ElementCount, GL_UNSIGNED_INT, VertexStride, (void*)Attr.Offset);
        }break;
        
        // for doubles, if you ever need them, see glVertexAttribLPointer here:
        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
        InvalidDefaultCase;
    }
    OpenGL.glEnableVertexAttribArray(AttrIndex);
}
static void
gswo_BindModel(gswo_procs OpenGL, gswo_model Model)
{
    OpenGL.glBindVertexArray(Model.VertexArray);
}
static u8*
gswo_CreateModel(gswo_procs OpenGL, gs_memory_arena* Arena, gsr_model RendererModel)
{
    gswo_model* Model = PushStruct(Arena, gswo_model);
    OpenGL.glGenVertexArrays(1, &Model->VertexArray);
    OpenGL.glGenBuffers(1, &Model->VertexBuffer);
    OpenGL.glGenBuffers(1, &Model->IndexBuffer);
    return (u8*)Model;
}
static void
gswo_SetBufferData(gswo_procs OpenGL, int Buffer, int BufferType, int Size, u8* Data, int DrawType)
{
    OpenGL.glBindBuffer(BufferType, Buffer);
    OpenGL.glBufferData(BufferType, Size, Data, DrawType);
}
static void
gswo_SetModelData(gswo_procs OpenGL, gsr_render_asset* ModelAsset, gsr_set_model_data ModelData)
{
    gsr_model RendererModel = ModelAsset->Model;
    gswo_model* Model = (gswo_model*)ModelAsset->BackendAssetData;
    
    gswo_BindModel(OpenGL, *Model);
    for (u32 i = 0; i < RendererModel.VertexAttributesCount; i++)
    {
        gswo_SetEnableVertexAttribute(OpenGL, RendererModel.VertexStride, i, RendererModel.VertexAttributes[i]);
    }
    
    gswo_BindModel(OpenGL, *Model);
    gswo_SetBufferData(OpenGL, Model->VertexBuffer, GL_ARRAY_BUFFER, ModelData.VertexBufferSize, ModelData.VertexBuffer, GL_STATIC_DRAW);
    gswo_SetBufferData(OpenGL, Model->IndexBuffer, GL_ELEMENT_ARRAY_BUFFER, ModelData.IndexBufferSize, ModelData.IndexBuffer, GL_STATIC_DRAW);
    
    Model->IndexCount = ModelData.IndexBufferSize / sizeof(u32);
}
static void
gswo_DrawModel(gswo_procs OpenGL, gswo_model Model)
{
    glDrawElements(GL_TRIANGLES, Model.IndexCount, GL_UNSIGNED_INT, 0);
}

static void
gswo_SetMatrix44(gswo_procs OpenGL, u32 MatrixLocation, r32* Matrix, b32 Transpose = false)
{
    GLboolean glTranspose = Transpose ? GL_TRUE : GL_FALSE;
    OpenGL.glUniformMatrix4fv(MatrixLocation, 1, glTranspose, Matrix);
}
static void
gswo_SetFloat(gswo_procs OpenGL, u32 FloatLocation, r32 Value)
{
    OpenGL.glUniform1fv(FloatLocation, 1, &Value);
}

//
// Texture
//

static void
gswo_SetTextureParameters(u32 Repeat, u32 MinFilter, u32 MagFilter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Repeat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Repeat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);
}

static void
gswo_SetTextureData(gsr_render_asset* Asset, u8* TextureData, u32 Width, u32 Height, u32 Channels)
{
    gswo_texture* Texture = (gswo_texture*)Asset->BackendAssetData;
    
    u32 PixelFormat = GL_RGB;
    if (Channels == 4)
    {
        PixelFormat = GL_RGBA;
    }
    
    glBindTexture(GL_TEXTURE_2D, Texture->Handle);
    glTexImage2D(GL_TEXTURE_2D, 0, PixelFormat, Width, Height, 0, PixelFormat, GL_UNSIGNED_BYTE, TextureData);
}
static void
gswo_SetTextureData(gsr_render_asset* Asset, gsr_set_texture_data TextureData)
{
    gswo_SetTextureData(Asset, TextureData.Data, TextureData.Width, TextureData.Height, TextureData.Channels);
}

static u8*
gswo_CreateTexture(gs_memory_arena* Arena)
{
    gswo_texture* Texture = PushStruct(Arena, gswo_texture);
    glGenTextures(1, &Texture->Handle);
    glBindTexture(GL_TEXTURE_2D, Texture->Handle);
    gswo_SetTextureParameters(GL_REPEAT, GL_LINEAR, GL_LINEAR);
    return (u8*)Texture;
}

static u8*
gswo_CreateDepthStencilTexture(u32 Width, u32 Height, gs_memory_arena* Arena)
{
    gswo_texture* Texture = PushStruct(Arena, gswo_texture);
    glGenTextures(1, &Texture->Handle);
    glBindTexture(GL_TEXTURE_2D, Texture->Handle);
    gswo_SetTextureParameters(GL_REPEAT, GL_LINEAR, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Width, Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    return (u8*)Texture;
}

//
// Shaders
//

static u32
gswo_CompileShader(char* Source, u32 ShaderType, gswo_procs OpenGL, gs_string* ErrorLog)
{
    u32 Result = OpenGL.glCreateShader(ShaderType);
    OpenGL.glShaderSource(Result, 1, (const char**)&Source, NULL);
    OpenGL.glCompileShader(Result);
    s32 Success = 0;
    OpenGL.glGetShaderiv(Result, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        if (ErrorLog && ErrorLog->Size > 0)
        {
            OpenGL.glGetShaderInfoLog(Result, ErrorLog->Size, (GLsizei*)&ErrorLog->Length, ErrorLog->Str);
        }
        OpenGL.glDeleteShader(Result);
        Result = 0;
    }
    return Result;
}

static u32
gswo_LinkProgram(u32 VertexShader, u32 FragmentShader, gswo_procs OpenGL, gs_string* ErrorLog)
{
    u32 Result = OpenGL.glCreateProgram();
    OpenGL.glAttachShader(Result, VertexShader);
    OpenGL.glAttachShader(Result, FragmentShader);
    OpenGL.glLinkProgram(Result);
    
    s32 Success = 0;
    OpenGL.glGetProgramiv(Result, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        if (ErrorLog && ErrorLog->Size > 0)
        {
            OpenGL.glGetProgramInfoLog(Result, ErrorLog->Size, (GLsizei*)&ErrorLog->Length, ErrorLog->Str);
        }
        OpenGL.glDeleteProgram(Result);
        Result = 0;
    }
    return Result;
}

static void
gswo_CompileAndLinkProgram(gswo_procs OpenGL, gsr_render_asset* ShaderAsset, char* VertexShader, char* FragmentShader,
                           gs_string* ErrorLog)
{
    gswo_shader_program* ShaderProgram = (gswo_shader_program*)ShaderAsset->BackendAssetData;
    
    u32 VertexShaderID = gswo_CompileShader(VertexShader, GL_VERTEX_SHADER, OpenGL, ErrorLog);
    Assert(VertexShaderID);
    
    u32 FragmentShaderID = gswo_CompileShader(FragmentShader, GL_FRAGMENT_SHADER, OpenGL, ErrorLog);
    Assert(FragmentShaderID);
    
    ShaderProgram->ShaderProgram = gswo_LinkProgram(VertexShaderID, FragmentShaderID, OpenGL, ErrorLog);
    Assert(ShaderProgram->ShaderProgram);
    
    OpenGL.glDeleteShader(VertexShaderID);
    OpenGL.glDeleteShader(FragmentShaderID);
}
static void
gswo_CompileAndLinkProgram(gswo_procs OpenGL, gsr_render_asset* ShaderAsset, gsr_compile_shader ShaderCode, gs_string* ErrorLog)
{
    gswo_CompileAndLinkProgram(OpenGL, ShaderAsset, ShaderCode.VertexShader, ShaderCode.FragmentShader, ErrorLog);
}

static u8*
gswo_CreateShaderProgram(gswo_procs OpenGL, gs_memory_arena* Arena)
{
    gswo_shader_program* Result = PushStruct(Arena, gswo_shader_program);
    return (u8*)Result;
}
//
// Frame Buffers
//

static u32
gswo_CreateFramebuffer(gswo_procs Procs)
{
    u32 Framebuffer = 0;
    Procs.glGenFramebuffers(1, &Framebuffer);
    return Framebuffer;
}

static void
gswo_BindFramebuffer(gswo_procs Procs, u32 Framebuffer)
{
    Procs.glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
}

#if 0
static bool
gswo_CreateRenderbuffer(gswo_procs Procs, u32 Width, u32 Height, u32* Framebuffer, u32* ColorTexture, u32* DepthStencilTexture, gs_memory_arena* Arena)
{
    bool Result = false;
    
    *Framebuffer = gswo_CreateFramebuffer(Procs);
    gswo_BindFramebuffer(Procs, *Framebuffer);
    
    *ColorTexture = gswo_CreateTexture(NULL, Width, Height, 3, Arena);
    Procs.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *ColorTexture, 0);
    
    *DepthStencilTexture= gswo_CreateDepthStencilTexture(Width, Height, Arena);
    Procs.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *DepthStencilTexture, 0);
    
    Result = (Procs.glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    gswo_BindFramebuffer(Procs, 0);
    return Result;
    
}
#endif

static void
gswo_Clear(gsr_clear Clear)
{
    glClearColor(Clear.R, Clear.G, Clear.B, Clear.A);
    // TODO(Peter): Probably want to provide more options
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void
gswo_UseShader(gswo_procs Procs, gsr_render_asset* Asset)
{
    Assert(Asset->BackendAssetData); // TODO(Peter): use a placeholder asset
    gswo_shader_program* Shader = (gswo_shader_program*)Asset->BackendAssetData;
    Procs.glUseProgram(Shader->ShaderProgram);
}

static void
gswo_BindTexture(gswo_procs Procs, gsr_render_asset* ShaderAsset, gsr_render_asset* TextureAsset, u32 TextureIndex, char* UniformName)
{
    u32 TextureHandle = 0;
    if (TextureAsset)
    {
        Assert(TextureAsset->BackendAssetData);
        gswo_texture* Texture = (gswo_texture*)TextureAsset->BackendAssetData;
        TextureHandle = Texture->Handle;
    }
    
    Assert(ShaderAsset->BackendAssetData);
    gswo_shader_program* Shader = (gswo_shader_program*)ShaderAsset->BackendAssetData;
    s32 UniformLocation = Procs.glGetUniformLocation(Shader->ShaderProgram, UniformName);
    Assert(UniformLocation >= 0);
    Procs.glUniform1i(UniformLocation, TextureIndex);
    
    Procs.glActiveTexture(GL_TEXTURE0 + TextureIndex);
    glBindTexture(GL_TEXTURE_2D, TextureHandle);
}

static void
gswo_BindModel(gswo_procs Procs, gsr_render_asset* Asset)
{
    gswo_model* Model = (gswo_model*)Asset->BackendAssetData;
    Procs.glBindVertexArray(Model->VertexArray);
}

static void
gswo_RenderModel(gsr_render_asset* Asset)
{
    gswo_model* Model = (gswo_model*)Asset->BackendAssetData;
    glDrawElements(GL_TRIANGLES, Model->IndexCount, GL_UNSIGNED_INT, 0);
}

//
// Immediate Mode
//

internal void
gswo_ImmediateDrawTri(v3 V0, v3 V1, v3 V2, v2 UV0, v2 UV1, v2 UV2, v4 Color)
{
    glBegin(GL_TRIANGLES);
    
    glColor4fv(Color.E);
    
    glTexCoord2fv(UV0.E);
    glVertex3fv(V0.E);
    
    glTexCoord2fv(UV1.E);
    glVertex3fv(V1.E);
    
    glTexCoord2fv(UV2.E);
    glVertex3fv(V2.E);
    
    glEnd();
}

internal void
gswo_ImmediateDrawQuad(v3 V0, v3 V1, v3 V2, v3 V3, v2 UV0, v2 UV1, v2 UV2, v2 UV3, v4 Color)
{
    gswo_ImmediateDrawTri(V0, V1, V2, UV0, UV1, UV2, Color);
    gswo_ImmediateDrawTri(V0, V2, V3, UV0, UV2, UV3, Color);
}
internal void
gswo_ImmediateDrawQuad(v3 V0, v3 V1, v3 V2, v3 V3, v4 Color)
{
    gswo_ImmediateDrawQuad(V0, V1, V2, V3, v2{0,0}, v2{1,0}, v2{1,1}, v2{0,1}, Color);
}

//
// Program Cycle
//

#define GetOpenGLProcAddress(container, type, name) \
container->name = (type*)wglGetProcAddress(#name); \
if (!container->name) { \
InvalidCodePath; \
} else {}

static void
gswo_PrintOpenGLError()
{
    u32 Error = glGetError();
    printf("    OpenGL Error: %d\n", glGetError());
    switch (Error)
    {
        case GL_NO_ERROR:
        {
            printf("    OpenGL Error: %d GL_NO_ERROR\n", glGetError());
        }break;
        
        case GL_INVALID_ENUM:
        {
            printf("    OpenGL Error: %d GL_INVALID_ENUM\n", glGetError());
        }break;
        
        case GL_INVALID_VALUE:
        {
            printf("    OpenGL Error: %d GL_INVALID_VALUE\n", glGetError());
        }break;
        
        case GL_INVALID_OPERATION:
        {
            printf("    OpenGL Error: %d GL_INVALID_OPERATION\n", glGetError());
        }break;
        
        case GL_OUT_OF_MEMORY:
        {
            printf("    OpenGL Error: %d GL_OUT_OF_MEMORY\n", glGetError());
        }break;
        
        case GL_STACK_UNDERFLOW:
        {
            printf("    OpenGL Error: %d GL_STACK_UNDERFLOW\n", glGetError());
        }break;
        
        case GL_STACK_OVERFLOW:
        {
            printf("    OpenGL Error: %d GL_STACK_OVERFLOW\n", glGetError());
        }break;
        
        default:
        {
            printf("    OpenGL Error: %d\n", glGetError());
        }break;
    }
}

#define gswo_PrintLastError(message) gswo_PrintLastError_(message, __FILE__, __LINE__)
static void
gswo_PrintLastError_(char* Message, char* File, u32 Line)
{
    printf("%s Line %d: %s\n", File, Line, Message);
    printf("    OpenGL Error: %d\n", glGetError());
    u32 Error = GetLastError();
    printf("    Win32 Error: %u\n", Error);
}

static void
gswo_PrintPixelFormatDescriptor(PIXELFORMATDESCRIPTOR Pfd)
{
    printf("Pixel Format Descriptor\n");
    printf("    Size: %d\n", Pfd.nSize);
    printf("    Version: %d\n", Pfd.nVersion);
    printf("    Flags: %d\n", Pfd.dwFlags);
    printf("    Pixel Type: %d\n", Pfd.iPixelType);
    printf("    Color Bits: %d\n", Pfd.cColorBits);
    printf("    Red Bits: %d\n", Pfd.cRedBits);
    printf("    Red Shift: %d\n", Pfd.cRedShift);
    printf("    Green Bits: %d\n", Pfd.cGreenBits);
    printf("    Green Shift: %d\n", Pfd.cGreenShift);
    printf("    Blue Bits: %d\n", Pfd.cBlueBits);
    printf("    Blue Shift: %d\n", Pfd.cBlueShift);
    printf("    Alpha Bits: %d\n", Pfd.cAlphaBits);
    printf("    Alpha Shift: %d\n", Pfd.cAlphaShift);
    printf("    Accum Bits: %d\n", Pfd.cAccumBits);
    printf("    Accum Red Bits: %d\n", Pfd.cAccumRedBits);
    printf("    Accum Green Bits: %d\n", Pfd.cAccumGreenBits);
    printf("    Accum Blue Bits: %d\n", Pfd.cAccumBlueBits);
    printf("    Accum Alpha Bits: %d\n", Pfd.cAccumAlphaBits);
    printf("    Depth Bits: %d\n", Pfd.cDepthBits);
    printf("    Stencil Bits: %d\n", Pfd.cStencilBits);
    printf("    Aux Buffers: %d\n", Pfd.cAuxBuffers);
    printf("    Layer Type: %d\n", Pfd.iLayerType);
    printf("    Layer Mask: %d\n", Pfd.dwLayerMask);
    printf("    Visible Mask: %d\n", Pfd.dwVisibleMask);
    printf("    Damage Mask: %d\n", Pfd.dwDamageMask);
}

static void
gswo_SetPixelFormat(gswo_procs Procs, HDC WindowDC)
{
    s32 SuggestedPixelFormatIndex = 0;
    GLuint ExtendedPick = 0;
    if (Procs.wglChoosePixelFormatARB)
    {
        int IntAttribList[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            0,
        };
        
        bool Success = Procs.wglChoosePixelFormatARB(WindowDC, IntAttribList, 0, 1, &SuggestedPixelFormatIndex, &ExtendedPick);
        if (!Success)
        {
            gswo_PrintLastError("Error Choosing Pixel Format ARB");
        }
    }
    
    if (!ExtendedPick)
    {
        PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
        DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
        DesiredPixelFormat.nVersion = 1;
        DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
        DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
        DesiredPixelFormat.cColorBits = 32;
        DesiredPixelFormat.cAlphaBits = 8;
        DesiredPixelFormat.cDepthBits = 24;
        DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;
        
        SuggestedPixelFormatIndex = ChoosePixelFormat(WindowDC, &DesiredPixelFormat);
        if (!SuggestedPixelFormatIndex)
        {
            gswo_PrintLastError("Error Choosing Pixel Format");
        }
    }
    
    if (SuggestedPixelFormatIndex)
    {
        PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
        if (DescribePixelFormat(WindowDC, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat))
        {
            gswo_PrintPixelFormatDescriptor(SuggestedPixelFormat);
            if (SetPixelFormat(WindowDC, SuggestedPixelFormatIndex, &SuggestedPixelFormat))
            {
                // Success
            }
            else
            {
                gswo_PrintLastError("Error Setting Pixel Format");
            }
        }
        else
        {
            gswo_PrintLastError("Error Describing Pixel Format");
        }
        
    }
}

static void
gswo_LoadWGLProcsForPixelFormat(gswo_procs* Procs)
{
    // NOTE(Peter): We have to create a fake window because in order to get access to the extension functions
    // of OpenGL, we need a RenderContext. In order create a render context, you need a window. We could use
    // the window passed in, but that means that whatever pixel format we choose at this first step has to
    // be the pixel format we use in the end because Windows only lets you set the pixel format once per window.
    // This prevents us from using OpenGL extensions to describe the pixel format.
    //
    // The solution is to create a fake window to get the extension function pointers, then create the real
    // render context with extension methods, before destroying the fake window.
    
    WNDCLASSA WindowClass = {};
    WindowClass.lpfnWndProc = DefWindowProcA;
    WindowClass.hInstance = GetModuleHandle(0);
    WindowClass.lpszClassName = "gs_WGLLoader";
    
    if (RegisterClassA(&WindowClass))
    {
        HWND Window = CreateWindowExA(0,
                                      WindowClass.lpszClassName,
                                      "gs_WGLLoader",
                                      0,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      0,
                                      0,
                                      WindowClass.hInstance,
                                      0);
        
        HDC WindowDC = GetDC(Window);
        gswo_SetPixelFormat(*Procs, WindowDC);
        HGLRC OGLRC = wglCreateContext(WindowDC);
        if (wglMakeCurrent(WindowDC, OGLRC))
        {
            GetOpenGLProcAddress(Procs, wgl_choose_pixel_format_arb, wglChoosePixelFormatARB);
            GetOpenGLProcAddress(Procs, wgl_create_context_attribs_arb, wglCreateContextAttribsARB);
            GetOpenGLProcAddress(Procs, wgl_get_extensions_string_ext, wglGetExtensionsStringEXT);
        }
        else
        {
            // Error
        }
        
        wglDeleteContext(OGLRC);
        ReleaseDC(Window, WindowDC);
        DestroyWindow(Window);
    }
    else
    {
        // Error
    }
}

static void
gswo_GetProcAddresses(gswo_procs* Procs)
{
    GetOpenGLProcAddress(Procs, gl_gen_buffers, glGenBuffers);
    GetOpenGLProcAddress(Procs, gl_bind_buffer, glBindBuffer);
    GetOpenGLProcAddress(Procs, gl_buffer_data, glBufferData);
    GetOpenGLProcAddress(Procs, gl_create_shader, glCreateShader);
    GetOpenGLProcAddress(Procs, gl_shader_source, glShaderSource);
    GetOpenGLProcAddress(Procs, gl_compile_shader, glCompileShader);
    GetOpenGLProcAddress(Procs, gl_get_shader_iv, glGetShaderiv);
    GetOpenGLProcAddress(Procs, gl_get_shader_info_log, glGetShaderInfoLog);
    GetOpenGLProcAddress(Procs, gl_attach_shader, glAttachShader);
    GetOpenGLProcAddress(Procs, gl_delete_shader, glDeleteShader);
    GetOpenGLProcAddress(Procs, gl_create_program, glCreateProgram);
    GetOpenGLProcAddress(Procs, gl_delete_program, glDeleteProgram);
    GetOpenGLProcAddress(Procs, gl_link_program, glLinkProgram);
    GetOpenGLProcAddress(Procs, gl_get_program_iv, glGetProgramiv);
    GetOpenGLProcAddress(Procs, gl_get_program_info_log, glGetProgramInfoLog);
    GetOpenGLProcAddress(Procs, gl_use_program, glUseProgram);
    GetOpenGLProcAddress(Procs, gl_vertex_attrib_pointer, glVertexAttribPointer);
    GetOpenGLProcAddress(Procs, gl_vertex_attrib_ipointer, glVertexAttribIPointer);
    GetOpenGLProcAddress(Procs, gl_enable_vertex_attrib_array, glEnableVertexAttribArray);
    GetOpenGLProcAddress(Procs, gl_disable_vertex_attrib_array, glDisableVertexAttribArray);
    GetOpenGLProcAddress(Procs, gl_gen_vertex_arrays, glGenVertexArrays);
    GetOpenGLProcAddress(Procs, gl_bind_vertex_array, glBindVertexArray);
    GetOpenGLProcAddress(Procs, gl_active_texture, glActiveTexture);
    
    GetOpenGLProcAddress(Procs, gl_get_uniform_location, glGetUniformLocation);
    GetOpenGLProcAddress(Procs, gl_uniform_matrix_3fv, glUniformMatrix3fv);
    GetOpenGLProcAddress(Procs, gl_uniform_matrix_4fv, glUniformMatrix4fv);
    GetOpenGLProcAddress(Procs, gl_uniform_1fv, glUniform1fv);
    GetOpenGLProcAddress(Procs, gl_uniform_2fv, glUniform2fv);
    GetOpenGLProcAddress(Procs, gl_uniform_3fv, glUniform3fv);
    GetOpenGLProcAddress(Procs, gl_uniform_4fv, glUniform4fv);
    GetOpenGLProcAddress(Procs, gl_uniform_1iv, glUniform1iv);
    GetOpenGLProcAddress(Procs, gl_uniform_2iv, glUniform2iv);
    GetOpenGLProcAddress(Procs, gl_uniform_3iv, glUniform3iv);
    GetOpenGLProcAddress(Procs, gl_uniform_4iv, glUniform4iv);
    GetOpenGLProcAddress(Procs, gl_uniform_1i, glUniform1i);
    GetOpenGLProcAddress(Procs, gl_uniform_2i, glUniform2i);
    GetOpenGLProcAddress(Procs, gl_uniform_3i, glUniform3i);
    GetOpenGLProcAddress(Procs, gl_uniform_4i, glUniform4i);
    
    GetOpenGLProcAddress(Procs, gl_gen_framebuffers, glGenFramebuffers);
    GetOpenGLProcAddress(Procs, gl_bind_framebuffer, glBindFramebuffer);
    GetOpenGLProcAddress(Procs, gl_check_framebuffer_status, glCheckFramebufferStatus);
    GetOpenGLProcAddress(Procs, gl_bind_framebuffer, glBindFramebuffer);
    GetOpenGLProcAddress(Procs, gl_delete_framebuffers, glDeleteFramebuffers);
    GetOpenGLProcAddress(Procs, gl_framebuffer_texture_2d, glFramebufferTexture2D);
}

static gswo_render_context
gswo_Init(HDC DeviceContext, int ColorBits, int AlphaBits, int DepthBits, gswo_procs* Procs)
{
    gswo_render_context Result = {0};
    *Procs = {0};
    
    gswo_LoadWGLProcsForPixelFormat(Procs);
    
    s32 MajorVersionMin = 3;
    s32 MinorVersionMin = 2;
    int ContextAttributesList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, MajorVersionMin,
        WGL_CONTEXT_MINOR_VERSION_ARB, MinorVersionMin,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    
    gswo_SetPixelFormat(*Procs, DeviceContext);
    
    HGLRC RenderContext = Procs->wglCreateContextAttribsARB(DeviceContext, NULL, ContextAttributesList);
    if (RenderContext != NULL)
    {
        if (wglMakeCurrent(DeviceContext, RenderContext))
        {
            printf("Init Renderer - OpenGL\n");
            printf("Vendor: %s\n", glGetString(GL_VENDOR));
            printf("Renderer: %s\n", glGetString(GL_RENDERER));
            printf("Version: %s\n", glGetString(GL_VERSION));
            printf("Shading Language Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
            printf("Extensions: %s\n\n", Procs->wglGetExtensionsStringEXT());
            
            gswo_GetProcAddresses(Procs);
            
            Result.DeviceContext = DeviceContext;
            Result.RenderContext = RenderContext;
        }
        else
        {
            gswo_PrintLastError("Could Not Make RenderContext Current");
        }
    }
    else
    {
        gswo_PrintLastError("Could Not Create Render Context");
    }
    
    return Result;
}

static void
gswo_Render(gswo_render_context OpenGL)
{
    SwapBuffers(OpenGL.DeviceContext);
}

static void
gswo_CleanupRenderContext(gswo_render_context* OpenGL)
{
    int Error = 0;
    bool Result = wglDeleteContext(OpenGL->RenderContext);
    if (!Result)
    {
        Error = GetLastError();
    }
}

#define WIN32_OPENGL_H
#endif // WIN32_OPENGL_H
