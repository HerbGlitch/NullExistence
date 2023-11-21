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

    Tile *Map::getTileById(uint32_t id){
        return tiles[(int32_t)id % (int32_t)tilesSize.x] + ((int32_t)id / (int32_t)tilesSize.x); 
    }

    Tile *Map::getTileByGridPos(uint32_t x, uint32_t y){
        return tiles[y] + x;
    }

    Tile *Map::getTileAt(ARC_FPoint pos){
        if(pos.x < 0 || pos.y < 0 || pos.x > tilesSize.x * tileBounds.w * scale || pos.y > tilesSize.y * tileBounds.h * scale){
            return nullptr;
        }

        return tiles[(int32_t)pos.y / (int32_t)(tileBounds.w * scale)] + ((int32_t)pos.x / (int32_t)(tileBounds.h * scale));
    }

    ARC_Point Map::getTileGridPosById(uint32_t id){
        return (ARC_Point){
            (int32_t)id % (int32_t)tilesSize.x,
            (int32_t)id / (int32_t)tilesSize.x 
        };
    }

    std::vector<Tile *> Map::getTilesOfTypes(uint32_t types){
        std::vector<Tile *> returnTiles;

        for(uint32_t y = 0; y < tilesSize.y; y++){
            for(uint32_t x = 0; x < tilesSize.x; x++){
                if(!(types & tiles[y][x].types)){
                    continue;
                }

                returnTiles.push_back(tiles[y] + x);
            }
        }

        return returnTiles;
    }

    ARC_FPoint Map::getTilePosCenter(Tile tile){
        ARC_Point tilePos = {
            (int32_t)tile.id % (int32_t)tilesSize.x,
            (int32_t)tile.id / (int32_t)tilesSize.x 
        };

        return {
            (tilePos.x * (float)tileBounds.w * (float)scale) + (((float)tileBounds.w * (float)scale) / 2.0f),
            (tilePos.y * (float)tileBounds.h * (float)scale) + (((float)tileBounds.h * (float)scale) / 2.0f),
        };
    }

    ARC_FPoint Map::getTilePosCenterByGridPos(ARC_Point pos){
        return {
            (pos.x * (float)tileBounds.w * (float)scale) + (((float)tileBounds.w * (float)scale) / 2.0f),
            (pos.y * (float)tileBounds.h * (float)scale) + (((float)tileBounds.h * (float)scale) / 2.0f),
        };
    }

    ARC_Rect Map::getMapBounds(){
        return {
            0,
            0,
            (int32_t)((float)tileBounds.w * (float)scale * (float)tilesSize.x),
            (int32_t)((float)tileBounds.h * (float)scale * (float)tilesSize.y)
        };
    }

    ARC_Rect Map::getMapGridBounds(){
        return {
            0,
            0,
            (int32_t)tilesSize.x,
            (int32_t)tilesSize.y
        };
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
                tiles[y][x].id = (y * tilesSize.x) + x;
            }
        }
    }
}