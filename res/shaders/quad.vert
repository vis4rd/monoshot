#version 450 core
// quad.vert

layout (location = 0) in vec2 acPos;
layout (location = 1) in vec2 acUv;
layout (location = 2) in vec4 aiColor;
layout (location = 3) in float aiTexIndex;
layout (location = 4) in mat4 aiModel;

layout (location = 0) uniform mat4 uProjection;
layout (location = 1) uniform mat4 uView;

struct VertexOutput
{
    vec4 Color;
    vec2 Uv;
};

layout (location = 0) out VertexOutput outVertex;
layout (location = 2) out flat float outTexIndex;

void main()
{
    outVertex.Color = aiColor;
    outVertex.Uv = acUv;
    outTexIndex = aiTexIndex;

    gl_Position = uProjection * uView * aiModel * vec4(acPos, 0.0, 1.0);
}
