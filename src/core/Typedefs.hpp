#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <SDL3/SDL.h>

#include "../Turtle.hpp"

namespace LSystem {

    using Action = std::string;
    using Axiom = std::string;
    using Iterations = std::uint32_t;
    using Data = std::string;
    using Command = std::function<void(Turtle::Builder&)>;

    struct Variable final {
        char value;
        Command command;
    };

    struct Constant final {
        char value;
        Command command;
    };

    struct Rule final {
        Variable var;
        Action action;
    };

    using Rules = std::vector<Rule>;
    using Variables = std::vector<Variable>;
    using Constants = std::vector<Constant>;
    using Commands = std::unordered_map<char, Command>;
};
