#pragma once

#include <archeus.hpp>
#include "types.hpp"

namespace tbyte {
    class Map {
    public:
        Map(ARC_FPoint *offset, char *group);
        ~Map();

        void update();
        void render();

        // std::vector<world::Tile *> tilesInArea(ARC_Rect area);

    private:
        void initTiles(char *group);

        ARC_Spritesheet *sheet;

        Tile       **tiles;
        ARC_UPoint   tilesSize;
        ARC_Rect     tileBounds;

        double scale;

        ARC_FPoint *offset;
    };
}