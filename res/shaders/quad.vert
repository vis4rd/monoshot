#version 450 core
// quad.vert

struct QuadInstanceData
{
    float color[4];
    float model[16];
    float texIndex;
};

layout (location = 0) in vec2 acPos;
layout (location = 1) in vec2 acUv;
layout(std430, binding = 0) buffer QuadInstanceStorageBlock
{
    QuadInstanceData quads[];
};

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
    QuadInstanceData instance = quads[gl_InstanceID];
    vec4 instance_color = vec4(instance.color[0], instance.color[1], instance.color[2], instance.color[3]);
    mat4 instance_model = mat4(instance.model[0], instance.model[1], instance.model[2], instance.model[3],
                      instance.model[4], instance.model[5], instance.model[6], instance.model[7],
                      instance.model[8], instance.model[9], instance.model[10], instance.model[11],
                      instance.model[12], instance.model[13], instance.model[14], instance.model[15]);

    outVertex.Color = instance_color;
    outVertex.Uv = acUv;
    outTexIndex = instance.texIndex;

    gl_Position = uProjection * uView * instance_model * vec4(acPos, 0.0, 1.0);
}
