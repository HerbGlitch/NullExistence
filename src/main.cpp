#include <archeus.hpp>

class Temp : public arc::State {
public:
    Temp(){}
    ~Temp(){}

    void update(){}
    void render(){}
};

namespace arc {
    ARC_Point *init(){
        arc::config->load("res/config/global.config");
        return arc::config->get<ARC_Point>("global::windowSize");
    }

    State *initState(){
        //tbyte::initConfig();
        //tbyte::initKeys();
        return new Temp();
    }

    void deinit(){
        arc::config->unload("res/config/global.config");
    }
}