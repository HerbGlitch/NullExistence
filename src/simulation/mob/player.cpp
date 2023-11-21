#include "player.hpp"

namespace tbyte {
    namespace mob {
        Player::Player(ARC_FPoint *offset, char *group, ARC_FPoint spawn, double moveTime): offset(offset){
            sprite = arc::config->get<ARC_Sprite>(group, "sprite");
            explosion = arc::config->get<ARC_Sprite>(group, "explosion");
            explosionTimer = new tools::Timer([&]{ animateExplosion(); }, 1000.0 / 60.0, true, false);
            scale = 10.0f;

            pos = centerPos(spawn);
            lastPos = pos;

            this->moveTime = moveTime;
            lerpPos = 0.0f;
        }

        Player::~Player(){
            delete explosionTimer;
        }

        void Player::update(){
            explosionTimer->update();

            if(moveQueue.empty()){
                return;
            }

            ARC_FPoint next = moveQueue.front();
            lerpPos += (moveTime * .001) * arc::data->dt;
            if(lerpPos >= 1.0f){
                lerpPos = 1.0f;
            }

            pos = ARC_FPoint_Lerp(&lastPos, &next, lerpPos);

            if((int32_t)pos.x == (int32_t)next.x && (int32_t)pos.y == (int32_t)next.y){
                lastPos = next;
                moveQueue.pop();
                lerpPos = 0.0f;

                if(moveQueue.empty()){
                    explode();
                }
            }
        }

        void Player::render(){
            ARC_Rect bounds = *ARC_Sprite_GetBounds(sprite);
            bounds.x = (int32_t)(pos.x + offset->x);
            bounds.y = (int32_t)(pos.y + offset->y);
            bounds.w = (int32_t)((float)bounds.w * scale);
            bounds.h = (int32_t)((float)bounds.h * scale);

            ARC_Sprite_Render(sprite, arc::data->renderer, &bounds);

            ARC_Rect explosionBounds = bounds;
            explosionBounds.w *= 4;
            explosionBounds.h *= 4;
            ARC_Rect_CenterOn(&explosionBounds, &bounds);

            ARC_Sprite_Render(explosion, arc::data->renderer, &explosionBounds);
        }

        void Player::moveTo(ARC_FPoint pos){
            moveQueue = {};
            moveQueue.push(centerPos(pos));
        }

        void Player::queueMovePos(ARC_FPoint pos){
            moveQueue.push(centerPos(pos));
        }

        void Player::explode(){
            explosionTimer->reset(true);
        }

        ARC_FPoint Player::getPos(){
            return pos;
        }

        ARC_FPoint Player::centerPos(ARC_FPoint pos){
            ARC_Rect *spriteBounds = ARC_Sprite_GetBounds(sprite);

            return {
                pos.x - (((float)spriteBounds->w * (float)scale) / 2),
                pos.y - (((float)spriteBounds->h * (float)scale) / 2)
            };
        }

        void Player::animateExplosion(){
            ARC_Sprite_IterateFrame(explosion);
            if(ARC_Sprite_GetFrameIndex(explosion) == 0){
                explosionTimer->stop();
            }
        }
    }
}