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
layout (location = 34) uniform uint uFrameCount[32];
layout (location = 66) uniform uint uFrameRowLength[32];
layout (location = 98) uniform uint uFrameCurrentIndex[32];

out vec4 FragColor;

void main()
{
    uint frame_count = uFrameCount[int(TexIndex)];
    uint frame_row_length = uFrameRowLength[int(TexIndex)];
    uint frame_row_count = uint(frame_count / frame_row_length);
    uint frame_index = uFrameCurrentIndex[int(TexIndex)];
    vec2 frame_coords = vec2(frame_index % frame_row_length, frame_index / frame_row_length);

    vec4 texColor = Input.Color * texture(uTextures[int(TexIndex)],
        vec2((Input.TexCoord.x / frame_row_length) + frame_coords.x * (1.0 / frame_row_length),
             (Input.TexCoord.y / frame_row_count)  + frame_coords.y * (1.0 / frame_row_count)));

    if(texColor.a == 0.0) { discard; }

    FragColor = texColor;
}
