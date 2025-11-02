#version 450 core
// all_white.frag

struct VertexInput
{
    vec4 Color;
    vec2 Uv;
};

layout (location = 0) in VertexInput inVertex;

layout (location = 2) uniform sampler2D uTextures[1];

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(uTextures[0], inVertex.Uv);

    if(texColor.a == 0.0) { discard; }

    FragColor = vec4(1.0, 1.0, 1.0, texColor.a);
}
