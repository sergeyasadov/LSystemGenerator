#include "Logger.hpp"
#include "../Defines.hpp"

#include <array>

#include <quill/Logger.h>
#include <quill/Frontend.h>
#include <quill/Backend.h>
#include <quill/sinks/ConsoleSink.h>

void Logger::init() {
    static constexpr std::array names = {
        Defines::Logger::Names::APPLICATION,
        Defines::Logger::Names::SDL,
        Defines::Logger::Names::IMGUI_LAYER,
        Defines::Logger::Names::LSYSTEM_COMPILER,
        Defines::Logger::Names::CONFIG
    };

    quill::Backend::start();
    auto consoleSink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>(Defines::Logger::CONSOLE_SINK);

    for(const auto& name : names) {
        quill::Frontend::create_or_get_logger(name, consoleSink);
    }

}
