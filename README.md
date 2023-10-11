# Monoshot

A simple top-down 2D shooter game written in C++ and OpenGL 4.5.

## Mono Engine

Currently there is an effort to separate the engine source code from the game itself to improve
flexibility for future possible projects.

## Gameplay

At the current stage of full refactor, the game does not offer any gameplay (not like it ever did).
But there are plans to create completely destructible tile-based maps and dynamic environment.

## Building

The project supports building with CMake as it allows cross-platform development and is relatively
easy to use:

```bash
# clone the repository
cd monoshot
git submodule update --init
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "<GENERATOR>" ..
```

Replace `<GENERATOR>` with your favourite build generator. Recommended choices are listed below:

- `Ninja`,
- `Unix Makefiles`,
- `MinGW Makefiles`.

On Windows, there is a possibility that it will even work with Visual Studio generated `.sln` file,
however this option has never been properly tested or supported.

After the configuration is complete, build the code with:

```bash
cmake --build .
```

Monoshot requires C++20 capable compiler such as GCC 13+ or Clang 14+ (preferably supporting
`<format>` library) with MSVCRT runtime library [^1].

## Dependencies

All libraries are fetched either by git submodules or by CMake and their use is compliant with
their respective licences:

- [EnTT](https://github.com/skypjack/entt) - Entity Component System (`MIT`)
- [ImGui](https://github.com/ocornut/imgui) - Graphical User Interface (`MIT`)
- [SFML](https://github.com/SFML/SFML) - Sound and music playing (`zlib`)
- [glad](https://github.com/Dav1dde/glad) - OpenGL symbol loader/generator (`MIT` +
`Public Domain`, `WTFPL` or `CC0`)
- [glfw](https://github.com/glfw/glfw) - Window and OpenGL context creation API (`zlib`/`libpng`)
- [glm](https://github.com/g-truc/glm) - OpenGL mathematics library (`Happy Bunny License` or `MIT`)
- [inifile-cpp](https://github.com/Rookfighter/inifile-cpp) -Config files handling (`MIT`)
- [spdlog](https://github.com/gabime/spdlog) - Logging library (`MIT`)
- [stbi](https://github.com/nothings/stb) - Image loading/decoding from file/memory (
`Public Domain` or `MIT`)
- [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) - Native window dialogs and
popups (`zlib`)

## License

This project is licensed under MIT, a free and open-source license. For more information, please
see [the license file](LICENSE.md).

> NOTE: `thesis` branch is licensed under its own EULA, which is not applicable here. At the
> same time, MIT license does not pertain to `thesis` branch.

[^1]: MSVCRT is required only because of SFML, which precompiles libFLAC (and other dependencies)
with the mentioned runtime library.
