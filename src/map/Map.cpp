#pragma once

Map::Map()
    : Grid({})
{
}

void Map::loadFromFile(const std::string_view& filename, const std::string_view& allowed_chars)
{
    std::ifstream file(reinterpret_cast<const char*>(filename.data()));
    if(!file.is_open() || !file.good())
    {
        spdlog::debug("Couldn't open file: {}", filename);
        return;
    }

    std::stringstream file_buffer;
    file_buffer << file.rdbuf();
    file.close();

    std::string buffer = file_buffer.str();
    // need to think about the \n characters
    // if(buffer.size() < WIDTH * HEIGHT)
    // {
    //     spdlog::debug("Contents of the file '{}' are invalid", filename);
    //     return;
    // }
    spdlog::debug("Character count: {}", buffer.size());

    std::string skipped_chars = "\n\t\r\b\0\e\f\v ";
    std::size_t iter = 0;
    for(auto& ch : buffer)
    {
        if(iter >= WIDTH * HEIGHT)
        {
            break;
        }
        else if(skipped_chars.find(ch) != skipped_chars.npos)  // silently ignore
        {
            continue;
        }
        else if((allowed_chars.find(ch) == allowed_chars.npos))
        {
            spdlog::debug("Found invalid character '{}' (char {}) in a file contents", ch, ch - '0');
            continue;
        }
        spdlog::trace("character: '{}', converted to ascii: '{}'", ch, ch - '0');
        Grid::m_tiles[iter / WIDTH][iter % WIDTH] = (ch - '0');  // for some reason m_tiles is not accessible here...
        iter++;
    }
    spdlog::debug("Reading map from file has finished successfully");
    for(int i = 0; i < WIDTH; i++)
    {
        for(int j = 0; j < HEIGHT; j++)
        {
            spdlog::trace("{} ", Grid::m_tiles[i][j]);
        }
        spdlog::trace("");
    }
}
