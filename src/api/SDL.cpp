#include "SDL.hpp"
#include "../Defines.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"


API::SDL::SDL()
:   window(nullptr),
    renderer(nullptr),
    windowFlags(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY),
    event(0) {

    this->window = SDL_CreateWindow(
        Defines::Window::TITLE,
        Defines::Window::WIDTH,
        Defines::Window::HEIGHT,
        this->windowFlags
    );
    if(this->window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window: %s", SDL_GetError());
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window created successfully");

    this->renderer = SDL_CreateRenderer(this->window, nullptr);
    if(this->renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create renderer: %s", SDL_GetError());
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Renderer created successfully");

}
//==============================================================================
API::SDL::~SDL() {
    SDL_DestroyRenderer(this->renderer);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Renderer destroyed successfully");

    SDL_DestroyWindow(this->window);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window destroyed successfully");

    SDL_Quit();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL quit successfully");
}
//==============================================================================
void API::SDL::handleEvent(std::vector<EventHandle> handles) {
    while(SDL_PollEvent(&this->event)) {
        SDL_ConvertEventToRenderCoordinates(this->renderer, &this->event);
        for(auto& handle : handles) {
            if(this->event.type == handle.eventType) {
                handle.callback(this->event);
            }
        }
    }
}
//==============================================================================
void API::SDL::render(std::vector<std::function<void(SDL_Renderer*)>> renderers) const {
    SDL_SetRenderDrawColor(this->renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->renderer);
    for(auto& r : renderers) {
        std::invoke(r, this->renderer);
    }
    SDL_RenderPresent(this->renderer);
}
//==============================================================================
