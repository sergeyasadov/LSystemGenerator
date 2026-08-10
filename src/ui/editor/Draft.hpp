#pragma once

#include <cstdint>

#include "../../core/Typedefs.hpp"

namespace UI::Editor::Draft {

    struct Command final {
        LSystem::CommandType type {LSystem::CommandType::FORWARD};
        float argument{};
        bool draw{true};
    };

    struct Rule final {
        std::uint64_t id;
        std::string symbol{"A"};
        std::string replacement;
        Command command;
    };

    struct Constant final {
        std::uint64_t id;
        std::string name;
        Command command;
    };

}
