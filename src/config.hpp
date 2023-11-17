#pragma once

#include <archeus.hpp>

namespace tbyte {
    void initConfig();

    void StripAndReadNextType(ARC_Config *config, ARC_String **subString, const char *search, ARC_ConfigKeyRead ReadFn, void **value);

    uint8_t TextMap_Read(ARC_Config *config, ARC_String *string, void **value);
    uint8_t Tile_Read   (ARC_Config *config, ARC_String *string, void **value);

    void TextMap_Delete(ARC_Config *config, ARC_String *string, void *value);
    void Tile_Delete   (ARC_Config *config, ARC_String *string, void *value);
}