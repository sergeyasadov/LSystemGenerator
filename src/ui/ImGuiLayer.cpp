#include "ImGuiLayer.hpp"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

//==============================================================================
UI::ImGuiLayer::ImGuiLayer() {
    IMGUI_CHECKVERSION();

    this->context = ImGui::CreateContext();
    this->io = &ImGui::GetIO();
    this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    this->io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
}

void UI::ImGuiLayer::init(SDL_Window* window, SDL_Renderer* renderer) {
    if(!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        throw std::runtime_error("Failed to initialize ImGui for SDL renderer");
    }
    if(!ImGui_ImplSDLRenderer3_Init(renderer)) {
        ImGui_ImplSDL3_Shutdown();
        throw std::runtime_error("Failed to initialize ImGui for SDL renderer");
    }
}

//==============================================================================
UI::ImGuiLayer::~ImGuiLayer() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext(context);
}
//==============================================================================
void UI::ImGuiLayer::processEvent(SDL_Event& event) {
    ImGui_ImplSDL3_ProcessEvent(&event);
}

//==============================================================================
void UI::ImGuiLayer::render(SDL_Renderer* renderer, std::function<void()>& f) {
    ImGui::NewFrame();
    f();
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
}
//==============================================================================
