#pragma once

template<std::size_t WIDTH, std::size_t HEIGHT>
void Grid<WIDTH, HEIGHT>::prepareForRender()
{
    GLuint position_attribute_index = 0;
    GLuint vb_binding_index = 0;
    std::int64_t offset = 0;  // offset of the first element in the buffer
    std::int32_t stride = 3 * sizeof(float);  // distance between values of the same character

    glCreateVertexArrays(1, &m_vao);

    glCreateBuffers(1, &m_ebo);
    glNamedBufferData(m_ebo, m_elementBuffer.size() * sizeof(float), reinterpret_cast<const void*>(m_elementBuffer.data()), GL_STATIC_DRAW);
    glVertexArrayElementBuffer(m_vao, m_ebo);

    glCreateBuffers(1, &m_vbo);
    glNamedBufferData(m_vbo, m_vertexBuffer.size() * sizeof(float), reinterpret_cast<const void*>(m_vertexBuffer.data()), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(m_vao, vb_binding_index, m_vbo, offset, stride);

    glVertexArrayAttribFormat(m_vao, position_attribute_index, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_vao, position_attribute_index, vb_binding_index);
    glEnableVertexArrayAttrib(m_vao, position_attribute_index);

    ShaderManager::addShaderProgram("../res/shaders", "grid");
}

template<std::size_t WIDTH, std::size_t HEIGHT>
Grid<WIDTH, HEIGHT>::Grid(const std::array<tile_t, WIDTH * HEIGHT>& tiles)
{
    for(auto i = 0; i < WIDTH * HEIGHT; i++)
    {
        m_tiles[i] = tiles[i];
    }
}

template<std::size_t WIDTH, std::size_t HEIGHT>
Grid<WIDTH, HEIGHT>::Grid(const std::initializer_list<tile_t>& tiles)
{
    if(tiles.size() >= WIDTH * HEIGHT)
    {
        for(auto i = 0; i < WIDTH; i++)
        {
            for(auto j = 0; j < HEIGHT; j++)
            {
                m_tiles[i][j] = std::data(tiles)[(i * 4) + j];
            }
        }
    }
}

template<std::size_t WIDTH, std::size_t HEIGHT>
auto Grid<WIDTH, HEIGHT>::data() const -> const tile_t*
{
    return reinterpret_cast<const tile_t*>(m_tiles);
}

template<std::size_t WIDTH, std::size_t HEIGHT>
auto Grid<WIDTH, HEIGHT>::operator[](const std::size_t& index) -> tile_t&
{
    return *(m_tiles[index]);
}

template<std::size_t WIDTH, std::size_t HEIGHT>
auto Grid<WIDTH, HEIGHT>::operator[](const std::size_t& index) const -> const tile_t&
{
    return *(m_tiles[index]);
}

template<std::size_t WIDTH, std::size_t HEIGHT>
auto Grid<WIDTH, HEIGHT>::at(const std::size_t& index) -> tile_t&
{
    return *(m_tiles[index % (WIDTH * HEIGHT)]);
}

template<std::size_t WIDTH, std::size_t HEIGHT>
auto Grid<WIDTH, HEIGHT>::at(const std::size_t& index) const -> const tile_t&
{
    return *(m_tiles[index % (WIDTH * HEIGHT)]);
}

template<std::size_t WIDTH, std::size_t HEIGHT>
void Grid<WIDTH, HEIGHT>::render() noexcept
{
    spdlog::trace("Vertex count: {}, element count: {}", m_vertexBuffer.size() / 3, m_elementBuffer.size());
    ShaderManager::useShader("grid");
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_elementBuffer.size(), GL_UNSIGNED_INT, 0);
}

template<std::size_t WIDTH, std::size_t HEIGHT>
void Grid<WIDTH, HEIGHT>::update() noexcept
{
    enum corner : std::size_t
    {
        BL = 0,  // bottom left
        BR = 1,  // bottom right
        UR = 2,  // upper right
        UL = 3  // upper left
    };

    // typedef for convenience
    auto& grid = m_tiles;

    // pre-calculate center of the grid
    std::size_t center_x = WIDTH / 2;
    std::size_t center_y = HEIGHT / 2;

    // indices of all tiles
    std::vector<std::uint32_t> all_index_buffer(WIDTH * HEIGHT * 4);

    // calculate number of filled tiles
    std::size_t temp_count = 0;
    for(std::size_t i = 0; i < WIDTH; i++)
    {
        for(std::size_t j = 0; j < HEIGHT; j++)
        {
            temp_count += (grid[i][j] == 1);
        }
    }
    const std::size_t filled_count = temp_count;
    const std::size_t empty_count = WIDTH * HEIGHT - filled_count;

    // calculate maximum number of indices and vertices
    const std::size_t max_index_count = filled_count * 6;
    const std::size_t max_vertex_count = filled_count * 4 * 3;

    // std::vector<std::uint32_t> index_buffer(max_index_count);
    std::size_t ib_pivot = 0;  // next free position to use in index_buffer

    std::size_t next_index = 0;  // next index value to be assigned to vertex in index_buffer

    // create index buffer
    m_elementBuffer.resize(max_index_count);

    // prepare vertex buffer
    m_vertexBuffer.resize(max_vertex_count);


    // generate element buffer (index buffer)
    for(std::size_t i = 0; i < WIDTH; i++)
    {
        for(std::size_t j = 0; j < HEIGHT; j++)
        {
            // pre-calculating tile indices in all_index_buffer
            auto current_tile = (i * WIDTH * 4) + (j * 4);
            auto left_tile = (i * WIDTH * 4) + ((j - 1) * 4);
            auto upper_left_tile = ((i - 1) * WIDTH * 4) + ((j - 1) * 4);
            auto upper_right_tile = ((i - 1) * WIDTH * 4) + ((j + 1) * 4);
            auto upper_tile = ((i - 1) * WIDTH * 4) + (j * 4);

            // calculate distance of current tile from the center of the grid
            auto x_diff = static_cast<float>(j) - center_x;
            auto y_diff = static_cast<float>(i) - center_y;
            spdlog::trace("tile {}: grid_center = ({}x, {}y), diff = ({}x, {}y)", current_tile / 4, center_x, center_y, x_diff, y_diff);

            // calculate exact positions of vertices in local space
            auto bl_x = -0.5f + x_diff;
            auto bl_y = -0.5f - y_diff;
            auto br_x = 0.5f + x_diff;
            auto br_y = -0.5f - y_diff;
            auto ur_x = 0.5f + x_diff;
            auto ur_y = 0.5f - y_diff;
            auto ul_x = -0.5f + x_diff;
            auto ul_y = 0.5f - y_diff;

            if(grid[i][j] == 1)  // if the tile is filled
            {
                // bottom left
                if(j == 0)  // if it is a left-most tile
                {
                    all_index_buffer[current_tile + corner::BL] = next_index++;
                    m_vertexBuffer[((next_index - 1) * 3)] = bl_x;
                    m_vertexBuffer[((next_index - 1) * 3) + 1] = bl_y;
                    m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                }
                else if(grid[i][j - 1] == 1)  // if left tile is filled
                {
                    all_index_buffer[current_tile + corner::BL] = all_index_buffer[left_tile + corner::BR];
                }
                else
                {
                    all_index_buffer[current_tile + corner::BL] = next_index++;
                    m_vertexBuffer[((next_index - 1) * 3)] = bl_x;
                    m_vertexBuffer[((next_index - 1) * 3) + 1] = bl_y;
                    m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                }
                spdlog::trace("Vertex {} = ({}x, {}y, {}z)", (next_index - 1), m_vertexBuffer[((next_index - 1) * 3)], m_vertexBuffer[((next_index - 1) * 3) + 1], m_vertexBuffer[((next_index - 1) * 3) + 2]);


                // bottom right
                all_index_buffer[current_tile + corner::BR] = next_index++;
                m_vertexBuffer[((next_index - 1) * 3)] = br_x;
                m_vertexBuffer[((next_index - 1) * 3) + 1] = br_y;
                m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                spdlog::trace("Vertex {} = ({}x, {}y, {}z)", (next_index - 1), m_vertexBuffer[((next_index - 1) * 3)], m_vertexBuffer[((next_index - 1) * 3) + 1], m_vertexBuffer[((next_index - 1) * 3) + 2]);

                // upper right
                if(i == 0)  // if it is upper-most tile
                {
                    all_index_buffer[current_tile + corner::UR] = next_index++;
                    m_vertexBuffer[((next_index - 1) * 3)] = ur_x;
                    m_vertexBuffer[((next_index - 1) * 3) + 1] = ur_y;
                    m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                }
                else if(grid[i - 1][j] == 1)  // if upper tile is filled
                {
                    all_index_buffer[current_tile + corner::UR] = all_index_buffer[upper_tile + corner::BR];
                }
                else if(j == (WIDTH - 1))  // if it is right-most tile
                {
                    all_index_buffer[current_tile + corner::UR] = next_index++;
                    m_vertexBuffer[((next_index - 1) * 3)] = ur_x;
                    m_vertexBuffer[((next_index - 1) * 3) + 1] = ur_y;
                    m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                }
                else if(grid[i - 1][j + 1] == 1)  // if upper-right tile is filled
                {
                    all_index_buffer[current_tile + corner::UR] = all_index_buffer[upper_right_tile + corner::BL];
                }
                else
                {
                    all_index_buffer[current_tile + corner::UR] = next_index++;
                    m_vertexBuffer[((next_index - 1) * 3)] = ur_x;
                    m_vertexBuffer[((next_index - 1) * 3) + 1] = ur_y;
                    m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                }
                spdlog::trace("Vertex {} = ({}x, {}y, {}z)", (next_index - 1), m_vertexBuffer[((next_index - 1) * 3)], m_vertexBuffer[((next_index - 1) * 3) + 1], m_vertexBuffer[((next_index - 1) * 3) + 2]);


                // upper left
                if(i == 0)  // if it is upper-most tile
                {
                    if(j == 0)  // if it is left-most tile
                    {
                        all_index_buffer[current_tile + corner::UL] = next_index++;
                        m_vertexBuffer[((next_index - 1) * 3)] = ul_x;
                        m_vertexBuffer[((next_index - 1) * 3) + 1] = ul_y;
                        m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                    }
                    else if(grid[i][j - 1] == 1)  // if left tile is filled
                    {
                        all_index_buffer[current_tile + corner::UL] = all_index_buffer[left_tile + corner::UR];
                    }
                    else
                    {
                        all_index_buffer[current_tile + corner::UL] = next_index++;
                        m_vertexBuffer[((next_index - 1) * 3)] = ul_x;
                        m_vertexBuffer[((next_index - 1) * 3) + 1] = ul_y;
                        m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                    }
                }
                else if(grid[i - 1][j] == 1)  // if upper tile is filled
                {
                    all_index_buffer[current_tile + corner::UL] = all_index_buffer[upper_tile + corner::BL];
                }
                else if(j == 0)  // if it is left-most tile
                {
                    all_index_buffer[current_tile + corner::UL] = next_index++;
                    m_vertexBuffer[((next_index - 1) * 3)] = ul_x;
                    m_vertexBuffer[((next_index - 1) * 3) + 1] = ul_y;
                    m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                }
                else if(grid[i - 1][j - 1] == 1)  // if upper-left tile is filled
                {
                    all_index_buffer[current_tile + corner::UL] = all_index_buffer[upper_left_tile + corner::BR];
                }
                else if(grid[i][j - 1] == 1)  // if left tile is filled
                {
                    all_index_buffer[current_tile + corner::UL] = all_index_buffer[left_tile + corner::UR];
                }
                else
                {
                    all_index_buffer[current_tile + corner::UL] = next_index++;
                    m_vertexBuffer[((next_index - 1) * 3)] = ul_x;
                    m_vertexBuffer[((next_index - 1) * 3) + 1] = ul_y;
                    m_vertexBuffer[((next_index - 1) * 3) + 2] = 0.f;
                }
                spdlog::trace("Vertex {} = ({}x, {}y, {}z)", (next_index - 1), m_vertexBuffer[((next_index - 1) * 3)], m_vertexBuffer[((next_index - 1) * 3) + 1], m_vertexBuffer[((next_index - 1) * 3) + 2]);

                m_elementBuffer[ib_pivot++] = all_index_buffer[current_tile + corner::BL];
                m_elementBuffer[ib_pivot++] = all_index_buffer[current_tile + corner::BR];
                m_elementBuffer[ib_pivot++] = all_index_buffer[current_tile + corner::UR];
                m_elementBuffer[ib_pivot++] = all_index_buffer[current_tile + corner::UR];
                m_elementBuffer[ib_pivot++] = all_index_buffer[current_tile + corner::UL];
                m_elementBuffer[ib_pivot++] = all_index_buffer[current_tile + corner::BL];
            }
        }
    }
    m_vertexBuffer.resize(next_index * 3);
    m_vertexBuffer.shrink_to_fit();
    spdlog::debug("Element count: {}", m_elementBuffer.size());
    spdlog::debug("Vertex count: {}", m_vertexBuffer.size() / 3);
    spdlog::debug("Unique vertex positions: {}", next_index);
    spdlog::debug("Vertices of filled tiles: {}", max_vertex_count / 3);
    spdlog::debug("All vertices in the grid: {}", WIDTH * HEIGHT * 6);

    std::string output = "Element buffer of the grid:\n";
    for(std::size_t i = 0; i < m_elementBuffer.size(); i++)
    {
        output += (std::to_string(m_elementBuffer[i]) + ", ");
    }
    spdlog::debug(output);
}

// Grid<4, 4> = {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1};
// EB: 0, 1, 2, 3, 1, 4, 5, 2, 4, 6, 7, 5, 8, 9, 4, 1, 9, 10, 6, 4, 10, 11, 12, 6, 13, 14, 10, 9, 15, 16, 17, 18, 16, 19, 13, 17, 19, 20, 14, 13, 20, 21, 22, 14,

// for(std::size_t i = 0; i < WIDTH; i++)
// {
//     if(grid[0][i] == 1)
//     {
//         if(i == 0)  // special case: first ever tile
//         {
//             all_index_buffer[(i * 4) + corner::BL] = next_index++;
//             all_index_buffer[(i * 4) + corner::BR] = next_index++;
//             all_index_buffer[(i * 4) + corner::UR] = next_index++;
//             all_index_buffer[(i * 4) + corner::UL] = next_index++;
//         }
//         else if(grid[0][i - 1] == 1)  // if left tile is filled
//         {
//             all_index_buffer[(i * 4) + corner::BL] = all_index_buffer[((i - 1) * 4) + corner::BR];
//             all_index_buffer[(i * 4) + corner::BR] = next_index++;
//             all_index_buffer[(i * 4) + corner::UR] = next_index++;
//             all_index_buffer[(i * 4) + corner::UL] = all_index_buffer[((i - 1) * 4) + corner::UR];
//         }
//         else  // upper tile is empty
//         {
//             all_index_buffer[(i * 4) + corner::BL] = next_index++;
//             all_index_buffer[(i * 4) + corner::BR] = next_index++;
//             all_index_buffer[(i * 4) + corner::UR] = next_index++;
//             all_index_buffer[(i * 4) + corner::UL] = next_index++;
//         }
//         index_buffer[ib_pivot++] = all_index_buffer[(i * 4) + corner::BL];
//         index_buffer[ib_pivot++] = all_index_buffer[(i * 4) + corner::BR];
//         index_buffer[ib_pivot++] = all_index_buffer[(i * 4) + corner::UR];
//         index_buffer[ib_pivot++] = all_index_buffer[(i * 4) + corner::UL];
//     }
// }

// for(std::size_t i = 1; i < WIDTH; i++)
// {
//     for(std::size_t j = 0; j < HEIGHT; j++)
//     {
//         // pre-calculating tile indices in all_index_buffer
//         auto current_tile = (i * WIDTH * 4) + (j * 4);
//         auto left_tile = (i * WIDTH * 4) + ((j - 1) * 4);
//         auto upper_left_tile = ((i - 1) * WIDTH * 4) + ((j - 1) * 4);
//         auto upper_right_tile = ((i - 1) * WIDTH * 4) + ((j + 1) * 4);
//         auto upper_tile = ((i - 1) * WIDTH * 4) + (j * 4);

// // if the tile is filled
// if(grid[i][j] == 1)
// {
//     // bottom left
//     if(j == 0)  // is this is a left-most tile
//     {
//         all_index_buffer[current_tile + corner::BL] = next_index++;
//     }
//     else if(grid[i][j - 1] == 1)  // is left tile filled
//     {
//         all_index_buffer[current_tile + corner::BL] = all_index_buffer[left_tile + corner::BR];
//     }
//     else
//     {
//         all_index_buffer[current_tile + corner::BL] = next_index++;
//     }

// // bottom right
// all_index_buffer[current_tile + corner::BR] = next_index++;

// // upper right
// if(grid[i - 1][j] == 1)  // if upper tile is filled
// {
//     all_index_buffer[current_tile + corner::UR] = all_index_buffer[upper_tile + corner::BR];
// }
// else if(j == (WIDTH - 1))  // if it is right-most tile
// {
//     all_index_buffer[current_tile + corner::UR] = next_index++;
// }
// else if(grid[i - 1][j + 1] == 1)  // if upper-right tile is filled
// {
//     all_index_buffer[current_tile + corner::UR] = all_index_buffer[upper_right_tile + corner::BL];
// }
// else
// {
//     all_index_buffer[current_tile + corner::UR] = next_index++;
// }

// // upper left
// if(grid[i - 1][j] == 1)  // if upper tile is filled
// {
//     all_index_buffer[current_tile + corner::UL] = all_index_buffer[upper_tile + corner::BL];
// }
// else if(j == 0)  // if it is left-most tile
// {
//     all_index_buffer[current_tile + corner::UL] = next_index++;
// }
// else if(grid[i - 1][j - 1] == 1)  // if upper-left tile is filled
// {
//     all_index_buffer[current_tile + corner::UL] = all_index_buffer[upper_left_tile + corner::BR];
// }
// else if(grid[i][j - 1] == 1)  // if left tile is filled
// {
//     all_index_buffer[current_tile + corner::UL] = all_index_buffer[left_tile + corner::UR];
// }
// else
// {
//     all_index_buffer[current_tile + corner::UL] = next_index++;
// }

// index_buffer[ib_pivot++] = all_index_buffer[current_tile + corner::BL];
// index_buffer[ib_pivot++] = all_index_buffer[current_tile + corner::BR];
// index_buffer[ib_pivot++] = all_index_buffer[current_tile + corner::UR];
// index_buffer[ib_pivot++] = all_index_buffer[current_tile + corner::UL];
//         }
//   }
