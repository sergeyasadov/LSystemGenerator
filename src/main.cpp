#include "Application.hpp"

#include "utils/Logger.hpp"


int main(int, char**) {
    Logger::init();

    Application app;

    app.run();

    return 0;
}
