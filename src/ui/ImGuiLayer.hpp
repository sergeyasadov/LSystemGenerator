#pragma once

#include <imgui.h>
#include <SDL3/SDL.h>
#include <functional>

namespace UI {

    class ImGuiLayer {
        ImGuiContext* context;
        ImGuiIO* io;

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void init(SDL_Window* window, SDL_Renderer* renderer);

        void processEvent(SDL_Event& event);

        void render(SDL_Renderer* renderer, std::function<void()>& f);
    };

}
