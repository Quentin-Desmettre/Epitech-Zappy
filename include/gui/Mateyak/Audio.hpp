//
// Created by Talleux on 13/06/2023.
//

#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <memory>
#include <map>
#include "Wave.hpp"
#include "Mateyak/Vector.hpp"

namespace Mateyak
{
    enum action_type {
        BROADCAST,
        NEWPLAYER,
        ELEVATIONSTART,
        ELEVATIONEND,
        LEVELUP
    };

    class Audio {
        public:
            explicit Audio(Mateyak::action_type actionType, FMOD::System &system);
            ~Audio();

            void playSound();
            void stopSound() const;
            void pauseSound() const;
            void setVolume(float volume) const;
            void setStereo(float pan) const;
            bool getState();
            void computeStereoAndVolume(Mateyak::Vec3<float> camPos, std::tuple<float, float> pos, Mateyak::Vec3<float> camRot);
            void setEffects(float angle) const;

            bool _beingPlayed = false;
            FMOD::Channel* _channel{};
            Mateyak::action_type _actionType;
            FMOD::System* _system;
            FMOD::DSP* _dspFilter{};
            FMOD::DSP* _dspPhaser{};
            FMOD::DSP* _dspReverb{};
            FMOD::DSPConnection* _dspConnection{};
    };

    extern std::map<Mateyak::action_type, FMOD::Sound *> audios;
}


