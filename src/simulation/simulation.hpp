#pragma once
#include <archeus.hpp>
#include "map/map.hpp"
#include "mob/player.hpp"

namespace tbyte {
    class Simulation : public arc::State {
    public:
        Simulation();
        ~Simulation();

        void update();
        void render();

    private:
        void initMap(char *group);

        void runPlayer();

        bool getPath(ARC_Point &pos, bool **&visitedTiles);

        ARC_FPoint offset;

        Map *map;

        mob::Player *player;

        bool temp;

        uint32_t mapId;
        uint32_t lastMapId;
    };
}