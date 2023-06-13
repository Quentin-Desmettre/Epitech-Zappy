//
// Created by Talleux on 13/06/2023.
//

#include "Mateyak/Audio.hpp"

Mateyak::Audio::Audio(AudioWave wave)
{
    _sound = LoadSoundFromWave(wave.getWave());
}

Mateyak::Audio::~Audio()
{
    UnloadSound(_sound);
}

void Mateyak::Audio::playSound()
{
    PlaySound(_sound);
}

void Mateyak::Audio::stopSound()
{
    StopSound(_sound);
}

void Mateyak::Audio::pauseSound()
{
    PauseSound(_sound);
}

void Mateyak::Audio::setVolume(float volume)
{
    SetSoundVolume(_sound, volume);
}

void Mateyak::Audio::setStereo(float pan)
{
    SetSoundPan(_sound, pan);
}

bool Mateyak::Audio::getState()
{
    return IsSoundPlaying(_sound);
}
