#include "SDL.hpp"
#include "../Defines.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"

#include <quill/Frontend.h>
#include <quill/LogMacros.h>

//==============================================================================
API::SDL::SDL()
:   window(nullptr),
    renderer(nullptr),
    windowFlags(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY),
    event(0),
    logger(quill::Frontend::get_logger(Defines::Logger::Names::SDL))
{

    if(this->logger == nullptr) {
        SDL_Log("Failed to get logger for SDL");
        return;
    }

    this->window = SDL_CreateWindow(
        Defines::Window::TITLE,
        Defines::Window::WIDTH,
        Defines::Window::HEIGHT,
        this->windowFlags
    );
    if(this->window == nullptr) {
        LOG_INFO(this->logger, "Failed to create window: {}", SDL_GetError());
    }
    LOG_INFO(this->logger, "Window created successfully");

    this->renderer = SDL_CreateRenderer(this->window, nullptr);
    if(this->renderer == nullptr) {
        LOG_INFO(this->logger, "Failed to create renderer: {}", SDL_GetError());
    }
    LOG_INFO(this->logger, "Renderer created successfully");

}
//==============================================================================
API::SDL::~SDL() {
    SDL_DestroyRenderer(this->renderer);
    LOG_INFO(this->logger, "Renderer destroyed successfully");

    SDL_DestroyWindow(this->window);
    LOG_INFO(this->logger, "Window destroyed successfully");

    SDL_Quit();
    LOG_INFO(this->logger, "SDL quit successfully");
}
//==============================================================================
std::optional<SDL_Event> API::SDL::pollEvent() {
    if(SDL_PollEvent(&this->event)) {
        return this->event;
    }
    return std::nullopt;
}
//==============================================================================
SDL_Window* API::SDL::getWindow() const {
    return this->window;
}
//==============================================================================
SDL_Renderer* API::SDL::getRenderer() const {
    return this->renderer;
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
void API::SDL::getSize(int* x, int* y) const {
    SDL_GetRenderOutputSize(this->renderer, x, y);
}
//==============================================================================
