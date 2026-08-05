#include "Application.hpp"
#include "Defines.hpp"
#include "Screen.hpp"
#include "core/Interpreter.hpp"
#include "core/Typedefs.hpp"

#include <cmath>

Application::Application() : running(true), sdl() {

}


Application::~Application() {

}


// september пн 7 число - dr serega  ?

void Application::run() {
    SDL_Log("Application started");

    LSystem::Interpreter lsys;

    LSystem::Variable var1 = 'F';

    LSystem::Rule rule1;
    rule1.var = var1;
    rule1.action = "F+F-F-F+F";

    LSystem::Constant const1 = '+';

    LSystem::Constant const2 = '-';

    try {
        lsys.setConstants({const1, const2});
        lsys.setRule({rule1});
        lsys.setIterations(4);
        lsys.setAxiom("F");
    }
    catch (std::exception& e) {
        SDL_Log("Error: %s", e.what());
        return;
    }

    lsys.run();
    lsys.print();
    auto data = lsys.getData();

    SDL_Log("L-System iterations: %u", lsys.getIterations());
    SDL_Log("L-System size: %zu MB", data.size() / 8 / 1024);


    std::vector<SDL_FPoint> worldLine;
    worldLine.resize(data.size());

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

                // SDL_RenderLines(renderer,
                //     worldLine.data(),
                //     static_cast<int>(worldLine.size())
                // );

            },
        });


    }
}
