#pragma once
#include <archeus.hpp>
#include <vector>

namespace tbyte {
    namespace tools {
        namespace parallax {
            enum AXIS {
                NONE = 0x0,
                X    = 0x1,
                Y    = 0x2,
            };
        }

        class ParallaxLayer {
        public:
            ParallaxLayer(char *name, double scale, parallax::AXIS repeatAxis = parallax::AXIS::NONE);
            ParallaxLayer(char *name, ARC_FPoint pos = { 0, 0 }, double scale = 1, parallax::AXIS repeatAxis = parallax::AXIS::NONE);
            ~ParallaxLayer();

            void update();
            void render();
            void render(ARC_FPoint *offset);
            void render(ARC_FPoint *offset, ARC_FPoint *scale);

            void centerOn(ARC_FRect borderBounds);

            void offsetOn(ARC_FRect *initialBounds, ARC_FPoint *origin, ARC_FPoint *trackedPos, double *scale);

            ARC_FRect getBounds();

            void setPos(ARC_FPoint pos);
            void setRenderedArea(ARC_Rect renderedArea);

        private:
            ARC_Sprite *sprite;
            ARC_FRect bounds;
            double scale;

            ARC_Rect *renderedArea;
            std::vector<ARC_Rect> renderBounds;

            parallax::AXIS repeatAxis;
        };
    }
}