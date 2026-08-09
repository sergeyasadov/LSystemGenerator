#pragma once

#include "Typedefs.hpp"


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
        void setRule(std::vector<Rule> rules);
        void setConstants(std::vector<Constant> constants);
        void setIterations(std::uint32_t iters);

        Command makeCommand();

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
