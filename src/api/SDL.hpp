#ifndef API_SDL_HPP
#define API_SDL_HPP

#include <SDL3/SDL.h>
#include <functional>


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

    public:
        SDL();
        ~SDL();

        void handleEvent(std::vector<EventHandle> handles);

        void render(std::vector<std::function<void(SDL_Renderer*)>> renderers) const;
    };
}

#endif // API_SDL_HPP
