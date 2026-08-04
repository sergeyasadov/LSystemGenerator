#ifndef WORLD_LINE_HPP
#define WORLD_LINE_HPP

#include <SDL3/SDL_render.h>


struct Camera {
    SDL_FPoint position;
    float zoom;
};


namespace Screen {
    SDL_FPoint worldToScreen(SDL_FPoint point, Camera camera);

    SDL_FPoint screenToWorld(const Camera& camera, float screenX, float screenY);
}




#endif
