#pragma once


#include <glm/vec2.hpp>
#include <vector>
#include <stack>


namespace Turtle {

    struct State {
        glm::vec2 position;
        float angle;
    };

    struct Segment {
        glm::vec2 start;
        glm::vec2 end;
    };

    enum class CoordSystem {
        DEFAULT,
        SCREEN
    };


    class Builder final {
        std::vector<Segment> segments;
        State state;
        float step;
        CoordSystem convertCoords;

        std::stack<State> stateStack;
        std::vector<Segment> segmentStack;

    public:
        Builder(glm::vec2 pos, float angle, float step);

        const std::vector<Segment>& getSegments() const noexcept;

        void forward(bool draw);
        void rotate(float angle);
        void restore();
        void save();

        void setConvertCoords(CoordSystem convert);

        void clear();
    };

}
