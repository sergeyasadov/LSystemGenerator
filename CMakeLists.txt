cmake_minimum_required(VERSION 3.15)
project(lsys)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.12
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.92.9-docking
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.h
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.h
)


add_executable(${PROJECT_NAME}
  src/main.cpp
  src/api/SDL.cpp
  src/api/SDL.hpp
  src/Application.cpp
  src/Application.hpp
  src/core/Interpreter.cpp
  src/core/Interpreter.hpp
  src/core/Compiler.cpp
  src/core/Compiler.hpp
  src/core/Typedefs.hpp
  src/core/Node.cpp
  src/core/Node.hpp
  src/Turtle.cpp
  src/Turtle.hpp
  src/Camera.cpp
  src/Camera.hpp
  src/ui/ImGuiLayer.cpp
  src/ui/ImGuiLayer.hpp
  ${IMGUI_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
