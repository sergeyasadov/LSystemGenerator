#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <functional>
#include <SDL3/SDL.h>

#include "../Turtle.hpp"

namespace LSystem {

    using Value = char;
    using Axiom = std::string;
    using Action = std::string;
    using Iterations = std::uint32_t;
    using Data = std::string;
    using Command = std::function<void(Turtle::Builder&)>;


    enum class CommandType {
        FORWARD,
        ROTATE,
        SAVE,
        RESTORE
    };


    struct Variable final {
        Value value;
        Command command;
    };

    struct Constant final {
        Value value;
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

    static constexpr std::array type_cstr = {
        "Rule", "Variable", "Constant", "Axiom"
    };

    static constexpr std::array action_cstr = {
        "rotate", "forward"
    };


};
