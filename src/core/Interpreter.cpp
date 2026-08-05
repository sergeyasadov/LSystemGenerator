#include "Interpreter.hpp"

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <cstdio>

using namespace LSystem;

void Interpreter::setAxiom(std::string axiom) {
    if(axiom.empty()) {
        throw std::invalid_argument("Axiom cannot be empty");
    }
    this->axiom = std::move(axiom);
}

void Interpreter::setRule(std::vector<Rule> rules) {
    for(const auto& rule : rules) {
        if(this->isConstant(rule.var)) {
            throw std::invalid_argument("Variable is a constant");
        }

        const auto duplicate = std::ranges::find(
            this->rules,
            rule.var,
            [](const Rule& rule) { return rule.var; }
        );
        if(duplicate != this->rules.end()) {
            throw std::invalid_argument("Duplicate variable");
        }

        this->rules = std::move(rules);
    }
}

void Interpreter::setConstants(std::vector<Constant> constants) {
    if(constants.empty()) {
        throw std::invalid_argument("Constants cannot be empty");
    }
    for(const auto& c : constants) {
        if(this->isConstant(c)) {
            throw std::invalid_argument("Constants contain a constant");
        }
    }
    this->constants = constants;
}

void Interpreter::setIterations(std::uint32_t iters) {
    if(iters == 0) {
        throw std::invalid_argument("Iterations cannot be 0");
    }
    this->iterations = iters;
}

Rules Interpreter::getRules() const {
    return this->rules;
}
Axiom Interpreter::getAxiom() const {
    return this->axiom;
}

Iterations Interpreter::getIterations() const {
    return this->iterations;
}

Constants Interpreter::getConstants() const {
    return this->constants;
}

const Data& Interpreter::getData() const {
    return this->data;
}


void Interpreter::nextIteration() {
    Data next;
    next.reserve(this->axiom.size());

    for(const auto& symbol : this->axiom) {
        const auto rule = std::ranges::find(
            this->rules,
            symbol,
            [](const Rule& rule) { return rule.var; }
        );
        if(rule != this->rules.end()) {
            next += rule->action;
        } else {
            next.push_back(symbol);
        }
    }

    this->data = std::move(next);


    // for(const auto& [var, action, _] : this->rules) {
    //     auto splitView = this->axiom | std::views::split(var);
    //     auto joinedView = splitView | std::views::join_with(action);
    //     this->axiom = satd::string(std::from_range, joinedView);
    // }
}

void Interpreter::run() {
    for(auto i = 0; i < this->iterations; i++) {
        this->nextIteration();
    }
}

void Interpreter::print() {
    std::printf("%s\n", this->data.c_str());
}


bool Interpreter::isConstant(char c) const {
    return std::ranges::any_of(this->constants, [c](const Constant& constant) {
        return constant == c;
    });
}
