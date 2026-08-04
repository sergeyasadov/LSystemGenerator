#include "Screen.hpp"

SDL_FPoint Screen::worldToScreen(SDL_FPoint point, Camera camera) {
    return {
        static_cast<float>((point.x - camera.position.x) / camera.zoom),
        static_cast<float>((point.y - camera.position.y) / camera.zoom)
    };
}

SDL_FPoint Screen::screenToWorld(const Camera& camera, float screenX, float screenY) {
    return {
        camera.position.x + screenX / camera.zoom,
        camera.position.y + screenY / camera.zoom
    };
}
