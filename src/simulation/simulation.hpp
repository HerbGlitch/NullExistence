#pragma once
#include <archeus.hpp>
#include "map/map.hpp"

namespace tbyte {
    class Simulation : public arc::State {
    public:
        Simulation();
        ~Simulation();

        void update();
        void render();

    private:
        ARC_FPoint offset;

        Map *map;
    };
}