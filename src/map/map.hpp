#pragma once

#include <archeus.hpp>
#include <vector>
#include "section.hpp"

namespace roguespace {
    class Map {
    public:
        Map(ARC_FPoint *offset);
        ~Map();

        void update();
        void render();

        std::vector<world::Tile *> tilesInArea(ARC_Rect area);

    private:
        std::vector<world::Section *> sections; 

        double scale;

        ARC_FPoint *offset;
    };
}