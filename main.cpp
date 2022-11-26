#include <stdio.h>

#include "include/App.hpp"
#include "include/input/InputManager.hpp"
#include "include/shader/ShaderManager.hpp"

int main(int, char**)
{
    App::initLogger();
    App::initAudio();

    App app("MONOSHOT", 1280, 720);
    app.run();

    return 0;
}
