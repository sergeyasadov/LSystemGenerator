#include "Application.hpp"
#include "Defines.hpp"
#include "Screen.hpp"
#include "LSystem.hpp"

#include <cmath>

Application::Application() : running(true), sdl() {

}


Application::~Application() {

}


// september пн 7 число - dr serega  ?

void Application::run() {
    SDL_Log("Application started");


    LSystem lsys;
    lsys.set_axiom("F");
    lsys.set_rules("F", "F+F-F-F+F");
    lsys.set_iters(8);
    lsys.run();
    std::string result = lsys.get_axiom();

    SDL_Log("L-System iterations: %u", lsys.get_iters());
    SDL_Log("L-System size: %zu MB", result.size() / 8 / 1024);


    std::vector<SDL_FPoint> worldLine;
    worldLine.resize(result.size());
    worldLine[0] = {0, 0};

    for(auto i = 1, j = 0, rot = 0; i < worldLine.size(); i++, j++){
        worldLine[i] = worldLine[i - 1];
        if(result[j] == 'F') {
            worldLine[i].x += 10 * std::cos(rot * M_PI_2);
            worldLine[i].y += 10 * std::sin(rot * M_PI_2);
        }
        else if(result[j] == '+') {
            rot += 1;
        }
        else if(result[j] == '-') {
            rot -= 1;
        }
    }
    SDL_Log("worldLine size = %zu", worldLine.size());


    constexpr float FACTOR = 12.5f;

    while(this->running) {

        this->sdl.handleEvent({
            {SDL_EVENT_QUIT,
                [&](SDL_Event& event) {
                    this->running = false;
                    SDL_Log("Event: quit");
                }
            },
            {SDL_EVENT_KEY_DOWN,
                [&](SDL_Event& event) {
                    switch(event.key.scancode) {
                        case SDL_SCANCODE_ESCAPE : {
                            this->running = false;
                            SDL_Log("Event: key_escape");
                        }

                        case SDL_SCANCODE_A : {
                            for(auto i = 0; i < worldLine.size(); ++i) {
                                worldLine[i].x += Defines::Window::WIDTH / 3;
                            }
                            break;
                        }

                        case SDL_SCANCODE_D : {
                            for(auto i = 0; i < worldLine.size(); ++i) {
                                worldLine[i].x -= Defines::Window::WIDTH / 3;
                            }
                            break;
                        }

                        case SDL_SCANCODE_W : {
                            for(auto i = 0; i < worldLine.size(); ++i) {
                                worldLine[i].y += Defines::Window::HEIGHT / 3;
                            }
                            break;
                        }

                        case SDL_SCANCODE_S : {
                            for(auto i = 0; i < worldLine.size(); ++i) {
                                worldLine[i].y -= Defines::Window::HEIGHT / 3;
                            }
                            break;
                        }

                        default: {}
                    }
                }
            },
        });

        this->sdl.render({
            [&](SDL_Renderer* renderer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                SDL_RenderLines(renderer,
                    worldLine.data(),
                    static_cast<int>(worldLine.size())
                );

            },
        });


    }
}
