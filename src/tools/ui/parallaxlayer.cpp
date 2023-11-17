#include "parallaxlayer.hpp"

namespace tbyte {
    namespace tools {
        ParallaxLayer::ParallaxLayer(char *name, double scale, parallax::AXIS repeatAxis): ParallaxLayer(name, { 0.0f, 0.0f }, scale, repeatAxis){}

        ParallaxLayer::ParallaxLayer(char *name, ARC_FPoint pos, double scale, parallax::AXIS repeatAxis): scale(scale), repeatAxis(repeatAxis){
            sprite = arc::config->get<ARC_Sprite>(name);

            ARC_Rect spriteBounds = *ARC_Sprite_GetBounds(sprite);
            bounds.x = pos.x;
            bounds.y = pos.y;
            bounds.w = (float)spriteBounds.w;
            bounds.h = (float)spriteBounds.h;

            bounds.w *= scale;
            bounds.h *= scale;

            renderedArea = nullptr;
        }

        ParallaxLayer::~ParallaxLayer(){
            if(renderedArea){
                delete renderedArea;
            }
        }

        void ParallaxLayer::render(){
            for(ARC_Rect &currentBounds : renderBounds){
                ARC_Sprite_Render(sprite, arc::data->renderer, &currentBounds);
            }
        }

        void ParallaxLayer::render(ARC_FPoint *offset){
            for(ARC_Rect currentBounds : renderBounds){
                currentBounds.x += (int32_t)offset->x;
                currentBounds.y += (int32_t)offset->y;
                ARC_Sprite_Render(sprite, arc::data->renderer, &currentBounds);
            }
        }

        void ParallaxLayer::render(ARC_FPoint *offset, ARC_FPoint *scale){
            for(ARC_Rect currentBounds : renderBounds){
                currentBounds.x = (int32_t)(((float)currentBounds.x + offset->x) * scale->x);
                currentBounds.y = (int32_t)(((float)currentBounds.y + offset->y) * scale->y);
                currentBounds.w = (int32_t)((float)currentBounds.w * scale->x);
                currentBounds.h = (int32_t)((float)currentBounds.h * scale->y);
                ARC_Sprite_Render(sprite, arc::data->renderer, &currentBounds);
            }
        }

        void ParallaxLayer::update(){
            renderBounds.clear();

            //check bounds if no repeat
            if(repeatAxis == parallax::AXIS::NONE){
                ARC_Rect currentBounds = ARC_FRect_CastToRect(&bounds);
                if(ARC_Rect_Intersects(renderedArea, &currentBounds)){
                    renderBounds.push_back(currentBounds);
                }
                return;
            }

            //repeat only on x axis
            if((repeatAxis & parallax::AXIS::X) && !(repeatAxis & parallax::AXIS::Y)){
                //check if outside of bounds that will be drawn
                if(bounds.y > renderedArea->y + renderedArea->h || bounds.y + bounds.h < renderedArea->y){
                    return;
                }

                int32_t repeatX = (renderedArea->w % (int32_t)bounds.w) + 1;
                int32_t posX = ((renderedArea->w) / (int32_t)(bounds.w)) - ((renderedArea->x - (int32_t)bounds.x));
                posX %= (int32_t)bounds.w;

                if(posX != 0 || renderedArea->w % (int32_t)bounds.w){
                    repeatX++;

                    if(posX > 0){
                        posX -= (int32_t)bounds.w;
                    }
                }

                for(int32_t x = 0; x < repeatX; x++){
                    renderBounds.push_back({
                        .x = posX + (int32_t)((float)x * bounds.w),
                        .y = (int32_t)bounds.y,
                        .w = (int32_t)bounds.w,
                        .h = (int32_t)bounds.h
                    });
                }

                return;
            }

            //repeat only on y axis
            //TODO: check to see if this works
            if((repeatAxis & parallax::AXIS::Y) && !(repeatAxis & parallax::AXIS::X)){
                //check if outside of bounds that will be drawn
                if(bounds.x > renderedArea->x + renderedArea->w || bounds.x + bounds.w < renderedArea->x){
                    return;
                }

                int32_t repeatY = (renderedArea->h % (int32_t)bounds.h) + 1;
                int32_t posY = ((renderedArea->h) / (int32_t)(bounds.h)) - ((renderedArea->y - (int32_t)bounds.y));
                posY %= (int32_t)bounds.h;

                if(posY != 0 || renderedArea->h % (int32_t)bounds.h){
                    repeatY++;

                    if(posY > 0){
                        posY -= (int32_t)bounds.w;
                    }
                }

                for(int32_t y = 0; y < repeatY; y++){
                    renderBounds.push_back({
                        .x = (int32_t)bounds.x,
                        .y = posY + (int32_t)((float)y * bounds.h),
                        .w = (int32_t)bounds.w,
                        .h = (int32_t)bounds.h
                    });
                }

                return;
            }

            //TODO: write repeat in both directions
        }

        void ParallaxLayer::centerOn(ARC_FRect borderBounds){
            ARC_FRect_CenterOn(&bounds, &borderBounds);
        }

        void ParallaxLayer::offsetOn(ARC_FRect *initialBounds, ARC_FPoint *origin, ARC_FPoint *trackedPos, double *scale){
            bounds.x = initialBounds->x + ((origin->x - trackedPos->x) * *scale);
            bounds.y = initialBounds->y + ((origin->y - trackedPos->y) * *scale);
        }

        ARC_FRect ParallaxLayer::getBounds(){
            return bounds;
        }

        void ParallaxLayer::setPos(ARC_FPoint pos){
            bounds.x = pos.x;
            bounds.y = pos.y;
        }

        void ParallaxLayer::setRenderedArea(ARC_Rect renderedArea){
            if(this->renderedArea == nullptr){
                this->renderedArea = new ARC_Rect;
            }

            *this->renderedArea = renderedArea;
        }
    }
}