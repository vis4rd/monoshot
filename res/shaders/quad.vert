#version 450 core
// quad.vert

struct QuadInstanceData
{
    uint color;
    float position[2];
    float scale[2];
    uint rotation_texIndex;
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

vec4 createColor(uint color)
{
    const float red = float((color >> 24) & 0xFF) / 255.0;
    const float green = float((color >> 16) & 0xFF) / 255.0;
    const float blue = float((color >> 8) & 0xFF) / 255.0;
    const float alpha = float((color >> 0) & 0xFF) / 255.0;

    return vec4(red, green, blue, alpha);
}

mat4 createModelMatrix(vec2 position, vec2 scale, float rotation)
{
    mat4 translation_matrix = mat4(1.0);
    translation_matrix[3][0] = position.x;
    translation_matrix[3][1] = position.y;

    const float cos_theta = cos(rotation);
    const float sin_theta = sin(rotation);
    const mat4 rotationMatrix = mat4(
        cos_theta, -sin_theta, 0.0, 0.0,
        sin_theta, cos_theta, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 scaleMatrix = mat4(1.0);
    scaleMatrix[0][0] = scale.x;
    scaleMatrix[1][1] = scale.y;

    return translation_matrix * rotationMatrix * scaleMatrix;
}

void main()
{
    QuadInstanceData instance = quads[gl_InstanceID];

    const vec2 position = vec2(instance.position[0], instance.position[1]);
    const vec2 scale = vec2(instance.scale[0], instance.scale[1]);
    const float rotation = radians(instance.rotation_texIndex & 0x1FF);
    const float tex_index = (instance.rotation_texIndex >> 9) & 0x1F;

    const vec4 instance_color = createColor(instance.color);
    const mat4 instance_model = createModelMatrix(position, scale, rotation);

    outVertex.Color = instance_color;
    outVertex.Uv = acUv;
    outTexIndex = tex_index;

    gl_Position = uProjection * uView * instance_model * vec4(acPos, 0.0, 1.0);
}
