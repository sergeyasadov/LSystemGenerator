#include "Application.hpp"
#include "core/Interpreter.hpp"
#include "core/Node.hpp"
#include "core/Typedefs.hpp"
#include "Turtle.hpp"
#include "Camera.hpp"

#include <imgui.h>
#include <imnodes.h>

//==============================================================================
Application::Application() : running(true), sdl(), imguiLayer() {

}
//==============================================================================
Application::~Application() {

}
//==============================================================================
void Application::run() {
    SDL_Log("Application started");

    try {
        this->imguiLayer.init(this->sdl.getWindow(), this->sdl.getRenderer());
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize ImGUI: %s", e.what());
        return;
    }

    Camera camera(1, 20.f);


    while(this->running) {

        while(auto event = this->sdl.pollEvent()) {

            this->imguiLayer.processEvent(event.value());

            switch(event.value().type) {
                case SDL_EVENT_QUIT: {
                    this->running = false;
                    SDL_Log("Event: quit");
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    switch(event.value().key.scancode) {
                        case SDL_SCANCODE_ESCAPE : {
                            this->running = false;
                            SDL_Log("Event: key_escape");
                            break;
                        }

                        default: {
                            camera.handleEvent(event.value());
                            break;
                        }
                    }
                    break;
                }
            }

        }

        this->sdl.render({
            [&](SDL_Renderer* renderer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                // for(const auto& [start, end] : builder.getSegments()) {
                //     if(!SDL_RenderLine(
                //         renderer,
                //         start.x * camera.getScale() + camera.getOffsetX(),
                //         start.y * camera.getScale() + camera.getOffsetY(),
                //         end.x * camera.getScale() + camera.getOffsetX(),
                //         end.y * camera.getScale() + camera.getOffsetY())) {
                //         SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                //             "Failed to render line: %s", SDL_GetError()
                //         );
                //     }
                // }

            },
            [&](SDL_Renderer* renderer) {
                this->imguiLayer.render(renderer, [&] {

                    if(ImGui::Begin("Test")) {

                        ImNodes::BeginNodeEditor();

                        ImNodes::BeginNode(1);
                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("Node 1");
                        ImNodes::EndNodeTitleBar();

                        ImNodes::BeginOutputAttribute(101);
                        ImGui::TextUnformatted("Value out");
                        ImNodes::EndOutputAttribute();
                        ImNodes::EndNode();

                        ImNodes::BeginNode(2);
                        ImNodes::BeginNodeTitleBar();
                        ImGui::TextUnformatted("Node 2");
                        ImNodes::EndNodeTitleBar();

                        ImNodes::BeginInputAttribute(201);
                        ImGui::TextUnformatted("Value in");
                        ImNodes::EndInputAttribute();
                        ImNodes::EndNode();
                        ImNodes::Link(1000, 101, 201);
                        ImNodes::EndNodeEditor();
                    }
                    ImGui::End();
                });
            }

        });

    }
}
//==============================================================================
