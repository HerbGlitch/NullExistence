#include "start.hpp"
#include "keybinds.hpp"
#include "simulation/simulation.hpp"
// #include "editor.hpp"
#include "tools/ui/button.hpp"

namespace tbyte {
    namespace state {
        Start::Start(){
            arc::config->load("res/config/start_menu.config");

            double scale = *arc::config->get<double>("start_menu::scale");

            play = new tools::Button((char *)"start_menu::play", scale);
            // edit = new tools::Button((char *)"start_menu::edit", scale);
            exit = new tools::Button((char *)"start_menu::exit", scale);

            setButtonLocations();

            mouseCoord = ARC_Mouse_GetCoords(arc::data->mouse);
            
            buttonScale           = *arc::config->get<double>("start_menu::scale");
            buttonHoverScale      = *arc::config->get<double>("start_menu::hoverScale");
            buttonHoverScaleSpeed = *arc::config->get<double>("start_menu::hoverScaleSpeed");
        }

        Start::~Start(){
            delete exit;
            // delete edit;
            delete play;

            arc::config->unload("res/config/start_menu.config");
        }

        void Start::update(){
            scaleButton(play);
            // scaleButton(edit);
            scaleButton(exit);

            if(arc::keybinds.isInputState(arc::KEYBIND::LEFT_CLICK, arc::INPUT_STATE::RELEASED)){
                if(play->collides(mouseCoord)){
                    arc::Engine::addState(new Simulation);
                    return;
                }

                // if(edit->collides(mouseCoord)){
                    // arc::Engine::addState(new Editor);
                    // return;
                // }

                if(exit->collides(mouseCoord)){
                    arc::data->running = 1;
                    return;
                }
            }
        }

        void Start::render(){
            play->render();
            // edit->render();
            exit->render();
        }

        void Start::setButtonLocations(){
            float spacing = (float) *arc::config->get<double>("start_menu::spacing");

            ARC_FRect screenBounds = { 0.0f, 0.0f, (float)arc::data->windowSize.x, (float)arc::data->windowSize.y };

            ARC_FRect buttonBounds = {
                .x = 0.0f,
                .y = 0.0f,
                .w = play->getBounds().w,
                // .h = play->getBounds().h + spacing + edit->getBounds().h + spacing + exit->getBounds().h
                .h = play->getBounds().h + spacing + exit->getBounds().h
            };
            ARC_FRect_CenterOn(&buttonBounds, &screenBounds);

            //set buttons position
            play->setPos({ buttonBounds.x, buttonBounds.y });
            // edit->setPos({ buttonBounds.x, buttonBounds.y + play->getBounds().h + spacing });
            // exit->setPos({ buttonBounds.x, buttonBounds.y + play->getBounds().h + spacing + edit->getBounds().h + spacing });
            exit->setPos({ buttonBounds.x, buttonBounds.y + play->getBounds().h + spacing });
        }

        void Start::scaleButton(tools::Button *button){
            double currentScale = button->getScale();

            if(button->collides(mouseCoord)){
                double newScale = currentScale + (buttonHoverScaleSpeed * arc::data->dt);
                newScale = (newScale > buttonScale + buttonHoverScale)? buttonScale + buttonHoverScale : newScale;
                button->scaleFromCenter(newScale);
                return;
            }

            double newScale = currentScale - (buttonHoverScaleSpeed * arc::data->dt);
            newScale = (newScale < buttonScale)? buttonScale : newScale;
            button->scaleFromCenter(newScale);
        }
    }
}