#ifndef APPLICTION_HPP
#define APPLICTION_HPP

#include "api/SDL.hpp"


class Application final {
    bool running;
    API::SDL sdl;

public:
    Application();
    ~Application();
    void run();
};

#endif // APPLICTION_HPP
