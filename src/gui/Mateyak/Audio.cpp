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
    _channel->stop();
}

void Mateyak::Audio::playSound()
{
    FMOD_RESULT result = _system->playSound(audios[_actionType], nullptr, false, &_channel);
    if (result != FMOD_OK)
        std::cout << "Error while playing sound" << std::endl;
    _beingPlayed = true;

    result = _system->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &_dspFilter);
    if (result != FMOD_OK)
        std::cout << "Error while creating DSP" << std::endl;
    result = _channel->addDSP(0, _dspFilter);
    if (result != FMOD_OK)
        std::cout << "Error while adding DSP : " << FMOD_ErrorString(result) << std::endl;

    result = _system->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &_dspReverb);
    if (result != FMOD_OK)
        std::cout << "Error while creating DSP" << std::endl;
    result = _channel->addDSP(0, _dspReverb);
    if (result != FMOD_OK)
        std::cout << "Error while adding DSP : " << FMOD_ErrorString(result) << std::endl;
    _dspReverb->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, 100.0f);
    result = _system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &_dspPhaser);
    if (result != FMOD_OK)
        std::cout << "Error while creating DSP" << std::endl;
    result = _channel->addDSP(0, _dspPhaser);
    if (result != FMOD_OK)
        std::cout << "Error while adding DSP : " << FMOD_ErrorString(result) << std::endl;
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
    float y = camPos.z - std::get<1>(pos);
    float z = camPos.y - 0;

    float angle = atan2(x, y) * (180 / PI);
    if (angle < 0)
        angle += 360;
    float angle2 = atan2(camRot.x, camRot.z) * (180 / PI);
    if (angle2 < 0)
        angle2 += 360;
    float angle3 = angle - angle2;
    if (angle3 < 0)
        angle3 += 360;

    float distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    float pan = sin(angle3 * 0.017453292f);
    pan *= 0.95f;

    if (distance == 0)
        this->setVolume(1);
    else {
        float volume = (100 + 50 * log10(1 / distance)) / 100;
        if (volume < 0)
            volume = 0;
        if (angle3 >= 0 && angle3 <= 90) {
            volume *= 0.60f + ((angle3) * (0.30f / 90));
        }
        if (angle3 <= 360 && angle3 > 270) {
            volume *= 0.90f - ((angle3 - 270) * (0.30f / 90));
        }
        if (angle3 > 90 && angle3 <= 180) {
            volume *= 0.90f + ((angle3 - 90) * (0.10f / 90));
        }
        if (angle3 > 180 && angle3 <= 270) {
            volume *= 1.0f - ((angle3 - 180) * (0.10f / 90));
        }
        if (volume > 1)
            volume = 1;
        if (volume < 0)
            volume = 0;
        this->setVolume(volume);
    }
    setEffects(angle3);
    this->setStereo(pan);
}

void Mateyak::Audio::setEffects(float angle) const
{
    if (angle >= 0 && angle <= 90) {
        _dspFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 5000.0f + ((angle) * ((22000.0f - 5000.0f) / 90)));
        _dspReverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, 100.0f - ((angle) * (100.0f / 90)));
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_MIX, 25.0f - ((angle) * (25.0f / 90)));
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_DEPTH, 0.5f - ((angle) * (0.5f / 90)));
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_RATE, 10.f - ((angle) * (10.f / 90)));
    }
    if (angle <= 360 && angle > 270) {
        _dspFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000.0f - ((angle - 270) * ((22000.0f - 5000.0f) / 90)));
        _dspReverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, 0.0f + ((angle - 270) * (100.0f / 90)));
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_MIX, 0.0f + ((angle - 270) * (25.0f / 90)));
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_DEPTH, 0.0f + ((angle - 270) * (0.5f / 90)));
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_RATE, 0.0f + ((angle - 270) * (10.0f / 90)));
    }
    if (angle > 90 && angle <= 180) {
        _dspFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000.0f);
        _dspReverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, 0.0f);
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_MIX, 0.0f);
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_DEPTH, 0.0f);
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_RATE, 0.f);
    }
    if (angle > 180 && angle <= 270) {
        _dspFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000.0f);
        _dspReverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, 0.0f);
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_MIX, 0.0f);
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_DEPTH, 0.0f);
        _dspPhaser->setParameterFloat(FMOD_DSP_FLANGE_RATE, 0.f);
    }
}
