#include "tile.hpp"

namespace roguespace {
    namespace world {
        Tile::Tile(uint32_t id, ARC_Spritesheet *sheet, ARC_Rect bounds, ARC_FRect hitbox, ARC_FRect renderBounds, bool collideable){
            ARC_Array *boundsArray = new (ARC_Array){
                .size = 1,
                .data = (void *) new ARC_Rect
            };
            *(ARC_Rect *)boundsArray->data = bounds;
            ARC_Sprite_Create(&sprite, sheet, boundsArray);
            this->hitbox = hitbox;
            this->renderBounds = renderBounds;
            this->color = nullptr;
            this->id = id;

            this->collideable = collideable;
        }

        Tile::~Tile(){
            ARC_Array *boundsArray = ARC_Sprite_GetAllBounds(sprite);
            ARC_Sprite_Destroy(sprite);
            
            delete (ARC_Rect *)boundsArray->data;
            delete boundsArray;
        }

        void Tile::render(ARC_FPoint *offset){
            if(sprite == nullptr){
                return;
            }

            ARC_Rect spriteBounds = ARC_FRect_CastToRect(&renderBounds);
            spriteBounds.x = offset->x + hitbox.x;
            spriteBounds.y = offset->y + hitbox.y;
            ARC_Sprite_Render(sprite, arc::data->renderer, &spriteBounds);

        }

        void Tile::renderHitbox(ARC_FPoint *offset, ARC_Color *color){
            if(this->color == nullptr){
                this->color = color;
                return;
            }

            ARC_FRect bounds = hitbox;
            bounds.x += offset->x;
            bounds.y += offset->y;
            ARC_FRect_Render(&bounds, arc::data->renderer, this->color);
            this->color = color;
        }
        
        ARC_FRect *Tile::getHitbox(){
            return &hitbox;
        }
        
        void Tile::setColor(ARC_Color *color){
            this->color = color;
        }
        
        uint32_t Tile::getId(){
            return id;
        }

        bool Tile::getCollideable(){
            return collideable;
        }
    }
}