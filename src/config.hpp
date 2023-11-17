#pragma once

#include <archeus.hpp>

namespace tbyte {
    void initConfig();

    void StripAndReadNextType(ARC_Config *config, ARC_String **subString, const char *search, ARC_ConfigKeyRead ReadFn, void **value);
}