#include "Interpreter.hpp"

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <cstdio>


using namespace LSystem;

void Interpreter::setAxiom(std::string_view axiom) {
    if(axiom.empty()) {
        throw std::invalid_argument("Axiom cannot be empty");
    }
    this->axiom = std::move(axiom);
    this->data = this->axiom;
}
//==============================================================================
void Interpreter::setRules(std::vector<Rule> rules) {
    for(const auto& rule : rules) {
        if(this->isConstant(rule.var.value)) {
            throw std::invalid_argument("Variable is a constant");
        }

        const auto duplicate = std::ranges::find(
            this->rules,
            rule.var.value,
            [](const Rule& rule) { return rule.var.value; }
        );
        if(duplicate != this->rules.end()) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Duplicate variable: '%c'", rule.var.value);
            throw std::invalid_argument("Duplicate variable");
        }

    }

    this->rules = std::move(rules);
}
//==============================================================================
void Interpreter::setConstants(std::vector<Constant> constants) {
    if(constants.empty()) {
        throw std::invalid_argument("Constants cannot be empty");
    }
    for(const auto& c : constants) {
        if(this->isConstant(c.value)) {
            throw std::invalid_argument("Constants contain a constant");
        }
    }
    this->constants = constants;
}
//==============================================================================
void Interpreter::setIterations(std::uint32_t iters) {
    if(iters == 0) {
        throw std::invalid_argument("Iterations cannot be 0");
    }
    this->iterations = iters;
}
//==============================================================================
const Rules& Interpreter::getRules() const {
    return this->rules;
}
//==============================================================================
const Axiom& Interpreter::getAxiom() const {
    return this->axiom;
}
//==============================================================================
const Iterations& Interpreter::getIterations() const {
    return this->iterations;
}
//==============================================================================
const Constants& Interpreter::getConstants() const {
    return this->constants;
}
//==============================================================================
const Data& Interpreter::getData() const {
    return this->data;
}
//==============================================================================
void Interpreter::nextIteration() {
    Data next;
    next.reserve(this->data.size());

    for(const auto& symbol : this->data) {
        const auto rule = std::ranges::find(
            this->rules,
            symbol,
            [](const Rule& rule) { return rule.var.value; }
        );
        if(rule != this->rules.end()) {
            next += rule->replacement;
        }
        else {
            next.push_back(symbol);
        }
    }

    this->data = std::move(next);

    // for(const auto& [var, action, _] : this->rules) {
    //     auto splitView = this->axiom | std::views::split(var);
    //     auto joinedView = splitView | std::views::join_with(action);
    //     this->axiom = satd::string(std::from_range, joinedView);
}
//==============================================================================
void Interpreter::pushRule(Rule rule) {
    this->rules.push_back(rule);
}
//==============================================================================
void Interpreter::pushConstant(Constant constant) {
    this->constants.push_back(constant);
}
//==============================================================================
void Interpreter::run() {
    for(auto i = 0; i < this->iterations; i++) {
        this->nextIteration();
    }
}
//==============================================================================
void Interpreter::print() {
    std::printf("%s\n", this->data.c_str());
}
//==============================================================================
bool Interpreter::isConstant(char c) const {
    return std::ranges::any_of(this->constants, [c](const Constant& constant) {
        return constant.value == c;
    });
}
//==============================================================================
LSystem::Command Interpreter::makeCommand(const UI::Editor::Draft::Command& draft) {
    switch(draft.type) {
        case CommandType::FORWARD:
            return [draft](Turtle::Builder& builder) { builder.forward(draft.draw); };
        case CommandType::ROTATE:
            return [draft](Turtle::Builder& builder) { builder.rotate(draft.argument); };
        case CommandType::SAVE:
            return [](Turtle::Builder& builder) { builder.save(); };
        case CommandType::RESTORE:
            return [](Turtle::Builder& builder) { builder.restore(); };
        default:
            return {};
    }
}
//==============================================================================
