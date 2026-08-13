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

    const float scaleChange = static_cast<float>(keys[SDL_SCANCODE_Z]) - static_cast<float>(keys[SDL_SCANCODE_X]);
    this->scale *= std::exp(scaleChange * 2.f * dt);
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
void Camera::setWindowSize(int width, int height) {
    this->windowSize.x = static_cast<float>(width);
    this->windowSize.y = static_cast<float>(height);
}

//==============================================================================
const glm::vec2& Camera::getOffset() const {
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
void Camera::setOffset(const glm::vec2& offset) {
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
bool Camera::isVisible(const glm::vec2& start, const glm::vec2& end) const {
    const float worldLeft =  -this->offset.x / this->scale;
    const float worldRight = (this->windowSize.x - this->offset.x) / this->scale;
    const float worldTop = -this->offset.y / this->scale;
    const float worldBottom = (this->windowSize.y - this->offset.y) / this->scale;

    const float minX = std::min(start.x, end.x);
    const float minY = std::min(start.y, end.y);
    const float maxX = std::max(start.x, end.x);
    const float maxY = std::max(start.y, end.y);

    return maxX <= worldLeft
        || minX >= worldRight
        || maxY <= worldTop
        || minY >= worldBottom;
}
//==============================================================================
glm::vec4 Camera::transform(const glm::vec2& start, const glm::vec2& end) const {
    return {
        start.x * this->scale + this->offset.x,
        start.y * this->scale + this->offset.y,
        end.x * this->scale + this->offset.x,
        end.y * this->scale + this->offset.y
    };
}
//==============================================================================
