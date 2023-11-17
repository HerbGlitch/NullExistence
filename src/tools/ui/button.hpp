#pragma once
#include <archeus.hpp>

namespace tbyte {
    namespace tools {
        class Button {
        public:
            Button(char *name, double scale);
            Button(char *name, ARC_FPoint pos = { 0, 0 }, double scale = 1);
            ~Button();

            void render();

            void scaleFromCenter(double scale);
            
            bool collides(ARC_Point *point);

            ARC_FRect getBounds();

            double getScale();

            void setPos(ARC_FPoint pos);

        private:
            ARC_Sprite *sprite;
            ARC_FRect bounds;
            double scale;
        };
    }
}