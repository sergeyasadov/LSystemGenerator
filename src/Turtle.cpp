#include "Turtle.hpp"

#include <cmath>
#include <numbers>

static constexpr auto TURN_ANGLE = std::numbers::pi / 180.0F;


Turtle::Builder::Builder(SDL_FPoint pos, float angle, float step)
    :   state(pos, angle), step(step)
{}
//==============================================================================
void Turtle::Builder::forward(bool draw)
{
    const SDL_FPoint begin = this->state.position;

    this->state.position.x += this->step * std::cosf(this->state.angle * TURN_ANGLE);
    this->state.position.y += this->step * std::sinf(this->state.angle * TURN_ANGLE);

    if (draw)
    {
        this->segments.emplace_back(begin, this->state.position);
    }

    // SDL_Log("forward: begin = (%f, %f), end = (%f, %f)", begin.x, begin.y, this->state.position.x, this->state.position.y);
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
const std::vector<Turtle::Segment>& Turtle::Builder::getSegments() const {
    return this->segments;
}
//==============================================================================
