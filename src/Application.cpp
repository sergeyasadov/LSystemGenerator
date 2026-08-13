#include "Application.hpp"
#include "Defines.hpp"
#include "SDL3/SDL_filesystem.h"
#include "core/Compiler.hpp"
#include "Turtle.hpp"
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
        pipelineEditor(),
        project(),
        camera(1, 20.f),
        builder({0.f, 0.f}, 0.f, 2.f),
        dataPath(std::filesystem::path(SDL_GetUserFolder(SDL_FOLDER_DOCUMENTS)) / Defines::DATA_FILE)
{
    this->logger = quill::Frontend::get_logger(Defines::Logger::Names::APPLICATION);
    LOG_INFO(this->logger, "Default save data path: {}", this->dataPath.string());

    try {
        this->imguiLayer.init(this->sdl.getWindow(), this->sdl.getRenderer());
    } catch (const std::exception& e) {
        LOG_ERROR(this->logger, "Failed to initialize ImGUI: {}", e.what());
        return;
    }

    int winWidth{};
    int winHeight{};
    this->sdl.getSize(&winWidth, &winHeight);
    this->camera.setWindowSize(winWidth, winHeight);

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
void Application::loadProject() {
    std::ifstream readDataFile(this->dataPath);
    glz::basic_istream_buffer<std::ifstream> buffer(readDataFile);

    auto ec = glz::read_json(this->project, buffer);
    if(ec) {
        LOG_WARNING(this->logger, "Failed to read data");
    }
    this->pipelineEditor.setData(this->project);
}
//==============================================================================
void Application::saveProject() {
    std::ofstream writeDataFile(this->dataPath);
    glz::basic_ostream_buffer<std::ofstream> buffer(writeDataFile);
    auto ex = glz::write_json(this->pipelineEditor.getData(), buffer);

    if(ex.ec != glz::error_code::none) {
        LOG_ERROR(this->logger, "Failed to save LSystem");
    }
    else {
        LOG_INFO(this->logger, "LSystem data saved");
    }
}
//==============================================================================
void Application::drawSystem() {
    this->sdl.setRenderDrawColor({1.f, 1.f, 1.f, 1.f});

    for(const auto& [start, end] : this->builder.getSegments()) {
        if(this->camera.isVisible(start, end)) {
            continue;
        }

        if(!this->sdl.renderLine(this->camera.transform(start, end))) {
            LOG_ERROR(this->logger, "Failed to render line: {}", SDL_GetError());
        }
    }
}
//==============================================================================
void Application::drawEditor(SDL_Renderer* renderer) {
    this->imguiLayer.render(renderer, [&] {

        if(ImGui::Begin("Editor")) {
            if(this->pipelineEditor.draw()) {

                auto result = this->pipelineEditor.build();

                if(result.has_value()) {
                    try {
                        auto lsys = std::move(result.value());
                        lsys.run();

                        Turtle::Builder candidate({0.f, 0.f}, 0.f, 2.f);
                        LSystem::Compiler compiler;
                        compiler.compile(lsys, candidate);

                        this->builder = std::move(candidate);
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
                this->saveProject();
            }

            ImGui::End();
        }
    });
}
//==============================================================================
void Application::run() {
    LOG_INFO(this->logger, "Application started");

    // std::vector<Editor::Link> links;
    // int nextLinkId{1000};

    this->loadProject();

    auto previousTime = SDL_GetTicksNS();

    while(this->running) {

        const auto currentTime = SDL_GetTicksNS();

        const float dt = std::min(
            static_cast<float>(currentTime - previousTime) / 1'000'000'000.f,
            0.05f
        );

        previousTime = currentTime;

        if(!ImGui::GetIO().WantCaptureKeyboard) {
            this->camera.update(dt);
        }

        while(auto event = this->sdl.pollEvent()) {

            this->imguiLayer.processEvent(event.value());

            switch(event.value().type) {
                case SDL_EVENT_QUIT: {
                    this->running = false;
                    LOG_DEBUG(this->logger, "Event: quit");
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    switch(event.value().key.scancode) {
                        case SDL_SCANCODE_ESCAPE : {
                            this->running = false;
                            LOG_DEBUG(this->logger, "Event: key_escape");
                            break;
                        }

                        case SDL_SCANCODE_1: {
                            this->builder.setConvertCoords(Turtle::CoordSystem::DEFAULT);
                            LOG_INFO(this->logger, "Set convert coords to DEFAULT");
                            break;
                        }
                        case SDL_SCANCODE_2: {
                            this->builder.setConvertCoords(Turtle::CoordSystem::SCREEN);
                            LOG_INFO(this->logger, "Set convert coords to SCREEN");
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

                this->drawSystem();

                this->drawEditor(renderer);
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
