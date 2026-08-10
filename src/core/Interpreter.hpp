#pragma once

#include "Typedefs.hpp"

#include "../ui/editor/Draft.hpp"


namespace LSystem {

class Interpreter {
    Rules rules;
    Variables variables;
    Constants constants;
    Axiom axiom;
    Iterations iterations;
    Data data {};

    public:
        Interpreter() = default;
        ~Interpreter() = default;

        void setAxiom(std::string_view axiom);
        void setRules(std::vector<Rule> rules);
        void setConstants(std::vector<Constant> constants);
        void setIterations(std::uint32_t iters);

        void pushRule(Rule rule);
        void pushConstant(Constant constant);

        Command makeCommand(const UI::Editor::Draft::Command& draft);

        const Rules& getRules() const;
        const Constants& getConstants() const;
        const Axiom& getAxiom() const;
        const Iterations& getIterations() const;
        const Data& getData() const;

        void nextIteration();
        void print();
        void run();

    private:
        bool isConstant(char c) const;
};

} // namespace LSystem
