#include "simulation.hpp"
#include "keybinds.hpp"
#include "menu/start.hpp"
#include <vector>
#include <time.h>
#include <stdlib.h>

namespace tbyte {
    Simulation::Simulation(){
        arc::config->load("res/config/maps.config");
        arc::config->load("res/config/player.config");

        offset = { 0.0f, 0.0f };

        mapId = 0;
        lastMapId = 2;
        map = nullptr;
        player = nullptr;
        initMap((char *)"map_");
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
            runPlayer();
        }

        std::vector<Tile *> exitTiles = map->getTilesOfTypes((uint32_t)TileType::PLAYER_EXIT);
        for(Tile *& exitTile : exitTiles){
            Tile *playerTile = map->getTileAt(player->getPos());
            if(playerTile->id == exitTile->id){
                mapId++;
                if(mapId > lastMapId){
                    return;
                }

                initMap((char *)"map_");
                return;
            }
        }
    }

    void Simulation::render(){
        map->render();
        player->render();
    }

    void Simulation::initMap(char *group){
        if(map != nullptr){
            delete map;
        }

        char groupName[strlen(group) + 65];
        sprintf(groupName, "%s%u", group, mapId);
        map = new Map(&offset, groupName);

        std::vector<Tile *> playerSpawnPoints = map->getTilesOfTypes((uint32_t)TileType::PLAYER_SPAWN);

        if(!playerSpawnPoints.empty()){
            srand((uint32_t)time(NULL));
            int32_t locationIndex = rand() % playerSpawnPoints.size();

            ARC_FPoint spawnPoint = map->getTilePosCenter(*playerSpawnPoints.at(locationIndex));

            if(player != nullptr){
                delete player;
            }
            player = new mob::Player(&offset, (char *)"player", spawnPoint, 5.0);
        }

        temp = false;

        ARC_Rect mapBounds = map->getMapBounds();
        offset = {
            ((float)arc::data->windowSize.x - (float)mapBounds.w) / 2.0f,
            ((float)arc::data->windowSize.y - (float)mapBounds.h) / 2.0f
        };
    }

    void Simulation::runPlayer(){
        Tile *playerTile = map->getTileAt(player->getPos());
        ARC_Point nextPos = map->getTileGridPosById(playerTile->id);
        ARC_Rect gridBounds = map->getMapGridBounds();

        bool **visitedTiles = new bool *[gridBounds.w];
        for(int32_t y = 0; y < gridBounds.w; y++){
            visitedTiles[y] = new bool[gridBounds.x];
            for(int32_t x = 0; x < gridBounds.h; x++){
                visitedTiles[y][x] = false;
            }
        }

        getPath(nextPos, visitedTiles);

        for(int32_t y = 0; y < gridBounds.w; y++){
            delete [] visitedTiles[y];
        }
        delete [] visitedTiles;

        temp = true;
    }

    bool Simulation::getPath(ARC_Point &pos, bool **&visitedTiles){
        Tile *current = map->getTileByGridPos((uint32_t)pos.x, (uint32_t)pos.y);

        ARC_Rect gridBounds = map->getMapGridBounds();
        if(visitedTiles[pos.y][pos.x]){
            return false;
        }

        if(current->types & (uint32_t)TileType::COLLIDEABLE){
            return false;
        }

        visitedTiles[pos.y][pos.x] = true;
        player->queueMovePos(map->getTilePosCenterByGridPos(pos));

        if(current->types & (uint32_t)TileType::PLAYER_EXIT){
            return true;
        }

        ARC_Point nextPos = pos;
        nextPos.y--;
        if(getPath(nextPos, visitedTiles)){
            return true;
        }
        player->queueMovePos(map->getTilePosCenterByGridPos(pos));

        nextPos = pos;
        nextPos.y++;
        if(getPath(nextPos, visitedTiles)){
            return true;
        }
        player->queueMovePos(map->getTilePosCenterByGridPos(pos));

        nextPos = pos;
        nextPos.x--;
        if(getPath(nextPos, visitedTiles)){
            return true;
        }
        player->queueMovePos(map->getTilePosCenterByGridPos(pos));

        nextPos = pos;
        nextPos.x++;
        if(getPath(nextPos, visitedTiles)){
            return true;
        }

        player->queueMovePos(map->getTilePosCenterByGridPos(pos));
        return false;
    }
}