#include <config/Config.hpp>
#include <mono/log/Logging.hpp>

#include "include/App.hpp"

int main(int, char**)
{
    mono::config::initialize();
    mono::log::initialize();

    App app("MONOSHOT");
    app.run();
    app.terminate(EXIT_SUCCESS);

    return EXIT_SUCCESS;
}
