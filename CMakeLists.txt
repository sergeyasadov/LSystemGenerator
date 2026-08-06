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
  src/api/SDL.cpp
  src/api/SDL.hpp
  src/Application.cpp
  src/Application.hpp
  src/Screen.cpp
  src/Screen.hpp
  src/core/Interpreter.cpp
  src/core/Interpreter.hpp
  src/core/Compiler.cpp
  src/core/Compiler.hpp
  src/core/Typedefs.hpp
  src/core/Node.cpp
  src/core/Node.hpp
  src/Turtle.cpp
  src/Turtle.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
