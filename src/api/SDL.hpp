#ifndef API_SDL_HPP
#define API_SDL_HPP

#include <SDL3/SDL.h>
#include <functional>
#include <optional>

#include <quill/Logger.h>

struct EventHandle {
    SDL_EventType eventType;
    std::function<void(SDL_Event&)> callback;
};

namespace API {

    class SDL {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_WindowFlags windowFlags;
        SDL_Event event;
        quill::Logger* logger;

    public:
        SDL();
        ~SDL();

        std::optional<SDL_Event> pollEvent();

        SDL_Window* getWindow() const;
        SDL_Renderer* getRenderer() const;

        void getSize(int* x, int* y) const;

        void render(std::vector<std::function<void(SDL_Renderer*)>> renderers) const;
    };
}

#endif // API_SDL_HPP
