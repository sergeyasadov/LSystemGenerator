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


add_executable(${PROJECT_NAME}
  src/main.cpp
  src/LSystem.cpp
  src/LSystem.hpp
  src/api/SDL.cpp
  src/api/SDL.hpp
  src/Application.cpp
  src/Application.hpp
  src/Screen.cpp
  src/Screen.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
