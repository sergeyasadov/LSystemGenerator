#pragma once

#include "Interpreter.hpp"
#include "../Turtle.hpp"
#include "../api/SDL.hpp"

#include <vector>


namespace LSystem {

    class Node {
        std::vector<LSystem::Interpreter> interpreters;

    public:
        void add(LSystem::Interpreter interpreter);

        void execute(Turtle::Builder& builder) const;
    };

}
