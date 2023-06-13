//
// Created by Talleux on 13/06/2023.
//

#pragma once

#include <raylib.h>
#include <string>
#include <memory>
#include "Wave.hpp"

namespace Mateyak
{
    class Audio {
        public:
            explicit Audio(AudioWave wave);
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


