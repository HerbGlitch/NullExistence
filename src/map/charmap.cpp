#include "charmap.hpp"
#include <archeus.hpp>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

namespace roguespace {
    namespace world {
        CharMap CharMap_GetFromCString(char *mapString){
            CharMap map;

            std::vector<unsigned char> row;
            size_t mapStrLen = strlen(mapString);
            for(size_t i = 0; i < mapStrLen; i++){
                if(mapString[i] == '\n'){
                    if(row.size() != 0){
                        map.push_back(row);
                        row = std::vector<unsigned char>();
                        continue;
                    }
                }

                row.push_back(mapString[i]);
            }

            if(row.size() != 0){
                map.push_back(row);
            }

            return map;
        }

        // void CharMap_Render(CharMap &map, TileMap &tiles, double scale, ARC_FPoint offset){
            // for(uint32_t y = 0; y < map.size(); y++){
                // for(uint32_t x = 0; x < map.at(y).size(); x++){
                    // ARC_Sprite *sprite = tiles[map.at(y).at(x)];
                    // if(sprite == nullptr){
                        // continue;
                    // }

                    // ARC_Rect *spriteBounds = ARC_Sprite_GetBounds(sprite);
                    // ARC_Rect bounds = {
                        // .x = (int32_t)((x * spriteBounds->w * scale) + offset.x),
                        // .y = (int32_t)((y * spriteBounds->h * scale) + offset.y),
                        // .w = (int32_t)((    spriteBounds->w * scale)),
                        // .h = (int32_t)((    spriteBounds->h * scale))
                    // };

                    // ARC_Sprite_Render(sprite, arc::data->renderer, &bounds);
                // }
            // }
        // }

        void CharMap_Print(CharMap &map){
            for(uint32_t y = 0; y < map.size(); y++){
                for(uint32_t x = 0; x < map.at(y).size(); x++){
                    printf("%c", (int)map.at(y).at(x));
                }

                printf("\n");
            }
        }
    }
}