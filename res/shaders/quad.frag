#version 450 core
// quad.frag

struct VertexInput
{
    vec4 Color;
    vec2 Uv;
};

layout (location = 0) in VertexInput inVertex;

out vec4 FragColor;

void main()
{
    vec4 texColor = inVertex.Color;

    if(texColor.a == 0.0) { discard; }

    FragColor = texColor;
}
