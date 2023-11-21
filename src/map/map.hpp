#pragma once

#include <archeus.hpp>
#include <vector>
#include "types.hpp"

namespace tbyte {
    class Map {
    public:
        Map(ARC_FPoint *offset, char *group);
        ~Map();

        void update();
        void render();

        Tile *getTileById(uint32_t id);
        Tile *getTileByGridPos(uint32_t x, uint32_t y);
        Tile *getTileAt(ARC_FPoint pos);
        ARC_Point getTileGridPosById(uint32_t id);

        std::vector<Tile *> getTilesOfTypes(uint32_t types);
        // std::vector<world::Tile *> tilesInArea(ARC_Rect area);

        ARC_FPoint getTilePosCenter(Tile tile);
        ARC_FPoint getTilePosCenterByGridPos(ARC_Point pos);

        ARC_Rect getMapBounds();
        ARC_Rect getMapGridBounds();

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