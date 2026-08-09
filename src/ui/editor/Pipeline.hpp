#pragma once

#include "Draft.hpp"
#include "../../core/Interpreter.hpp"

#include <vector>
#include <expected>

namespace UI::Editor {

    class Pipeline final {
        std::string axiom{"A"};
        int iters{};

        std::uint64_t nextId{1};
        std::string error;

        std::vector<Draft::Rule> rules;
        std::vector<Draft::Constant> constants;

    public:
        bool draw();

        [[nodiscard]]
        std::expected<LSystem::Interpreter, std::string>
        build() const;

    private:
        void removeRule(std::uint64_t id);
        void removeConstant(std::uint64_t id);
        void drawRules();
        void drawConstants();
    };



}
