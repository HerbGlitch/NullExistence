#pragma once
#include <archeus.hpp>

namespace tbyte {
    class Simulation : arc::State {
    public:
        Simulation();
        ~Simulation();

        void update();
        void render();
    };
}