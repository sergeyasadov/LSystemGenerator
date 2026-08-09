#include "Compiler.hpp"
#include "Typedefs.hpp"

#include <unordered_map>

LSystem::Compiler::Compiler()
{
}
//==============================================================================
void LSystem::Compiler::compile(const LSystem::Interpreter& interpreter, Turtle::Builder& turtle
) {
    std::unordered_map<LSystem::Value, LSystem::Command> commandMap;
    for(const auto& [var, _] : interpreter.getRules()) {
        if(var.command == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Rule %c has no command", var.value);
            continue;
        }

        commandMap.insert_or_assign(var.value, var.command);
        SDL_Log("Rule: %c", var.value);
    }

    for(const auto& [constVal, command] : interpreter.getConstants()) {
        if(command == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Constant %c has no command", constVal);
            continue;
        }

        commandMap.insert_or_assign(constVal, command);
        SDL_Log("Constant: %c", constVal);
    }

    for(const auto& sym : interpreter.getData()) {
        const auto& commandIt = commandMap.find(sym);

        if(commandIt == commandMap.end()) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unknown symbol: %c", sym);
            continue;
        }

        const Command& command = commandIt->second;
        if(command == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Command for symbol %c is null", sym);
            continue;
        }

        std::invoke(command, turtle);
    }
}
//==============================================================================
const std::vector<SDL_FPoint>& LSystem::Compiler::getPoints() const {
    return points;
}
//==============================================================================
void LSystem::Compiler::print() const {
    for(const auto& [x, y] : points) {
        SDL_Log("(%f, %f)", x, y);
    }
}
//==============================================================================
