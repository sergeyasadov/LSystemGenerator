#include "Application.hpp"
#include "core/Interpreter.hpp"
#include "core/Pipeline.hpp"
#include "core/Typedefs.hpp"
#include "Turtle.hpp"
#include "ui/editor/Pipeline.hpp"
#include "Camera.hpp"

#include <array>
#include <cstddef>
#include <cstring>
#include <imgui.h>
#include <imnodes.h>
#include <format>
#include <cstdint>
#include <variant>

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
    // std::vector<Editor::Link> links;
    // int nextLinkId{1000};

    Camera camera(1, 20.f);
    // Turtle::Builder builder;s
    UI::Editor::Pipeline uiEditor;

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

                    if(ImGui::Begin("Editor")) {
                        if(uiEditor.draw()) {
                            auto result = uiEditor.build();

                            if(result.has_value()) {
                                auto lsys = std::move(result.value());
                                lsys.run();
                                lsys.print();
                            }
                            else {
                                SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                                    "Failed to build LSystem: %s",
                                    result.error().c_str()
                                );
                            }
                        }
                    }
                    ImGui::End();



                    // if(ImGui::Begin("Editor")) {

                    //     ImNodes::BeginNodeEditor();

                    //     ImNodes::BeginNode(1);
                    //     ImNodes::BeginNodeTitleBar();
                    //     ImGui::TextUnformatted("Node 1");
                    //     ImNodes::EndNodeTitleBar();

                    //     ImNodes::BeginOutputAttribute(101);
                    //     ImGui::TextUnformatted("Value out");
                    //     ImNodes::EndOutputAttribute();
                    //     ImNodes::EndNode();

                    //     ImNodes::BeginNode(2);
                    //     ImNodes::BeginNodeTitleBar();
                    //     ImGui::TextUnformatted("Node 2");
                    //     ImNodes::EndNodeTitleBar();

                    //     ImNodes::BeginInputAttribute(201);
                    //     ImGui::TextUnformatted("Value in");
                    //     ImNodes::EndInputAttribute();
                    //     ImNodes::EndNode();

                    //     for(const auto& link : links) {
                    //         ImNodes::Link(
                    //             link.id,
                    //             link.fromAttr,
                    //             link.toAttr
                    //         );
                    //     }
                    //     ImNodes::EndNodeEditor();
                    // }
                    // ImGui::End();
                });
            }

        });


        // int startAttr, endAttr;
        // if(ImNodes::IsLinkCreated(&startAttr, &endAttr)) {
        //     links.push_back({nextLinkId++, startAttr, endAttr});
        // }

        // int linkDestr;
        // if(ImNodes::IsLinkDestroyed(&linkDestr)) {
        //     for(auto it = links.begin(); it != links.end(); ++it) {
        //         if(it->id == linkDestr) {
        //             links.erase(it);
        //             break;
        //         }
        //     }
        // }
    }
}
//==============================================================================
