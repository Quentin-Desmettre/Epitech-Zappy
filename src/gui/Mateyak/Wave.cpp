//
// Created by Talleux on 13/06/2023.
//

#include "Mateyak/Wave.hpp"

Mateyak::AudioWave::AudioWave(const std::string& path)
{
    _wave = LoadWave(path.c_str());
}

Mateyak::AudioWave::~AudioWave()
{
    UnloadWave(_wave);
}

Wave Mateyak::AudioWave::getWave() const
{
    return _wave;
}


