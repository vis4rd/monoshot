#version 450 core
// line.frag

layout (location = 0) in vec4 iColor;
out vec4 oColor;

void main()
{
    oColor = iColor;
}
