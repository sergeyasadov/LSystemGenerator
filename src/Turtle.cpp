#include "Turtle.hpp"

#include <cmath>
#include <numbers>


static constexpr auto TURN_ANGLE = std::numbers::pi / 180.0F;


Turtle::Builder::Builder(glm::vec2 pos, float angle, float step)
    :   state(pos, angle), step(step), convertCoords(CoordSystem::SCREEN)
{}
//==============================================================================
void Turtle::Builder::forward(bool draw) {
    const glm::vec2 begin = this->state.position;

    this->state.position.x += this->step * std::cosf(this->state.angle * TURN_ANGLE);
    this->state.position.y += this->step * std::sinf(this->state.angle * TURN_ANGLE);

    if(draw)
    {
        glm::vec2 renderBegin = begin;
        glm::vec2 renderEnd = this->state.position;
        if(this->convertCoords == CoordSystem::DEFAULT) {
            renderBegin = glm::vec2(begin.y, -begin.x);
            renderEnd = glm::vec2(this->state.position.y, -this->state.position.x);
        }
        this->segments.emplace_back(renderBegin, renderEnd);
    }
}
//==============================================================================
void Turtle::Builder::rotate(float angle) {
    this->state.angle += angle;
}
//==============================================================================
void Turtle::Builder::restore() {
    if(this->stateStack.empty()) {
        throw std::runtime_error("State stack is empty");
    }

    this->state = this->stateStack.top();
    this->stateStack.pop();
}
//==============================================================================
void Turtle::Builder::save() {
    this->stateStack.push(this->state);
}
//==============================================================================
const std::vector<Turtle::Segment>& Turtle::Builder::getSegments() const noexcept {
    return this->segments;
}
//==============================================================================
void Turtle::Builder::clear() {
    this->segments.clear();
    this->segmentStack.clear();
    this->stateStack = std::stack<State>();
    this->state.position = {0.0f, 0.0f};
    this->state.angle = 0.0f;
}
//==============================================================================
void Turtle::Builder::setConvertCoords(CoordSystem convert) {
    this->convertCoords = convert;
}
//==============================================================================
