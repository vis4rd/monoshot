#version 450 core
// quad.vert

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in float aTexIndex;

layout (location = 0) uniform mat4 uProjection;
layout (location = 1) uniform mat4 uView;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 2) out flat float TexIndex;

void main()
{
    Output.Color = aColor;
    Output.TexCoord = aTexCoords;
    TexIndex = aTexIndex;

    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
