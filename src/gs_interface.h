//
// File: gs_interface.h
// Author: Peter Slattery
// Creation Date: 2020-04-25
//
#ifndef GS_INTERFACE_H

// TODO(Peter): Maybe this isn't gs_interface.h -> should it be gs_draw.h?

#if !defined(GS_TYPES_H)
# error "gs_interface.h relies on gs_types.h, but it is not included"
#endif

#if !defined(GS_RENDERER_H) || !defined(GS_RENDER_QUEUE_H)
# error "gs_interface.h relies on both gs_renderer.h and gs_render_queue.h but they are not included."
#endif

// TODO(Peter): Is it a problem that I'm storing GLSL shaders as the defaults for things?
// is there a way to translate shaders between backends? Is GLSL even the same thing on
// different OSes?
global char* gsi_InterfaceVertexShader = R"FOO(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in int aTexIndex;

out vec2 TexCoord;
out vec3 FragPos;
out vec4 Color;
flat out int TextureIndex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main ()
{
gl_Position = Projection * View * Model * vec4(aPos, 1.0);
TexCoord = aTexCoord;
FragPos = vec3(Model * vec4(aPos, 1.0));
Color = aColor;
TextureIndex = aTexIndex;
}
)FOO";

global char* gsi_InterfaceFragmentShader = R"FOO(
#version 330 core

in vec2 TexCoord;
in vec4 Color;
flat in int TextureIndex;

out vec4 FragColor;
uniform sampler2D Textures[4];

void main()
{
FragColor = texture(Textures[1], TexCoord) * Color;

if (TextureIndex == 0)
{
FragColor = vec4(1, 0, 0, 1);
}
else if (TextureIndex == 1)
{
FragColor = vec4(0, 1, 0, 1);
}
else
{
FragColor = vec4(0, 0, 1, 1);
}

}
)FOO";

global gsr_vertex_attribute gsi_InterfaceVertexAttributes[] = {
    { "aPos", gs_BasicType_r32, 3, 0 },
    { "aTexCoords", gs_BasicType_r32, 2, 0 },
    { "aColor", gs_BasicType_r32, 4, 0},
    { "aTexIndex", gs_BasicType_s32, 1, 0},
    { 0 }
};

struct gsi_interface
{
    gsr_render_asset_handle VertexBufferHandle;
    gsr_render_asset_handle InterfaceShader;
};

internal gsi_interface
gsi_CreateInterface(gsr_render_queue* Queue)
{
    gsi_interface Interface = {0};
    Interface.VertexBufferHandle = gsr_TakeVertexBuffer(Queue, gsi_InterfaceVertexAttributes);
    Interface.InterfaceShader = gsr_CreateShader(Queue, gsi_InterfaceVertexShader, gsi_InterfaceFragmentShader);
    return Interface;
}
internal void
gsi_ClearInterface(gsi_interface Interface, gsr_render_queue* Queue)
{
    gsr_ResetVertexBuffer(Queue, Interface.VertexBufferHandle);
}
internal void
gsi_DrawInterface(gsi_interface Interface, gsr_render_queue* Queue, r32 ScreenWidth, r32 ScreenHeight, gsr_render_asset_handle DefaultTexture,
                  gsr_render_asset_handle BTexture)
{
    m44 OrthoProj = M44ProjectionInterfaceOrtho(ScreenWidth, ScreenHeight, 0, 100);
    m44 Identity = M44Identity();
    gsr_SetShader(Queue, Interface.InterfaceShader);
    gsr_SetMatrixUniform(Queue, 16, 1, Identity.E, "View");
    gsr_SetMatrixUniform(Queue, 16, 1, OrthoProj.E, "Projection");
    gsr_SetMatrixUniform(Queue, 16, 1, Identity.E, "Model");
    
    gsr_SetTexture(Queue, DefaultTexture, 0, "Textures[0]");
    gsr_SetTexture(Queue, BTexture, 1, "Textures[1]");
    
    gsr_RenderVertexBuffer(Queue, Interface.VertexBufferHandle);
}

internal void
gsi_SetVertexData(gsr_vertex_buffer_buffer* Buffer, u32 Vertex, v3 Position, v2 UV, v4 Color, u32 TextureIndex)
{
    gsr_VertexBufferSetVertex(Buffer, Vertex, 0, 3, Position.E);
    gsr_VertexBufferSetVertex(Buffer, Vertex, 3, 2, UV.E);
    gsr_VertexBufferSetVertex(Buffer, Vertex, 5, 4, Color.E);
    
    gsr_VertexBufferSetVertex(Buffer, Vertex, 9, 1, &TextureIndex);
}

internal void
gsi_DrawCircle2D(gsi_interface Interface, gsr_render_queue* Queue, v2 Center, r32 Radius, v4 Color)
{
    u32 PerimeterPoints = 32;
    u32 VertexCount = PerimeterPoints + 1;
    u32 TriCount = PerimeterPoints - 2;
    u32 IndexCount = TriCount * 3;
    gsr_vertex_buffer_buffer* NewBuffer = gsr_GrowVertexBuffer(Queue->Assets, &Queue->Arena, Interface.VertexBufferHandle, VertexCount, IndexCount);
    
    r32 Depth = -1;
    r32 TD = (1.0f / (r32)PerimeterPoints) * TauR32;
    v3 CenterV3 = ToV3(Center, Depth); // Center
    for (u32 i = 0; i < PerimeterPoints; i++)
    {
        r32 Theta0 = TD * i;
        v3 LocalPoint = v3{ CosR32(Theta0), SinR32(Theta0), 0 };
        v3 Point = CenterV3 + (LocalPoint * Radius);
        v2 UV = (LocalPoint.XY * .5f) + v2{.5f, .5f};
        gsi_SetVertexData(NewBuffer, i, Point, UV, Color, );
    }
    
    for (u32 t = 0; t < TriCount; t++)
    {
        gsr_VertexBufferSetTriIndices(NewBuffer, t, 0, t + 1, t + 2);
    }
}

internal void
gsi_DrawQuad2D(gsi_interface Interface, gsr_render_queue* Queue, v2 Min, v2 Max, v4 Color)
{
    u32 VertexCount = 4;
    u32 IndexCount = 6;
    gsr_vertex_buffer_buffer* NewBuffer = gsr_GrowVertexBuffer(Queue->Assets, &Queue->Arena, Interface.VertexBufferHandle, VertexCount, IndexCount);
    
    r32 Depth = -1;
    
    gsi_SetVertexData(NewBuffer, 0, ToV3(Min, Depth), v2{0,0}, Color, 0); // Bottom Left
    gsi_SetVertexData(NewBuffer, 1, ToV3(v2{Max.X, Min.Y}, Depth), v2{1,0}, Color, 0); // Bottom Right
    gsi_SetVertexData(NewBuffer, 2, ToV3(Max, Depth), v2{1,1}, Color, 0); // Top Right
    gsi_SetVertexData(NewBuffer, 3, ToV3(v2{Min.X, Max.Y}, Depth), v2{0,1}, Color, 0); // Top Left
    
    gsr_VertexBufferSetTriIndices(NewBuffer, 0, 0, 1, 2);
    gsr_VertexBufferSetTriIndices(NewBuffer, 1, 0, 2, 3);
}

inline void
gsr_PushLineSegment(gsr_vertex_buffer_buffer* Buffer, u32 T0Index, u32 V0Index, v2 P0, v2 P1, r32 Thickness, v4 Color)
{
    r32 Depth = -1;
    v2 Normal = V2Normalize(V2PerpendicularCW((P1 - P0))) * (Thickness / 2.0f);
    v3 V0 = ToV3(P0 - Normal, Depth);
    v3 V1 = ToV3(P1 - Normal, Depth);
    v3 V2 = ToV3(P1 + Normal, Depth);
    v3 V3 = ToV3(P0 + Normal, Depth);
    
    // TODO(Peter): This is inefficient, we can use the last two verteces of the last segment
    // as the first two of this one. This requires a bit more math than I have in my brain rn
    // We could also be MORE inefficient, and put a small circle at every intersection of lines... XD
    u32 V1Index = V0Index + 1;
    u32 V2Index = V0Index + 2;
    u32 V3Index = V0Index + 3;
    
    gsi_SetVertexData(Buffer, V0Index, V0, v2{0,0}, Color, 0);
    gsi_SetVertexData(Buffer, V1Index, V1, v2{1,0}, Color, 0);
    gsi_SetVertexData(Buffer, V2Index, V2, v2{1,1}, Color, 0);
    gsi_SetVertexData(Buffer, V3Index, V3, v2{0,1}, Color, 0);
    
    gsr_VertexBufferSetTriIndices(Buffer, T0Index, V0Index, V1Index, V2Index);
    gsr_VertexBufferSetTriIndices(Buffer, T0Index + 1, V0Index, V2Index, V3Index);
}
internal void
gsi_DrawLine2D(gsi_interface Interface, gsr_render_queue* Queue, v2* Points, u32 SegmentsCount, r32 Thickness, v4 Color)
{
    if (SegmentsCount > 0)
    {
        u32 VertexCount = 4 * SegmentsCount;
        u32 IndexCount = 6 * SegmentsCount;
        gsr_vertex_buffer_buffer* NewBuffer = gsr_GrowVertexBuffer(Queue->Assets, &Queue->Arena, Interface.VertexBufferHandle, VertexCount, IndexCount);
        
        for (u32 i = 0; i < SegmentsCount; i++)
        {
            u32 T0Index = i * 2;
            u32 V0Index = i * 4;
            gsr_PushLineSegment(NewBuffer, T0Index, V0Index, Points[i], Points[i + 1], Thickness, Color);
        }
    }
}
internal void
gsi_DrawLine2D(gsi_interface Interface, gsr_render_queue* Queue, r32 Thickness, v4 Color, u32 SegmentsCount, ...)
{
    if (SegmentsCount > 0)
    {
        u32 VertexCount = 4 * SegmentsCount;
        u32 IndexCount = 6 * SegmentsCount;
        gsr_vertex_buffer_buffer* NewBuffer = gsr_GrowVertexBuffer(Queue->Assets, &Queue->Arena, Interface.VertexBufferHandle, VertexCount, IndexCount);
        
        va_list Args;
        va_start(Args, SegmentsCount);
        v2 P0 = va_arg(Args, v2);
        for (u32 i = 0; i < SegmentsCount; i++)
        {
            u32 T0Index = i * 2;
            u32 V0Index = i * 4;
            v2 P1 = va_arg(Args, v2);
            gsr_PushLineSegment(NewBuffer, T0Index, V0Index, P0, P1, Thickness, Color);
            P0 = P1;
        }
    }
}
internal void
gsi_DrawLineSegment2D(gsi_interface Interface, gsr_render_queue* Queue, v2 P0, v2 P1, r32 Thickness, v4 Color)
{
    gsi_DrawLine2D(Interface, Queue, Thickness, Color, 1, P0, P1);
}
internal void
gsi_DrawCircleOutline2D(gsi_interface Interface, gsr_render_queue* Queue, v2 Center, r32 Radius, r32 Thickness, v4 Color)
{
    // TODO(Peter): Some way to make the vertex count adjust to the on screen size of the
    // circle
    v2 Points[33];
    u32 PointsCount = 32;
    r32 DeltaTheta = TauR32 * (1.0f / (r32)PointsCount);
    r32 InnerRadius = Radius - Thickness;
    for (u32 i = 0; i < PointsCount; i++)
    {
        r32 Theta = DeltaTheta * i;
        Points[i] = Center + (v2{ CosR32(Theta), SinR32(Theta) } * InnerRadius);
    }
    // wrap back to the beginning to close the circle
    Points[PointsCount] = Points[0];
    gsi_DrawLine2D(Interface, Queue, Points, PointsCount, Thickness, Color);
}
internal void
gsi_DrawQuadOutline2D(gsi_interface Interface, gsr_render_queue* Queue, v2 Min, v2 Max, r32 Thickness, v4 Color)
{
    v2 V0 = Min;
    v2 V1 = v2{Max.X, Min.Y};
    v2 V2 = Max;
    v2 V3 = v2{Min.X, Max.Y};
    gsi_DrawLine2D(Interface, Queue, Thickness, Color, 4, V0, V1, V2, V3, V0);
}

#define GS_INTERFACE_H
#endif // GS_INTERFACE_H