#pragma once
#include <archeus.hpp>
#include <queue>
#include "tools/time/timer.hpp"

namespace tbyte {
    namespace mob {
        enum class PlayerState {
            IDLE,
            MOVING
        };

        class Player {
        public:
            Player(ARC_FPoint *offset, char *spriteName, ARC_FPoint spawn, double moveTime);
            ~Player();

            void update();
            void render();

            void moveTo(ARC_FPoint pos);

            void queueMovePos(ARC_FPoint pos);

            void explode();

            ARC_FPoint getPos();
            ARC_FPoint getCenterPos();

        private:
            ARC_FPoint centerPos(ARC_FPoint pos);

            void animateBody();
            void animateExplosion();

            ARC_Sprite *sprite;
            ARC_Sprite *walk;
            tools::Timer *bodyTimer;

            ARC_Sprite *explosion;
            tools::Timer *explosionTimer;

            ARC_FPoint *offset;

            ARC_FPoint pos;
            double scale;

            ARC_FPoint lastPos;
            std::queue<ARC_FPoint> moveQueue;
            double moveTime;
            float lerpPos;

            PlayerState playerState;
        };
    }
}