#include "Node.hpp"
#include "Compiler.hpp"

//==============================================================================
void LSystem::Node::add(LSystem::Interpreter interpreter) {
    this->interpreters.push_back(std::move(interpreter));
}
//==============================================================================
void LSystem::Node::execute(Turtle::Builder& builder) const {
    for(const auto& interpreter : this->interpreters) {
        LSystem::Compiler compiler;
        compiler.compile(interpreter, builder);
    }
}
//==============================================================================
