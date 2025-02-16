# Monoshot

It's nothing more than my engine/framework sandbox for learning graphics-related stuff and just playing around and experimenting for fun.

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

After the configuration is complete, build the code with:

```bash
cmake --build .
```

C++20 capable compiler is required such as GCC 14.2+ or Clang 19.1+ (supporting `<format>` and `<concepts>`).

## Dependencies

All libraries are fetched either by git submodules or by CMake and their use is compliant with
their respective licences:

| Library                                                              | Description                             | Licensing                                 |
| -------------------------------------------------------------------- | --------------------------------------- | ----------------------------------------- |
| [EnTT](https://github.com/skypjack/entt)                             | Entity Component System                 | `MIT`                                     |
| [ImGui](https://github.com/ocornut/imgui)                            | Graphical User Interface                | `MIT`                                     |
| [glad](https://github.com/Dav1dde/glad)                              | OpenGL symbol loader/generator          | `MIT` + `Public Domain`, `WTFPL` or `CC0` |
| [glfw](https://github.com/glfw/glfw)                                 | Window and OpenGL context creation API  | `zlib`/`libpng`                           |
| [glm](https://github.com/g-truc/glm)                                 | OpenGL mathematics library              | `Happy Bunny License` or `MIT`            |
| [inifile-cpp](https://github.com/Rookfighter/inifile-cpp)            | Config files handling                   | `MIT`                                     |
| [spdlog](https://github.com/gabime/spdlog)                           | Logging library                         | `MIT`                                     |
| [stbi](https://github.com/nothings/stb)                              | Image loading/decoding from file/memory | `Public Domain` or `MIT`                  |
| [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) | Native window dialogs and popups        | `zlib`                                    |

## License

This project is licensed under MIT, a free and open-source license. For more information, please
see [the license file](LICENSE.md).

> [!NOTE]
> `thesis` branch is licensed under its own EULA, which is not applicable here. At the same time,
> MIT license does not pertain to `thesis` branch.
