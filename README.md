# Monoshot
A simple top-down 2D shooter game written in C++ and OpenGL 4.5.

## Engineering thesis
This is an implementation of my engineering thesis work titled:  
**"Top-down shooter 2D arcade game written in C++"** (eng)  
**"Zręcznościowa gra 2D typu top-down shooter napisana w C++"** (pl)  
  
The description of this thesis is as follows:  
English:
> The aim of the thesis is the creation of an arcade top-down shooter game. The gameplay is based on overcoming obstacles met on a few levels, on which the player is faced with simple tasks. The game is implemented with a help of C++ libraries.

Polish:
> Celem pracy jest stworzenie gry zręcznościowej typu top-down shooter. Rozgrywka będzie się opierać na pokonaniu przeciwności rozłożonych na kilku poziomach, na których do wykonania przez gracza będą proste zadania. Praca zaimplementowana będzie z wykorzystaniem bibliotek w C++.

## Gameplay
Currently, plans are to add 3 levels: tutorial, summer-themed and winter-themed. There are also 3 types of enemies which can be encountered on those levels. The goal of each map is to go from point A to B and sometimes find a quest item in the process.

## Roadmap
Currently planned features and to-do list can be found on [this Notion site](https://visard.notion.site/d62743bdc1ea41bda69a7c655179999e?v=3a975abae6ac4bafb9146d1072b42e60). In future it will be moved to issues here.

## Building with CMake
I've chosen CMake as it allows cross-platform development and is easy to use:
```bash
# clone the repository
$ cd monoshot
$ git submodule update --init
$ mkdir build && cd build
$ cmake [-DCMAKE_BUILD_TYPE=Debug] [-G "MinGW Makefiles"] ..
```
There are several ways to build the code depending on platform and CMake generator:
```bash
# on windows: open Visual Studio solution and build there
$ ./OPENGL_TEMPLATE.sln

# on linux: run make
$ make

# with MinGW generator: run mingw32-make
$ mingw32-make

# with Ninja generator: run ninja
$ ninja
```

Build with C++20 capable compiler such as GCC 12+ or Clang 14+.

#### Known issues
- Sometimes the build fails randomly on Visual Studio, probably due to dynamically generated Glad headers
- Ninja builds have to be repeated a couple times in order to work, sometimes clearing cache helps

## Dependencies
All libraries are fetched either by git submodules or by CMake and their use is compliant with their respective licences:
- [EnTT](https://github.com/skypjack/entt) - Entity Component System (MIT)
- [glad](https://github.com/Dav1dde/glad) - OpenGL symbol loader/generator (MIT + Public Domain, WTFPL or CC0)
- [glfw](https://github.com/glfw/glfw) - window and OpenGL context creation API (zlib/libpng)
- [glm](https://github.com/g-truc/glm) - OpenGL mathematics library (Happy Bunny License or MIT)
- [ImGui](https://github.com/ocornut/imgui) - Graphical User Interface (MIT)
- [spdlog](https://github.com/gabime/spdlog) - Logging library (MIT)
- [stbi](https://github.com/nothings/stb) - Image loading/decoding from file/memory (Public Domain or MIT)
- [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) - Native window dialogs and popups (zlib)
