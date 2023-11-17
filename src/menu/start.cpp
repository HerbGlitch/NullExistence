#pragma once
#include <archeus.hpp>
#include "tools/ui/button.hpp"

namespace tbyte {
    namespace state {
        class Start : public arc::State {
        public:
            Start();
            ~Start();

            void update();
            void render();

        private:
            void setButtonLocations();

            void scaleButton(tools::Button *button);

            tools::Button *play;
            tools::Button *edit;
            tools::Button *exit;

            ARC_Point *mouseCoord;

            double buttonScale;
            double buttonHoverScale;
            double buttonHoverScaleSpeed;

            double *scaleSpeed;
        };
    }
}