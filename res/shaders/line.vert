#version 450 core
// line.vert

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

layout (location = 0) uniform mat4 uProjection;
layout (location = 1) uniform mat4 uView;

layout (location = 0) out vec4 oColor;

void main()
{
    oColor = aColor;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
