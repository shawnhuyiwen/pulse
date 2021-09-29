#pragma once

#include <string>

class PulseBuildInformation {
    static std::string time();
    static std::string hash();
    static std::string version();
};

