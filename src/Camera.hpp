#pragma once

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


class Camera {
    glm::vec2 offset;
    glm::vec2 windowSize;
    float scale;
    float factor;
public:
    Camera();
    Camera(float scale, float factor);

    void update(float dt);

    const glm::vec2& getOffset() const;
    float getOffsetX() const;
    float getOffsetY() const;
    float getScale() const;
    float getFactor() const;

    bool isVisible(const glm::vec2& start, const glm::vec2& end) const;

    void setOffset(const glm::vec2& offset);
    void setScale(float scale);
    void setFactor(float factor);
    void setWindowSize(int width, int height);

    void handleEvent(const SDL_Event& event);

    glm::vec4 transform(const glm::vec2& start, const glm::vec2& end) const;
};
