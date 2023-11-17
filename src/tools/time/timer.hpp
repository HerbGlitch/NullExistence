#pragma once
#include <functional>

namespace tbyte {
    namespace tools {
        class Timer {
        public:
            Timer(std::function<void()> callback, double timeout, bool loop = true, bool startRunning = true);
            ~Timer();

            void start();
            void stop();
            void reset(bool startRunning);

            void update();
            
            void setCallback(std::function<void()> callback);
            void setTimout(double timeout);

        private:
            std::function<void()> callback;
            double timeout, current;
            bool loop, running;
        };
    }
}