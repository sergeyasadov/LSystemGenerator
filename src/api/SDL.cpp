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

    if(!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_CRITICAL(this->logger, "Failed to initialize SDL: {}", SDL_GetError());
        throw;
    }
    LOG_INFO(this->logger, "SDL initialized successfully");


    this->window = SDL_CreateWindow(
        Defines::Window::TITLE,
        Defines::Window::WIDTH,
        Defines::Window::HEIGHT,
        this->windowFlags
    );
    if(this->window == nullptr) {
        LOG_CRITICAL(this->logger, "Failed to create window: {}", SDL_GetError());
        throw;
    }
    LOG_INFO(this->logger, "Window created successfully");

    this->renderer = SDL_CreateRenderer(this->window, nullptr);
    if(this->renderer == nullptr) {
        LOG_CRITICAL(this->logger, "Failed to create renderer: {}", SDL_GetError());
        throw;
    }
    LOG_INFO(this->logger, "Renderer created successfully");

    LOG_INFO(this->logger, "Renderer backend: {}", SDL_GetRendererName(this->renderer));

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
glm::vec2 API::SDL::getRenderScale() const {
    glm::vec2 scale;
    SDL_GetRenderScale(this->renderer, &scale.x, &scale.y);
    return std::move(scale);
}
//==============================================================================
bool API::SDL::setRenderScale(glm::vec2 scale) {
    return SDL_SetRenderScale(this->renderer, scale.x, scale.y);
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
bool API::SDL::renderLine(glm::vec2 start, glm::vec2 end) const {
    return SDL_RenderLine(this->renderer, start.x, start.y, end.x, end.y);
}
//==============================================================================
bool API::SDL::renderLine(glm::vec4 line) const {
    return SDL_RenderLine(this->renderer, line.x, line.y, line.z, line.w);
}
//==============================================================================
void API::SDL::setRenderDrawColor(glm::vec4 color) const {
    SDL_SetRenderDrawColor(this->renderer, 255 * color.x, 255 * color.y, 255 * color.z, 255 * color.w);
}
//==============================================================================
