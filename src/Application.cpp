#include "Application.hpp"
#include "Defines.hpp"
#include "SDL3/SDL_filesystem.h"
#include "core/Compiler.hpp"
#include "Turtle.hpp"
#include "core/Typedefs.hpp"
#include "ui/editor/Pipeline.hpp"
#include "Camera.hpp"

#include <imgui.h>
#include <imnodes.h>
#include <quill/LogMacros.h>
#include <quill/Frontend.h>
#include <glaze/json.hpp>
#include <glaze/core/istream_buffer.hpp>
#include <glaze/core/ostream_buffer.hpp>

#include <cstddef>
#include <cstring>

//==============================================================================
Application::Application()
    :   running(true),
        sdl(),
        imguiLayer(),
        dataPath(std::filesystem::path(SDL_GetUserFolder(SDL_FOLDER_DOCUMENTS)) / Defines::DATA_FILE)
{
    this->logger = quill::Frontend::get_logger(Defines::Logger::Names::APPLICATION);
    LOG_INFO(this->logger, "Default save data path: {}", this->dataPath.string());

    if(!std::filesystem::exists(this->dataPath)) {
        LOG_WARNING(this->logger, "Data file not found, creating new one");
        std::ofstream(this->dataPath).close();
        LOG_INFO(this->logger, "Data file created");
    }
}
//==============================================================================
Application::~Application() {
    LOG_INFO(this->logger, "Application stopped");
}
//==============================================================================
void Application::run() {
    LOG_INFO(this->logger, "Application started");

    try {
        this->imguiLayer.init(this->sdl.getWindow(), this->sdl.getRenderer());
    } catch (const std::exception& e) {
        LOG_ERROR(this->logger, "Failed to initialize ImGUI: {}", e.what());
        return;
    }

    // std::vector<Editor::Link> links;
    // int nextLinkId{1000};

    int winWidth{};
    int winHeight{};

    Camera camera(1, 20.f);
    Turtle::Builder builder({0.f, 0.f}, 0.f, 2.f);
    UI::Editor::Pipeline uiEditor;

    auto previousTime = SDL_GetTicksNS();


    std::ifstream readDataFile(this->dataPath);
    glz::basic_istream_buffer<std::ifstream> buffer(readDataFile);

    UI::Editor::SaveData readDataBuffer;

    auto ec = glz::read_json(readDataBuffer, buffer);
    if(ec) {
        LOG_WARNING(this->logger, "Failed to read data");
    }
    uiEditor.setData(readDataBuffer);

    while(this->running) {

        const auto currentTime = SDL_GetTicksNS();

        const float dt = std::min(
            static_cast<float>(currentTime - previousTime) / 1'000'000'000.f,
            0.05f
        );

        previousTime = currentTime;

        if(!ImGui::GetIO().WantCaptureKeyboard) {
            camera.update(dt);
        }

        while(auto event = this->sdl.pollEvent()) {

            this->imguiLayer.processEvent(event.value());

            switch(event.value().type) {
                case SDL_EVENT_QUIT: {
                    this->running = false;
                    LOG_INFO(this->logger, "Event: quit");
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    switch(event.value().key.scancode) {
                        case SDL_SCANCODE_ESCAPE : {
                            this->running = false;
                            LOG_INFO(this->logger, "Event: key_escape");
                            break;
                        }

                        default: {
                            break;
                        }
                    }
                    break;
                }
            }

        }

        this->sdl.render({
            [&](SDL_Renderer* renderer) {
                this->sdl.getSize(&winWidth, &winHeight);

                const auto scale = camera.getScale();
                const auto offsetX = camera.getOffsetX();
                const auto offsetY = camera.getOffsetY();

                const float worldLeft =  -offsetX / scale;
                const float worldRight = (winWidth - offsetX) / scale;
                const float worldTop = -offsetY / scale;
                const float worldBottom = (winHeight - offsetY) / scale;

                for(const auto& [start, end] : builder.getSegments()) {
                    const float minX = std::min(start.x, end.x);
                    const float minY = std::min(start.y, end.y);
                    const float maxX = std::max(start.x, end.x);
                    const float maxY = std::max(start.y, end.y);

                    if(maxX <= worldLeft
                        || minX >= worldRight
                        || maxY <= worldTop
                        || minY >= worldBottom) {
                        continue;
                    }

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                    if(!SDL_RenderLine(
                        renderer,
                        start.x * scale + offsetX,
                        start.y * scale + offsetY,
                        end.x * scale + offsetX,
                        end.y * scale + offsetY)
                    ) {
                        LOG_ERROR(this->logger, "Failed to render line: {}", SDL_GetError());
                    }
                }

                this->imguiLayer.render(renderer, [&] {

                    if(ImGui::Begin("Editor")) {

                        if(uiEditor.draw()) {
                            auto result = uiEditor.build();

                            if(result.has_value()) {
                                try {
                                    auto lsys = std::move(result.value());
                                    lsys.run();

                                    Turtle::Builder candidate({0.f, 0.f}, 0.f, 2.f);
                                    LSystem::Compiler compiler;
                                    compiler.compile(lsys, candidate);

                                    builder = std::move(candidate);
                                }
                                catch(const std::exception& e) {
                                    LOG_ERROR(this->logger, "Failed to compile LSystem: {}", e.what());
                                }
                            }
                            else {
                                LOG_ERROR(this->logger, "Failed to build LSystem: {}", result.error().c_str());
                            }
                        }

                        ImGui::SameLine();

                        if(ImGui::Button("Save")) {
                            std::ofstream writeDataFile(this->dataPath);
                            glz::basic_ostream_buffer<std::ofstream> buffer(writeDataFile);
                            auto ex = glz::write_json(uiEditor.getData(), buffer);

                            if(ex.ec != glz::error_code::none) {
                                LOG_ERROR(this->logger, "Failed to save LSystem");
                            }
                            else {
                                LOG_INFO(this->logger, "LSystem data saved");
                            }
                        }

                    }
                    ImGui::End();

                    if(ImGui::Begin("Nodes")) {
                        ImNodes::BeginNodeEditor();



                        ImNodes::EndNodeEditor();
                    }
                    ImGui::End();

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
