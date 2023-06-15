//
// Created by Talleux on 13/06/2023.
//

#include "Mateyak/Audio.hpp"
#include <iostream>

/*const std::map<Mateyak::action_type, std::shared_ptr<Mateyak::AudioWave>> Mateyak::audios = {
        {Mateyak::BROADCAST, std::make_shared<Mateyak::AudioWave>("assets/sounds/broadcast.wav")},
        {Mateyak::ELEVATIONSTART, std::make_shared<Mateyak::AudioWave>("assets/sounds/elevation_start.wav")},
        {Mateyak::ELEVATIONEND, std::make_shared<Mateyak::AudioWave>("assets/sounds/elevation_end.wav")},
        {Mateyak::LEVELUP, std::make_shared<Mateyak::AudioWave>("assets/sounds/level_up.wav")}
};*/

std::map<Mateyak::action_type, FMOD::Sound *> Mateyak::audios = {
        {Mateyak::BROADCAST, nullptr},
        {Mateyak::ELEVATIONSTART, nullptr},
        {Mateyak::ELEVATIONEND, nullptr},
        {Mateyak::LEVELUP, nullptr}
};

Mateyak::Audio::Audio(Mateyak::action_type actionType, FMOD::System &system)
{
    _actionType = actionType;
    _system = &system;
}

Mateyak::Audio::~Audio()
{
}

void Mateyak::Audio::playSound()
{
    FMOD_RESULT result = _system->playSound(audios[_actionType], nullptr, false, &_channel);
    if (result != FMOD_OK)
        std::cout << "Error while playing sound" << std::endl;
}

void Mateyak::Audio::stopSound() const
{
    FMOD_RESULT result = _channel->stop();
    if (result != FMOD_OK)
        std::cout << "Error while stopping sound" << std::endl;
}

void Mateyak::Audio::pauseSound() const
{
    FMOD_RESULT result = _channel->setPaused(true);
    if (result != FMOD_OK)
        std::cout << "Error while pausing sound" << std::endl;
}

void Mateyak::Audio::setVolume(float volume) const
{
    FMOD_RESULT result = _channel->setVolume(volume);
    if (result != FMOD_OK)
        std::cout << "Error while setting volume" << std::endl;
}

void Mateyak::Audio::setStereo(float pan) const
{
    FMOD_RESULT result = _channel->setPan(pan);
    if (result != FMOD_OK)
        std::cout << "Error while setting stereo" << std::endl;
}

bool Mateyak::Audio::getState()
{
    bool isPlaying = false;
    FMOD_RESULT result = _channel->isPlaying(&isPlaying);
    if (result != FMOD_OK)
        std::cout << "Error while getting state" << std::endl;
    return isPlaying;
}

void Mateyak::Audio::computeStereoAndVolume(Mateyak::Vec3<float> camPos,
    std::tuple<float, float> pos, Mateyak::Vec3<float> camRot)
{
    std::get<0>(pos) = std::get<0>(pos) * (10/3) + ((10/3) / 2);
    std::get<1>(pos) = std::get<1>(pos) * (10/3) + ((10/3) / 2);

    float x = camPos.x - std::get<0>(pos);
    float y = camPos.y - std::get<1>(pos);

    float angle = atan2(y, x) * (180 / PI);
    if (angle < 0)
        angle += 360;
    float angle2 = atan2(camRot.y, camRot.x) * (180 / PI);
    if (angle2 < 0)
        angle2 += 360;
    float angle3 = angle - angle2;
    if (angle3 < 0)
        angle3 += 360;

    float distance = sqrt(pow(x, 2) + pow(y, 2));

    float pan = sin(angle3);


    this->setVolume(1);
    this->setStereo(pan);
}
