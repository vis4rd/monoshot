#include "include/App.hpp"

int main(int, char**)
{
    App::initLogger();

    App app("MONOSHOT", 1280, 720);
    app.run();
    app.terminate(EXIT_SUCCESS);

    return EXIT_SUCCESS;
}
