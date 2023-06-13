//
// Created by Talleux on 13/06/2023.
//

#pragma once

#include <raylib.h>
#include <string>
#include <memory>

namespace Mateyak
{
    class Audio {
        public:
            explicit Audio(Wave wave);
            ~Audio();

            void playSound();
            void stopSound();
            void pauseSound();
            void setVolume(float volume);
            void setStereo(float pan);
            bool getState();
        private:
            Sound _sound{};
    };
}


