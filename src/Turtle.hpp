#pragma once


#include <SDL3/SDL.h>
#include <vector>
#include <stack>


namespace Turtle {

    struct State {
        SDL_FPoint position;
        float angle;
    };

    struct Segment {
        SDL_FPoint start;
        SDL_FPoint end;
    };


    class Builder final {
        std::vector<Segment> segments;
        State state;
        float step;

        std::stack<State> stateStack;
        std::vector<Segment> segmentStack;

    public:
        Builder(SDL_FPoint pos, float angle, float step);

        const std::vector<Segment>& getSegments() const;

        void forward(bool draw);
        void rotate(float angle);
        void restore();
        void save();
    };

}
