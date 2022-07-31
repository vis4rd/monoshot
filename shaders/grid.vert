#version 450 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

// layout (location = 0) uniform mat4 uTransform;
layout (location = 1) uniform mat4 uProjection;
layout (location = 2) uniform mat4 uView;

void main()
{
   gl_Position = uProjection * uView * vec4(aPos, 1.0);
   vertexColor = vec4(1.f, 1.f, 1.f, 1.f);
}
