#ifndef API_SDL_HPP
#define API_SDL_HPP

#include <SDL3/SDL.h>
#include <quill/Logger.h>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

#include <functional>
#include <optional>


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

        SDL(const SDL&) = delete;
        SDL& operator=(const SDL&) = delete;

        std::optional<SDL_Event> pollEvent();

        SDL_Window* getWindow() const;
        SDL_Renderer* getRenderer() const;
        void getSize(int* x, int* y) const;
        glm::vec2 getRenderScale() const;

        [[nodiscard]] bool setRenderScale(glm::vec2 scale);

        void render(std::vector<std::function<void(SDL_Renderer*)>> renderers) const;

        bool renderLine(glm::vec2 start, glm::vec2 end) const;
        bool renderLine(glm::vec4 line) const;

        void setRenderDrawColor(glm::vec4 color) const;
    };
}

#endif // API_SDL_HPP
