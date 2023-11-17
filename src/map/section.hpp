#pragma once

#include <archeus.hpp>
#include <stdint.h>
#include <vector>
#include "tile.hpp"

namespace roguespace {
    namespace world {
        class Section {
        public:
            // Section(uint32_t width, uint32_t height, ARC_FRect tileBounds);
            Section(ARC_Spritesheet *sheet, ARC_Rect sectionBounds, ARC_FRect tileBounds, ARC_Array *csvData, float scale = 1.0f, bool collideable = false);
            ~Section();

            void render(ARC_FPoint *offset);

            Tile *at(uint32_t x, uint32_t y);

            std::vector<Tile *> tilesInArea(ARC_Rect area);

        private:
            std::vector<ARC_Rect> splitSpriteSheetToSprites(ARC_Spritesheet *sheet);

            void loadCSV(ARC_Spritesheet *sheet, ARC_Array *csvData, ARC_Point offset, float scale, bool collideable);

            ARC_Spritesheet *sheet;
            Tile **tiles;
            ARC_Rect sectionBounds;
            ARC_FRect tileBounds;
            
            float scale;
            ARC_Color *tempColor;
            ARC_Color *tempHoverColor;
        };
    }
}