#pragma once

#include "Draft.hpp"
#include "../../core/Interpreter.hpp"

#include <vector>
#include <expected>

namespace UI::Editor {

    struct SaveData {
        std::string axiom;
        int iters;
        std::vector<Draft::Rule> rules;
        std::vector<Draft::Constant> constants;
        std::vector<Draft::Variable> variables;
    };

    class Pipeline final {
        std::string axiom{"A"};
        int iters{};

        std::uint64_t nextId{1};
        std::string error;

        std::vector<Draft::Rule> rules;
        std::vector<Draft::Constant> constants;
        std::vector<Draft::Variable> variables;

    public:

        SaveData getData() const;
        void setData(const SaveData& data);

        bool draw();

        [[nodiscard]]
        std::expected<LSystem::Interpreter, std::string>
        build();

    private:
        void removeRule(std::uint64_t id);
        void removeConstant(std::uint64_t id);
        void removeVariable(std::uint64_t id);
        void drawRules();
        void drawConstants();
        void drawVariables();
    };



}
