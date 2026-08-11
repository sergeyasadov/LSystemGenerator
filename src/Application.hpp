#ifndef APPLICTION_HPP
#define APPLICTION_HPP

#include "api/SDL.hpp"
#include "ui/ImGuiLayer.hpp"

#include <quill/Logger.h>

class Application final {
    bool running;
    API::SDL sdl;
    UI::ImGuiLayer imguiLayer;
    quill::Logger* logger;

public:
    Application();
    ~Application();
    void run();
};

#endif // APPLICTION_HPP
