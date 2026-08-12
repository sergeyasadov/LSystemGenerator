#pragma once

#include "api/SDL.hpp"
#include "ui/ImGuiLayer.hpp"
#include "ui/editor/Pipeline.hpp"
#include "Camera.hpp"

#include <quill/Logger.h>
#include <filesystem>


class Application final {
    bool running;
    API::SDL sdl;
    UI::ImGuiLayer imguiLayer;
    quill::Logger* logger;
    std::filesystem::path dataPath;
    UI::Editor::Project project;
    UI::Editor::Pipeline pipelineEditor;
    Camera camera;
    Turtle::Builder builder;


public:
    Application();
    ~Application();
    void run();

private:
    void loadProject();
    void saveProject();
    void rebuildSystem();
    void drawSystem(SDL_Renderer*);
    void drawEditor(SDL_Renderer*);
};
