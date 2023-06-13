//
// Created by Talleux on 13/06/2023.
//

#pragma once

#include <string>
#include <memory>
#include <raylib.h>
#include <vector>
#include <map>

namespace Mateyak
{
    enum action_type {
        BROADCAST,
        ELEVATIONSTART,
        ELEVATIONEND,
        LEVELUP
    };

    class AudioWave {
        public:
            explicit AudioWave(const std::string& path);
            ~AudioWave();

            static AudioWave getAudioWaveFromActionType(enum action_type actionType);

            [[nodiscard]] Wave getWave() const;

        private:
            Wave _wave{};
    };

    extern const std::map<action_type, std::shared_ptr<AudioWave>> audios;
}
