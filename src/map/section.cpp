#include "section.hpp"

namespace roguespace {
    namespace world {
        Section::Section(ARC_Spritesheet *sheet, ARC_Rect sectionBounds, ARC_FRect tileBounds, ARC_Array *csvData, float scale, bool collideable): sectionBounds(sectionBounds), tileBounds(tileBounds){
            tiles = new Tile*[sectionBounds.w * sectionBounds.h];
            for(uint32_t i = 0; i < (uint32_t)sectionBounds.w * (uint32_t)sectionBounds.h; i++){
                tiles[i] = nullptr;
            }

            ARC_Point offset = { sectionBounds.x, sectionBounds.y };
            loadCSV(sheet, csvData, offset, scale, collideable);

            this->scale = scale;
            tempColor = new (ARC_Color){ .r = 0, .g = 0, .b = 255, .a = 255 };
            tempHoverColor = new (ARC_Color){ .r = 255, .g = 0, .b = 0, .a = 255 };
        }

        Section::~Section(){
            for(uint32_t i = 0; i < (uint32_t)sectionBounds.w * (uint32_t)sectionBounds.h; i++){
                if(tiles[i] != nullptr){
                    delete tiles[i];
                }
            }
            delete[] tiles;
        }

        void Section::render(ARC_FPoint *offset){
            for(uint32_t i = 0; i < (uint32_t)sectionBounds.w * (uint32_t)sectionBounds.h; i++){
                Tile *tile = tiles[i];
                if(tile == nullptr){
                    continue;
                }

                tile->render(offset);
                // TODO: add this to a debug render
                // tile->renderHitbox(offset);
            }
        }
        
        Tile *Section::at(uint32_t x, uint32_t y){
            if(x >= (uint32_t)sectionBounds.w || y >= (uint32_t)sectionBounds.h){
                return nullptr;
            }

            return tiles[(y * sectionBounds.w) + x];
        }

        std::vector<Tile *> Section::tilesInArea(ARC_Rect area){
            std::vector<Tile *> foundTiles;
            
            ARC_Rect sectionArea = {
                .x = sectionBounds.x,
                .y = sectionBounds.y,
                .w = (int32_t)((float)sectionBounds.w * tileBounds.w),
                .h = (int32_t)((float)sectionBounds.h * tileBounds.h)
            };
            if(!ARC_Rect_Intersects(&area, &sectionArea)){
                return foundTiles;
            }

            //todo: fix this
            ARC_Rect gridArea = { 0, 0, 0, 0 };
            gridArea.x = ((int32_t)((float)area.x - sectionBounds.x) / (int32_t)(tileBounds.w * scale));
            gridArea.y = ((int32_t)((float)area.y - sectionBounds.y) / (int32_t)(tileBounds.h * scale));
            gridArea.w = gridArea.x + ((area.w + (int32_t)(tileBounds.w * scale) - 1) / (int32_t)(tileBounds.w * scale) + 1);
            gridArea.h = gridArea.y + ((area.h + (int32_t)(tileBounds.h * scale) - 1) / (int32_t)(tileBounds.h * scale) + 1);

            gridArea.x = (gridArea.x < 0)? 0 : gridArea.x;
            gridArea.y = (gridArea.y < 0)? 0 : gridArea.y;
            gridArea.w = (gridArea.w < 0)? 0 : gridArea.w;
            gridArea.h = (gridArea.h < 0)? 0 : gridArea.h;

            gridArea.x = (gridArea.x >= sectionBounds.w)? sectionBounds.w : gridArea.x;
            gridArea.y = (gridArea.y >= sectionBounds.h)? sectionBounds.h : gridArea.y;
            gridArea.w = (gridArea.w >= sectionBounds.w)? sectionBounds.w : gridArea.w;
            gridArea.h = (gridArea.h >= sectionBounds.h)? sectionBounds.h : gridArea.h;

            for(int32_t y = gridArea.y; y < gridArea.h; y++){
                for(int32_t x = gridArea.x; x < gridArea.w; x++){
                    if(tiles[(y * sectionBounds.w) + x] == nullptr){
                        continue;
                    }

                    Tile *tile = tiles[(y * sectionBounds.w) + x];
                    tile->setColor(tempHoverColor);
                    foundTiles.push_back(tile);
                }
            }

            return foundTiles;
        }

        std::vector<ARC_Rect> Section::splitSpriteSheetToSprites(ARC_Spritesheet *sheet){
            std::vector<ARC_Rect> spriteBounds;
            ARC_Point sheetSize = ARC_Spritesheet_GetSize(sheet);
            
            for(uint32_t y = 0; y < sheetSize.y / (uint32_t)tileBounds.w; y++){
                for(uint32_t x = 0; x < sheetSize.x / (uint32_t)tileBounds.h; x++){
                    spriteBounds.push_back({
                        .x = (int32_t)(x * tileBounds.w),
                        .y = (int32_t)(y * tileBounds.h),
                        .w = (int32_t)tileBounds.w,
                        .h = (int32_t)tileBounds.h
                    });
                }
            }
            
            return spriteBounds;
        }
  
        void Section::loadCSV(ARC_Spritesheet *sheet, ARC_Array *csvData, ARC_Point offset, float scale, bool collideable){
            std::vector<ARC_Rect> spriteBounds = splitSpriteSheetToSprites(sheet);

            for(uint32_t y = 0; y < csvData->size; y++){
                ARC_Array *row = ((ARC_Array **)csvData->data)[y];
                for(uint32_t x = 0; x < row->size; x++){
                    if(((int32_t *)row->data)[x] == -1){
                        continue;
                    }

                    tiles[(y * sectionBounds.w) + x] = new Tile(
                        (uint32_t)((int32_t *)row->data)[x],
                        sheet,
                        spriteBounds.at(((int32_t *)row->data)[x]),
                        {
                            .x = (float)(x * tileBounds.w * scale) + offset.x,
                            .y = (float)(y * tileBounds.h * scale) + offset.y,
                            .w = (float)tileBounds.w * scale, 
                            .h = (float)tileBounds.h * scale
                        },
                        {
                            .x = (float)(x * tileBounds.w * scale) + offset.x,
                            .y = (float)(y * tileBounds.h * scale) + offset.y,
                            .w = (float)tileBounds.w * scale, 
                            .h = (float)tileBounds.h * scale
                        },
                        collideable
                    );
                }
            }
        }
    }
}