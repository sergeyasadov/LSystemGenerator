#pragma once

#include <SDL3/SDL.h>
#include <glm/vec4.hpp>

class Camera {
    SDL_FPoint offset;
    float scale;
    float factor;
public:
    Camera();
    Camera(float scale, float factor);

    void update(float dt);

    const SDL_FPoint& getOffset() const;
    float getOffsetX() const;
    float getOffsetY() const;
    float getScale() const;
    float getFactor() const;

    void setOffset(const SDL_FPoint& offset);
    void setScale(float scale);
    void setFactor(float factor);

    void handleEvent(const SDL_Event& event);

    glm::vec4 transform(const glm::vec4& position) const;
};
