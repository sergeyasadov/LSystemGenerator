#ifndef LSYSTEM_HPP
#define LSYSTEM_HPP

#include <unordered_map>
#include <string>
#include <vector>


class LSystem final {
    std::unordered_map<std::string, std::string> rules;
    std::string axiom;
    std::vector<char> variables;
    std::vector<char> constants;
    std::uint32_t iters;

public:
    LSystem();

    void set_rules(std::string_view key, std::string_view value);
    void set_axiom(std::string_view axiom);
    void set_iters(std::uint32_t iters);
    void set_constants(std::vector<char> constants);
    void set_variables(std::vector<char> variables);

    std::uint32_t get_iters() const;
    std::string get_axiom() const;
    std::vector<char> get_vars() const;
    std::vector<char> get_const() const;

    void next_iter();
    void print();
    void run();
};

#endif
