#include "timer.hpp"
#include <archeus.hpp>

namespace tbyte {
    namespace tools {
        Timer::Timer(std::function<void()> callback, double timeout, bool loop, bool startRunning): callback(callback), timeout(timeout), loop(loop), running(startRunning){
            current = 0.0;
        }

        Timer::~Timer(){
        }

        void Timer::start(){
            running = true;
        }

        void Timer::stop(){
            running = false;
        }
        
        void Timer::reset(bool startRunning){
            running = startRunning;
            current = 0.0;
        }

        void Timer::update(){
            if(!running){
                return;
            }

            current += arc::data->dt;
            if(current >= timeout){
                running = loop;
                current -= timeout;
                callback();
            }
        }

        void Timer::setCallback(std::function<void ()> callback){
            this->callback = callback;
        }

        void Timer::setTimout(double timeout){
            this->timeout = timeout;

            //TODO: maker sure resetting current is what is wanted
            current = 0.0;
        }
    }
}