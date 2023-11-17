#include <archeus.hpp>
#include "config.hpp"
#include "keybinds.hpp"
#include "menu/start.hpp"

namespace arc {
    ARC_Point *init(){
        arc::config->load("res/config/global.config");
        return arc::config->get<ARC_Point>("global::windowSize");
    }

    State *initState(){
        tbyte::initConfig();
        tbyte::initKeys();
        return new tbyte::state::Start();
    }

    void deinit(){
        arc::config->unload("res/config/global.config");
    }
}