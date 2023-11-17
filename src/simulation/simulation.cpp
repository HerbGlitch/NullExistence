#include "simulation.hpp"
#include "keybinds.hpp"
#include "menu/start.hpp"

namespace tbyte {
    Simulation::Simulation(){
        arc::config->load("res/config/maps.config");

        offset = { 0.0f, 0.0f };

        map = new Map(&offset, (char *)"map_00");
    }

    Simulation::~Simulation(){
        delete map;

        arc::config->unload("res/config/maps.config");
    }

    void Simulation::update(){
        if(arc::keybinds.isInputState(arc::KEYBIND::ESCAPE, arc::INPUT_STATE::PRESSED)){
            arc::State *state = new state::Start;
            arc::Engine::addState(state);
            return;
        }

        map->update();
    }

    void Simulation::render(){
        map->render();
    }
}