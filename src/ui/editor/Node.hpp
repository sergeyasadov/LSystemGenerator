#pragma once

#include <imgui.h>
#include <string_view>
#include <vector>

namespace Editor {

    struct Link {
        int id;
        int fromAttr;
        int toAttr;
    };

}
