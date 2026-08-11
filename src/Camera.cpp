#include "Camera.hpp"
#include <algorithm>
#include <cmath>

//==============================================================================
Camera::Camera()
    :   offset({0, 0}),
        scale(1.0f),
        factor(1.f)
{
}
//==============================================================================
Camera::Camera(float scale, float factor)
    :   offset({0, 0}),
        scale(scale),
        factor(factor)
{
}
//==============================================================================
void Camera::update(float dt) {
    const auto keys = SDL_GetKeyboardState(nullptr);

    this->scale += (keys[SDL_SCANCODE_Z] - keys[SDL_SCANCODE_X]) * 0.2f;
    this->scale = std::clamp(this->scale, 0.01f, 200.f);

    float dx = static_cast<float>(keys[SDL_SCANCODE_A] - keys[SDL_SCANCODE_D]);
    float dy = static_cast<float>(keys[SDL_SCANCODE_W] - keys[SDL_SCANCODE_S]);

    const float length = std::hypot(dx, dy);

    if(length > 0.f) {
        dx /= length;
        dy /= length;
    }

    const bool fast = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
    const float multiplier = fast ? 5.f : 1.f;

    const float dist = 700.f * multiplier * dt;

    this->offset.x += dx * dist;
    this->offset.y += dy * dist;
}
//==============================================================================
const SDL_FPoint& Camera::getOffset() const {
    return offset;
}
//==============================================================================
float Camera::getOffsetX() const {
    return offset.x;
}
//==============================================================================
float Camera::getOffsetY() const {
    return offset.y;
}
//==============================================================================
float Camera::getScale() const {
    return scale;
}
//==============================================================================
float Camera::getFactor() const {
    return factor;
}
//==============================================================================
void Camera::setOffset(const SDL_FPoint& offset) {
    this->offset = offset;
}
//==============================================================================
void Camera::setScale(float scale) {
    this->scale = scale;
}
//==============================================================================
void Camera::setFactor(float factor) {
    this->factor = factor;
}
//==============================================================================
void Camera::handleEvent(const SDL_Event& event) {
    switch(event.key.scancode) {
        case SDL_SCANCODE_W:
            this->offset.y += this->factor;
            break;
        case SDL_SCANCODE_S:
            this->offset.y -= this->factor;
            break;
        case SDL_SCANCODE_A:
            this->offset.x += this->factor;
            break;
        case SDL_SCANCODE_D:
            this->offset.x -= this->factor;
            break;
        case SDL_SCANCODE_Z:
            this->scale *= 0.5f;
            break;
        case SDL_SCANCODE_X:
            this->scale *= 1.5f;
            break;
        default: { break; }
    }
}
//==============================================================================
glm::vec4 Camera::transform(const glm::vec4& position) const {
    return {
        position.x * this->scale + this->offset.x,
        position.y * this->scale + this->offset.y,
        position.z * this->scale + this->offset.x,
        position.w * this->scale + this->offset.y
    };
}
//==============================================================================
