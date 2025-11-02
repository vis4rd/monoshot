#version 450 core
// post_process.vert

layout (location = 0) in vec2 acPos;
layout (location = 1) in vec2 acUv;

struct VertexOutput
{
    vec4 Color;
    vec2 Uv;
};

layout (location = 0) out VertexOutput outVertex;

void main()
{
    outVertex.Color = vec4(1.0, 1.0, 1.0, 1.0);
    outVertex.Uv = acUv;

    gl_Position = vec4(acPos, 0.0, 1.0);
}
