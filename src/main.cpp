#include "Application.hpp"

#include <ranges>

int main(int, char**) {
    Application app;

    app.run();

    // LSystem lsys;
    // lsys.set_axiom("F");
    // lsys.set_rules("F", "F+F-F-F+F");
    // lsys.set_iters(4);
    // lsys.run();
    // std::string result = lsys.get_axiom();

    // printf("%s\n", result.c_str());

    return 0;
}
