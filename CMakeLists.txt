cmake_minimum_required(VERSION 3.15)
project(lsys LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#===============================================================================
include(FetchContent)
#===============================================================================
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.12
    GIT_PROGRESS TRUE
)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)
#===============================================================================
FetchContent_Declare(
    imgui_source
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.9-docking
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(imgui_source)

set(IMGUI_DIR "${imgui_source_SOURCE_DIR}")

add_library(imgui STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"

    "${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp"

    "${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp"
)

add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui
    PUBLIC
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
)

target_link_libraries(imgui
    PUBLIC
        SDL3::SDL3
)

#===============================================================================
FetchContent_Declare(
    imnodes
    GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
    GIT_TAG master
    GIT_PROGRESS TRUE
)

set(
    IMNODES_IMGUI_TARGET_NAME
    imgui::imgui
    CACHE STRING ""
    FORCE
)

set(
    IMNODES_EXAMPLES
    OFF
    CACHE BOOL ""
    FORCE
)
FetchContent_MakeAvailable(imnodes)
#===============================================================================
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glm)
#===============================================================================
FetchContent_Declare(
    quill
    GIT_REPOSITORY https://github.com/odygrd/quill.git
    GIT_TAG v12.1.0
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(quill)

set(QUILL_DISABLE_FILE_INFO ON)
set(QUILL_COMPILE_WITH_HEADER_ONLY OFF CACHE INTERNAL "")
#===============================================================================
FetchContent_Declare(
    glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG v8.0.0
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glaze)
#===============================================================================
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
    src/core/Pipeline.cpp
    src/core/Pipeline.hpp

    src/Turtle.cpp
    src/Turtle.hpp

    src/Camera.cpp
    src/Camera.hpp

    src/ui/ImGuiLayer.cpp
    src/ui/ImGuiLayer.hpp
    src/ui/editor/Node.cpp
    src/ui/editor/Node.hpp
    src/ui/editor/Pipeline.cpp
    src/ui/editor/Pipeline.hpp

    src/utils/Logger.cpp
    src/utils/Logger.hpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3-static
        glm::glm
        imgui::imgui
        imnodes
        quill::quill
        glaze::glaze
)
#===============================================================================
