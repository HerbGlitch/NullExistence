#pragma once
#include <archeus.hpp>

namespace tbyte {
    namespace tools {
        namespace camera {
            enum AXIS {
                NONE = 0x0,
                X    = 0x1,
                Y    = 0x2,
            };
        }

        class Camera {
        public:
            Camera(ARC_Rect viewSize, ARC_FRect *followTarget = nullptr, uint32_t followAxis = camera::AXIS::NONE);
            ~Camera();

            void update();
            void renderScreenCover();

            void centerOn(ARC_FRect *target);
            void centerViewOnWindow();

            void setScale(ARC_FPoint scale);

            ARC_FPoint *getPos();
            ARC_FPoint *getOffset();
            ARC_FPoint *getScale();

            ARC_FRect getScaledViewSize();

        private:
            ARC_FPoint pos;
            ARC_FPoint offset;
            ARC_Rect viewSize;
            ARC_FRect *followTarget;
            uint32_t followAxis;

            ARC_FPoint scale;
        };
    }
}