#ifndef APPLICTION_HPP
#define APPLICTION_HPP

#include "api/SDL.hpp"
#include "ui/ImGuiLayer.hpp"

class Application final {
    bool running;
    API::SDL sdl;
    UI::ImGuiLayer imguiLayer;

public:
    Application();
    ~Application();
    void run();
};

#endif // APPLICTION_HPP
