#version 450 core
// quad.frag

struct VertexInput
{
    vec4 Color;
    vec2 Uv;
};

layout (location = 0) in VertexInput inVertex;
layout (location = 2) in flat float inTexIndex;

layout (location = 2) uniform sampler2D uTextures[32];

out vec4 FragColor;

void main()
{
    const int tex_index = int(inTexIndex);

    vec4 texColor = inVertex.Color * texture(uTextures[tex_index], inVertex.Uv);

    if(texColor.a == 0.0) { discard; }

    FragColor = texColor;
}
