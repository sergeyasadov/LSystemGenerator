#include "Application.hpp"
#include "core/Interpreter.hpp"
#include "core/Node.hpp"
#include "core/Typedefs.hpp"
#include "Turtle.hpp"
#include "Camera.hpp"


#include <imgui.h>

//==============================================================================
Application::Application() : running(true), sdl(), imguiLayer() {

}
//==============================================================================
Application::~Application() {

}
//==============================================================================
// september пн 7 число - dr serega  ?

void Application::run() {
    SDL_Log("Application started");

    try {
        this->imguiLayer.init(this->sdl.getWindow(), this->sdl.getRenderer());
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize ImGUI: %s", e.what());
        return;
    }

    LSystem::Interpreter lsys;

    LSystem::Variable var1;
    var1.value = 'A';
    var1.command = [](Turtle::Builder& builder) {
        builder.forward(true);
    };

    LSystem::Variable var2;
    var2.value = 'B';
    var2.command = [](Turtle::Builder& builder) {
        builder.forward(true);
    };

    LSystem::Rule rule1;
    rule1.var = var1;
    rule1.action = "B-A-B";

    LSystem::Rule rule2;
    rule2.var = var2;
    rule2.action = "A+B+A";

    LSystem::Constant const1;
    const1.value = '+';
    const1.command = [](Turtle::Builder& builder) {
        builder.rotate(60.f);
    };

    LSystem::Constant const2;
    const2.value = '-';
    const2.command = [](Turtle::Builder& builder) {
        builder.rotate(-60.f);
    };

    LSystem::Axiom axiom = "A";

    try {
        lsys.setConstants({const1, const2});
        lsys.setRule({rule1, rule2});
        lsys.setIterations(10);
        lsys.setAxiom(axiom);
    }
    catch (const std::exception& e) {
        SDL_Log("EXCEPTION: %s", e.what());
        return;
    }
    lsys.run();

    Turtle::Builder builder({100, 300}, 0.f, 4.f);

    LSystem::Node node;
    node.add(lsys);
    node.execute(builder);

    Camera camera(1, 20.f);

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
                            break;
                        }

                        default: {
                            camera.handleEvent(event);
                            break;
                        }
                    }
                }
            }
        });

        this->sdl.render({
            [&](SDL_Renderer* renderer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                for(const auto& [start, end] : builder.getSegments()) {
                    if(!SDL_RenderLine(
                        renderer,
                        start.x * camera.getScale() + camera.getOffsetX(),
                        start.y * camera.getScale() + camera.getOffsetY(),
                        end.x * camera.getScale() + camera.getOffsetX(),
                        end.y * camera.getScale() + camera.getOffsetY())) {
                        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                            "Failed to render line: %s", SDL_GetError()
                        );
                    }
                }

            },
        });

    }
}
//==============================================================================
