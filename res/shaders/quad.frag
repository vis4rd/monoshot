#version 450 core
// quad.frag

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 2) in flat float TexIndex;

layout (location = 2) uniform sampler2D uTextures[32];

out vec4 FragColor;

void main()
{
    vec4 texColor = Input.Color * texture(uTextures[int(TexIndex)], Input.TexCoord);

    if(texColor.a == 0.0) { discard; }

    FragColor = texColor;
}
