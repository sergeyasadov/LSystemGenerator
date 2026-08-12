#pragma once

#include "api/SDL.hpp"
#include "ui/ImGuiLayer.hpp"

#include <quill/Logger.h>
#include <filesystem>


class Application final {
    bool running;
    API::SDL sdl;
    UI::ImGuiLayer imguiLayer;
    quill::Logger* logger;
    std::filesystem::path dataPath;

public:
    Application();
    ~Application();
    void run();
};
