#include "ImGuiLayer.hpp"

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

//==============================================================================
UI::ImGuiLayer::ImGuiLayer() {
    IMGUI_CHECKVERSION();

    this->context = ImGui::CreateContext();

    this->nodesContext = ImNodes::CreateContext();

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

    if(!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        throw std::runtime_error("Failed to initialize ImGui for SDL renderer");
    }
    this->platformInit = true;
    if(!ImGui_ImplSDLRenderer3_Init(renderer)) {
        ImGui_ImplSDL3_Shutdown();
        throw std::runtime_error("Failed to initialize ImGui for SDL renderer");
    }
    this->renderInit = true;
}

//==============================================================================
UI::ImGuiLayer::~ImGuiLayer() {
    if(this->platformInit) {
        ImGui_ImplSDLRenderer3_Shutdown();
    }
    if(this->renderInit) {
        ImGui_ImplSDL3_Shutdown();
    }

    if(this->nodesContext != nullptr) {
        ImNodes::DestroyContext(this->nodesContext);
    }

    if(this->context != nullptr) {
        ImGui::DestroyContext(this->context);
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
