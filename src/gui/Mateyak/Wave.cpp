//
// Created by Talleux on 13/06/2023.
//

#include "Mateyak/Wave.hpp"

const std::map<Mateyak::action_type, std::shared_ptr<Mateyak::AudioWave>> Mateyak::audios = {
        {Mateyak::BROADCAST, std::make_shared<Mateyak::AudioWave>("assets/sounds/broadcast.wav")},
        {Mateyak::ELEVATIONSTART, std::make_shared<Mateyak::AudioWave>("assets/sounds/elevation_start.wav")},
        {Mateyak::ELEVATIONEND, std::make_shared<Mateyak::AudioWave>("assets/sounds/elevation_end.wav")},
        {Mateyak::LEVELUP, std::make_shared<Mateyak::AudioWave>("assets/sounds/level_up.wav")}
};

Mateyak::AudioWave::AudioWave(const std::string& path)
{
    _wave = LoadWave(path.c_str());
}

Mateyak::AudioWave::~AudioWave()
{
}

Wave Mateyak::AudioWave::getWave() const
{
    return _wave;
}

Mateyak::AudioWave Mateyak::AudioWave::getAudioWaveFromActionType(Mateyak::action_type actionType)
{
    for (auto& audio : audios) {
        if (audio.first == actionType)
            return *audio.second;
    }
    return AudioWave("assets/sounds/broadcast.wav");
}


