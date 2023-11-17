#include "map.hpp"
#include <stdint.h>

#include "map/tile.hpp"

namespace roguespace {
    Map::Map(ARC_FPoint *offset): offset(offset){
        arc::config->load("res/config/map.config");

        float scale = *arc::config->get<float>("global::scale");

        ARC_Spritesheet *sheet = arc::config->get<ARC_Spritesheet>("map::dungeonLayerSheet");
        ARC_Array *csv = arc::config->get<ARC_Array>("map::dungeonFloorCSV");
        sections.push_back(new world::Section(sheet, { 0, 0, 400, 400 }, {0, 0, 16.0, 16.0 }, csv, scale));

        csv = arc::config->get<ARC_Array>("map::dungeonWallCSV");
        sections.push_back(new world::Section(sheet, { 0, 0, 400, 400 }, {0, 0, 16.0, 16.0 }, csv, scale, true));
    }

    Map::~Map(){
    }

    void Map::update(){
    }

    void Map::render(){
        for(world::Section *&section : sections){
            section->render(offset);
        }
    }

    std::vector<world::Tile *> Map::tilesInArea(ARC_Rect area){
        std::vector<world::Tile *> foundTiles;

        for(world::Section *&section : sections){
            for(world::Tile *tile : section->tilesInArea(area)){
                foundTiles.push_back(tile);
            }
        }

        return foundTiles;
    }
}