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
layout (location = 34) uniform uint uFrameCount[32];
layout (location = 66) uniform uint uFrameRowLength[32];
layout (location = 98) uniform uint uFrameCurrentIndex[32];

out vec4 FragColor;

void main()
{
    const uint frame_count = uFrameCount[int(inTexIndex)];
    const uint frame_row_length = uFrameRowLength[int(inTexIndex)];
    const uint frame_row_count = uint(frame_count / frame_row_length);
    const uint frame_index = uFrameCurrentIndex[int(inTexIndex)];
    const vec2 frame_coords = vec2(frame_index % frame_row_length, frame_index / frame_row_length);

    // TODO: change this math to vector operations: (inVertex.Uv + frame_coords) / vec2(frame_row_length, frame_row_count)
    vec4 texColor = inVertex.Color * texture(uTextures[int(inTexIndex)],
        vec2((inVertex.Uv.x + frame_coords.x ) / frame_row_length,
             (inVertex.Uv.y + frame_coords.y) / frame_row_count));

    if(texColor.a == 0.0) { discard; }

    FragColor = texColor;
}
