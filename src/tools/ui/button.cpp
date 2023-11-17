#include "button.hpp"

namespace tbyte {
    namespace tools {
        Button::Button(char *name, double scale): Button(name, { 0.0, 0.0 }, scale){}

        Button::Button(char *name, ARC_FPoint pos, double scale): scale(scale){
            sprite = arc::config->get<ARC_Sprite>(name);

            ARC_Rect spriteBounds = *ARC_Sprite_GetBounds(sprite);
            bounds.x = pos.x;
            bounds.y = pos.y;
            bounds.w = (float)spriteBounds.w;
            bounds.h = (float)spriteBounds.h;

            bounds.w *= scale;
            bounds.h *= scale;
        }

        Button::~Button(){}

        void Button::render(){
            ARC_Rect renderBounds = ARC_FRect_CastToRect(&bounds);
            ARC_Sprite_Render(sprite, arc::data->renderer, &renderBounds);
        }

        void Button::scaleFromCenter(double scale){
            if(this->scale == scale){
                return;
            }

            this->scale = scale;
            ARC_FRect oldBounds = bounds;

            ARC_Rect spriteBounds = *ARC_Sprite_GetBounds(sprite);
            bounds.x = 0;
            bounds.y = 0;
            bounds.w = (float)spriteBounds.w;
            bounds.h = (float)spriteBounds.h;

            bounds.w *= scale;
            bounds.h *= scale;

            ARC_FRect_CenterOn(&bounds, &oldBounds);
        }
        
        bool Button::collides(ARC_Point *point){
            return (bool)ARC_FRect_IntersectsPoint(&bounds, point);
        }
        
        ARC_FRect Button::getBounds(){
            return bounds;
        }
        
        double Button::getScale(){
            return scale;
        }
        
        void Button::setPos(ARC_FPoint pos){
            bounds.x = pos.x;
            bounds.y = pos.y;
        }
    }
}