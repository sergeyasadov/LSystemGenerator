#pragma once

#include "Typedefs.hpp"


namespace LSystem {

class Interpreter final {
    Rules rules;
    Variables variables;
    Constants constants;
    Axiom axiom;
    Iterations iterations;
    Data data {};

    public:
        Interpreter() = default;
        ~Interpreter() = default;

        void setAxiom(std::string axiom);
        void setRule(std::vector<Rule> rules);
        void setConstants(std::vector<Constant> constants);
        void setIterations(std::uint32_t iters);

        Rules getRules() const;
        Constants getConstants() const;
        Axiom getAxiom() const;
        Iterations getIterations() const;
        const Data& getData() const;

        void nextIteration();
        void print();
        void run();

    private:
        bool isConstant(char c) const;
};

} // namespace LSystem
