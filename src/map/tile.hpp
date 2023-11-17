#pragma once
#include <archeus.hpp>

namespace roguespace {
    namespace world {
        class Tile {
        public:
            Tile(uint32_t id, ARC_Spritesheet *sheet, ARC_Rect spriteBounds, ARC_FRect hitbox, ARC_FRect renderBounds, bool collideable = false);
            ~Tile();

            void render(ARC_FPoint *offset);

            void renderHitbox(ARC_FPoint *offset, ARC_Color *color = nullptr);

            ARC_FRect *getHitbox();
            
            void setColor(ARC_Color *color);

            uint32_t getId();
            bool getCollideable();

        private:
            ARC_Color *color;
            ARC_FRect renderBounds;
            ARC_FRect hitbox;
            ARC_Sprite *sprite;
            uint32_t id;

            bool collideable;
        };
    }
}