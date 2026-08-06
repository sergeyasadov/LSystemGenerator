#pragma once

#include <string_view>
#include <SDL3/SDL.h>

#include "../Turtle.hpp"
#include "Typedefs.hpp"
#include "Interpreter.hpp"


namespace LSystem {

class Compiler {
    std::vector<SDL_FPoint> points;

public:
    Compiler();

    void compile(const LSystem::Interpreter& interpreter, Turtle::Builder& turtle);

    const std::vector<SDL_FPoint>& getPoints() const;

    void print() const;
};

}
