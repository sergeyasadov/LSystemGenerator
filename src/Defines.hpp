#pragma once

namespace Defines {

    static inline constexpr auto DATA_FILE = "lsystemdata.json";

    namespace Window {
        inline constexpr auto TITLE = "LSystem";
        inline constexpr auto WIDTH = 800;
        inline constexpr auto HEIGHT = 600;
    }

    namespace Logger {
        static inline constexpr auto CONSOLE_SINK = "ConsoleMainSink";

        namespace Names {
            static inline constexpr auto APPLICATION = "Application";
            static inline constexpr auto SDL = "SDL";
            static inline constexpr auto IMGUI_LAYER = "ImGuiLayer";
            static inline constexpr auto LSYSTEM_COMPILER = "LSystemCompiler";
            static inline constexpr auto CONFIG = "Config";
        }
    }
}
