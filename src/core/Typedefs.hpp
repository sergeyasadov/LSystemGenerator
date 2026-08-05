#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

namespace LSystem {

    using Variable = char;
    using Constant = char;
    using Command = std::function<void()>;
    using Action = std::string;
    using Axiom = std::string;
    using Iterations = std::uint32_t;
    using Data = std::string;

    struct Rule final {
        Variable var;
        Action action;
        Command command;
    };

    using Rules = std::vector<Rule>;
    using Variables = std::vector<Variable>;
    using Constants = std::vector<Constant>;
};
