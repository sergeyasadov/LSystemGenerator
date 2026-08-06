#include "Camera.hpp"

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
const SDL_FPoint& Camera::getOffset() const
{
    return offset;
}//==============================================================================
float Camera::getOffsetX() const
{
    return offset.x;
}
//==============================================================================
float Camera::getOffsetY() const
{
    return offset.y;
}
//==============================================================================
float Camera::getScale() const
{
    return scale;
}
//==============================================================================
float Camera::getFactor() const
{
    return factor;
}
//==============================================================================
void Camera::setOffset(const SDL_FPoint& offset)
{
    this->offset = offset;
}
//==============================================================================
void Camera::setScale(float scale)
{
    this->scale = scale;
}
//==============================================================================
void Camera::setFactor(float factor)
{
    this->factor = factor;
}
//==============================================================================
void Camera::handleEvent(const SDL_Event& event)
{
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
