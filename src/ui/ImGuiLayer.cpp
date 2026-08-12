#include "ImGuiLayer.hpp"
#include "../Defines.hpp"

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <quill/LogMacros.h>
#include <quill/Frontend.h>

//==============================================================================
UI::ImGuiLayer::ImGuiLayer() {
    IMGUI_CHECKVERSION();

    this->logger = quill::Frontend::get_logger(Defines::Logger::Names::IMGUI_LAYER);

    this->context = ImGui::CreateContext();
    if(!this->context) {
        throw std::runtime_error("Failed to create ImGui context");
    }
    LOG_INFO(this->logger, "Context created");


    this->nodesContext = ImNodes::CreateContext();
    if(!this->nodesContext) {
        throw std::runtime_error("Failed to create ImNodes context");
    }
    LOG_INFO(this->logger, "ImNodes context created");


    this->io = &ImGui::GetIO();
    this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    this->io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    this->style = &ImGui::GetStyle();

    ImGui::StyleColorsDark();
    ImNodes::StyleColorsDark();
}

void UI::ImGuiLayer::init(SDL_Window* window, SDL_Renderer* renderer) {
    this->style->ScaleAllSizes(this->dpiScale);
    this->style->FontScaleDpi = this->dpiScale;
    LOG_INFO(this->logger, "ImGui style set");
    LOG_INFO(this->logger, "ImGui style scaled by dpi scale: {}", this->dpiScale);

    if(!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        throw std::runtime_error("Failed to initialize ImGui for SDL renderer");
    }
    LOG_INFO(this->logger, "Platform init");

    this->platformInit = true;
    if(!ImGui_ImplSDLRenderer3_Init(renderer)) {
        ImGui_ImplSDL3_Shutdown();
        this->platformInit = false;
        throw std::runtime_error("Failed to initialize ImGui for SDL renderer");
    }
    LOG_INFO(this->logger, "Render init");
    this->renderInit = true;
}

//==============================================================================
UI::ImGuiLayer::~ImGuiLayer() {
    if(this->renderInit) {
        ImGui_ImplSDLRenderer3_Shutdown();
        LOG_INFO(this->logger, "Platform shutdown");
    }
    if(this->platformInit) {
        ImGui_ImplSDL3_Shutdown();
        LOG_INFO(this->logger, "Render shutdown");
    }

    if(this->nodesContext != nullptr) {
        ImNodes::DestroyContext(this->nodesContext);
        LOG_INFO(this->logger, "Nodes context shutdown");
    }

    if(this->context != nullptr) {
        ImGui::DestroyContext(this->context);
        LOG_INFO(this->logger, "Context shutdown");
    }
}
//==============================================================================
void UI::ImGuiLayer::processEvent(SDL_Event& event) {
    ImGui_ImplSDL3_ProcessEvent(&event);
}
//==============================================================================
void UI::ImGuiLayer::render(SDL_Renderer* renderer, std::function<void()> f) {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();
    f();
    ImGui::Render();

    float prevScaleX = 1.f;
    float prevScaleY = 1.f;

    SDL_GetRenderScale(renderer, &prevScaleX, &prevScaleY);
    const auto framebufferScale = this->io->DisplayFramebufferScale;

    SDL_SetRenderScale(renderer, framebufferScale.x, framebufferScale.y);

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

    SDL_SetRenderScale(renderer, prevScaleX, prevScaleY);
}
//==============================================================================
