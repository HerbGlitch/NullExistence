#include "simulation.hpp"
#include "keybinds.hpp"
#include "menu/start.hpp"
#include <vector>

namespace tbyte {
    Simulation::Simulation(){
        arc::config->load("res/config/maps.config");
        arc::config->load("res/config/player.config");

        offset = { 0.0f, 0.0f };

        map = new Map(&offset, (char *)"map_00");

        std::vector<Tile *> playerSpawnPoints = map->getTilesOfTypes((uint32_t)TileType::PLAYER_SPAWN);

        if(!playerSpawnPoints.empty()){
            ARC_FPoint spawnPoint = map->getTilePosCenter(*playerSpawnPoints.at(0));
            printf("SPAWN: %f, %f\n", spawnPoint.x, spawnPoint.y);
            player = new mob::Player(&offset, (char *)"player", spawnPoint, 1.0);
        }

        temp = false;

        ARC_Rect mapBounds = map->getMapBounds();
        offset = {
            ((float)arc::data->windowSize.x - (float)mapBounds.w) / 2.0f,
            ((float)arc::data->windowSize.y - (float)mapBounds.h) / 2.0f
        };
    }

    Simulation::~Simulation(){
        delete player;

        delete map;

        arc::config->unload("res/config/player.config");
        arc::config->unload("res/config/maps.config");
    }

    void Simulation::update(){
        if(arc::keybinds.isInputState(arc::KEYBIND::ESCAPE, arc::INPUT_STATE::PRESSED)){
            arc::State *state = new state::Start;
            arc::Engine::addState(state);
            return;
        }

        player->update();
        map->update();

        if(!temp && arc::keybinds.isInputState(arc::KEYBIND::ENTER, arc::INPUT_STATE::PRESSED)){
            std::vector<Tile *> playerSpawnPoints = map->getTilesOfTypes((uint32_t)TileType::PLAYER_SPAWN);

            if(playerSpawnPoints.empty()){
                return;
            }

            ARC_Point nextPos = map->getTileGridPosById(playerSpawnPoints.at(0)->id);

            for(int32_t i = 0; i < 5; i++){
                nextPos.y--;
                player->queueMovePosition(map->getTilePosCenterByGridPos(nextPos));
            }

            for(int32_t i = 0; i < 8; i++){
                nextPos.x--;
                player->queueMovePosition(map->getTilePosCenterByGridPos(nextPos));
            }

            for(int32_t i = 0; i < 2; i++){
                nextPos.y++;
                player->queueMovePosition(map->getTilePosCenterByGridPos(nextPos));
            }

            for(int32_t i = 0; i < 6; i++){
                nextPos.x++;
                player->queueMovePosition(map->getTilePosCenterByGridPos(nextPos));
            }

            for(int32_t i = 0; i < 2; i++){
                nextPos.y++;
                player->queueMovePosition(map->getTilePosCenterByGridPos(nextPos));
            }

            for(int32_t i = 0; i < 4; i++){
                nextPos.x--;
                player->queueMovePosition(map->getTilePosCenterByGridPos(nextPos));
            }

            nextPos.y++;
            player->queueMovePosition(map->getTilePosCenterByGridPos(nextPos));

            temp = true;
        }
    }

    void Simulation::render(){
        map->render();
        player->render();
    }
}