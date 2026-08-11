#pragma once

#include <imgui.h>
#include <imnodes.h>
#include <SDL3/SDL.h>
#include <functional>

#include <quill/Logger.h>

namespace UI {

    class ImGuiLayer {
        ImGuiContext* context;
        ImNodesContext* nodesContext;
        ImGuiIO* io;
        ImGuiStyle* style;
        float dpiScale {1.f};

        quill::Logger* logger;

        bool platformInit{false};
        bool renderInit{false};

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void init(SDL_Window* window, SDL_Renderer* renderer);

        void processEvent(SDL_Event& event);

        void render(SDL_Renderer* renderer, std::function<void()> f);
    };

}
