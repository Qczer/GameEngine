#pragma once

#include <string>

namespace GameEngine {

    class FileDialogs {
    public:
        static std::optional<std::string> OpenFile(const char* filter);
        static std::optional<std::string> SaveFile(const char* filter);
    };

}
