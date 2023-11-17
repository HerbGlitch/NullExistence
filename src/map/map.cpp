#include "map.hpp"
#include <stdint.h>
#include "types.hpp"

namespace tbyte {
    Map::Map(ARC_FPoint *offset, char *group): offset(offset){
        tileBounds = *arc::config->get<ARC_Rect>(group, "tileBounds");

        initTiles(group);

        scale = 12.0f;
    }

    Map::~Map(){
        for(int32_t y = 0; y < tilesSize.y; y++){
            delete [] tiles[y];
        }

        delete [] tiles;
    }

    void Map::update(){
    }

    void Map::render(){
        ARC_Rect bounds = {
            0,
            0,
            (int32_t)((double)tileBounds.w * scale),
            (int32_t)((double)tileBounds.h * scale)
        };

        for(uint32_t y = 0; y < tilesSize.y; y++){
            for(uint32_t x = 0; x < tilesSize.x; x++){
                Tile tile = tiles[y][x];

                bounds.x = (int32_t)((x * tileBounds.w * scale) + (double)offset->x),
                bounds.y = (int32_t)((y * tileBounds.h * scale) + (double)offset->y),

                tileBounds.x = tileBounds.w * tile.pos.x;
                tileBounds.y = tileBounds.h * tile.pos.y;

                ARC_Spritesheet_RenderArea(sheet, &tileBounds, arc::data->renderer, &bounds);
            }
        }
    }

    void Map::initTiles(char *group){
        sheet = arc::config->get<ARC_Spritesheet>(group, "sheet");

        TextMap *map = arc::config->get<TextMap>(group, "map");
        tilesSize = map->size;

        tiles = new Tile *[tilesSize.y];
        for(uint32_t y = 0; y < tilesSize.y; y++){
            tiles[y] = new Tile[tilesSize.x];

            for(uint32_t x = 0; x < tilesSize.x; x++){
                char name[3];
                name[0] = 't';
                name[1] = map->tiles[y][x];
                name[2] = '\0';

                tiles[y][x] = *arc::config->get<Tile>(group, name);
            }
        }
    }
}