#include "Compiler.hpp"
#include "Typedefs.hpp"
#include "../Defines.hpp"

#include <unordered_map>

#include <quill/Logger.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>

LSystem::Compiler::Compiler()
{
}
//==============================================================================
void LSystem::Compiler::compile(const LSystem::Interpreter& interpreter, Turtle::Builder& turtle
) {
    auto logger = quill::Frontend::get_logger(Defines::Logger::Names::LSYSTEM_COMPILER);

    std::unordered_map<LSystem::Value, LSystem::Command> commandMap;
    for(const auto& [var, _] : interpreter.getRules()) {
        if(var.command == nullptr) {
            LOG_WARNING(logger, "Rule {} has no command", var.value);
            continue;
        }

        commandMap.insert_or_assign(var.value, var.command);
        LOG_INFO(logger, "Rule: {}", var.value);
    }

    for(const auto& [variableValue, command] : interpreter.getVariables()) {
        if(command == nullptr) {
            LOG_WARNING(logger, "Variable {} has no command", variableValue);
            continue;
        }

        commandMap.insert_or_assign(variableValue, command);
        LOG_INFO(logger, "Variable: {}", variableValue);
    }

    for(const auto& [constVal, command] : interpreter.getConstants()) {
        if(command == nullptr) {
            LOG_WARNING(logger, "Constant {} has no command", constVal);
            continue;
        }

        commandMap.insert_or_assign(constVal, command);
        LOG_INFO(logger, "Constant: {}", constVal);
    }

    for(const auto& sym : interpreter.getData()) {
        const auto& commandIt = commandMap.find(sym);

        if(commandIt == commandMap.end()) {
            LOG_WARNING(logger, "Unknown symbol: {}", sym);
            continue;
        }

        const Command& command = commandIt->second;
        if(command == nullptr) {
            LOG_ERROR(logger, "Command for symbol {} is null", sym);
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
