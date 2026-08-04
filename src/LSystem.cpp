#include <algorithm>
#include <string_view>
#include <ranges>


#include "LSystem.hpp"


LSystem::LSystem() : rules({}), axiom(""), iters(1) {}

void LSystem::set_axiom(std::string_view axiom) {
    this->axiom = std::string(axiom);
}

void LSystem::set_rules(std::string_view key, std::string_view value) {
    rules[std::string(key)] = std::string(value);
}

void LSystem::set_iters(std::uint32_t iters) {
    this->iters = iters;
}

void LSystem::set_constants(std::vector<char> constants) {
    this->constants = constants;
}

void LSystem::set_variables(std::vector<char> variables) {
    this->variables = variables;
}

std::vector<char> LSystem::get_vars() const {
    return this->variables;
}

std::vector<char> LSystem::get_const() const {
    return this->constants;
}

std::string LSystem::get_axiom() const {
    return this->axiom;
}

std::uint32_t LSystem::get_iters() const {
    return this->iters;
}

void LSystem::next_iter() {
    for(auto& el : this->rules) {
        auto splitView = this->axiom | std::views::split(el.first.front());
        auto joinedView = splitView | std::views::join_with(el.second);
        this->axiom = std::string(std::from_range, joinedView);
    }
}

void LSystem::run() {
    for(auto i = 0; i < this->iters; i++) {
        this->next_iter();
    }
}

void LSystem::print() {
    std::printf("%s\n", this->axiom.c_str());
}
