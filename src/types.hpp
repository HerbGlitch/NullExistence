#pragma once
#include <archeus.hpp>
#include <string.h>

namespace tbyte {
    typedef struct TextMap {
        ARC_UPoint   size;
        char       **tiles;
    } TextMap;

    enum class TileType {
        COLLIDEABLE  = 0x01,
        PLAYER_SPAWN = 0x02,
        PLAYER_EXIT  = 0x04
    };

    typedef struct Tile {
        uint32_t  id;
    
        ARC_Point pos;

        //using a uint so types can be added as flags
        uint32_t  types;
    } Tile;
}
