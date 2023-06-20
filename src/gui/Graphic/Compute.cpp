/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_COMPUTE_CPP
** File description:
** Compute.cpp
*/

#include "Graphic.hpp"

void Graphic::getTeamsPlace(Mateyak::Window &win)
{
    for (auto &it : _serverInformations.getTeams()) {
        it.getName();
        Vector2 size = MeasureTextEx(win._font, it.getName().c_str(), 15, 1);
        it.width = size.x;
        it.height = size.y;
        if (size.x > _maxSize)
            _maxSize = size.x;
    }
    _teamNumber = _serverInformations.getTeams().size();
    _boxSize = _maxSize * (_teamNumber / 4) + (_teamNumber % 4 > 0 ? _maxSize : 0);
}

void Graphic::setMapSize(const Mateyak::Vec2f &mapSize)
{
    _mapSize = mapSize;
    _cam = Mateyak::Camera({0.0f, 8.0f, 0.0f}, {mapSize.x * 5 / 3.f, 0.5f, mapSize.y * 5 / 3.f}, {0.0f, 1.0f, 0.0f}, 45.0f);
    _map = std::make_unique<Map>(mapSize * 10, 0.5);
    _shader.setUniform("lightsPos", {mapSize.x * 5.f / 3.f, 20.0f, mapSize.y * 5.f / 3.f});
}

Graphic::Graphic(const Mateyak::Vec2f &mapSize, const Mateyak::Vec2f &windowSize, ServerInformations &serverInformations) :
    _serverInformations(serverInformations),
    _windowWidth(windowSize.x), _windowHeight(windowSize.y),
    _mapSize(mapSize),
    _win(_windowWidth, _windowHeight, "Zappy", 400),
    _cam({0.0f, 8.0f, 0.0f}, {mapSize.x * 5 / 3.f, 0.5f, mapSize.y * 5 / 3.f}, {0.0f, 1.0f, 0.0f}, 45.0f),
    _flat(GenMeshPoly(10, 10000.0f), Mateyak::Vec3f{-500, -1, -500}, 1.0f, BLACK),
    _shader("src/gui/shader/base_lighting.vs", "src/gui/shader/test.fs")
{
    _map = std::make_unique<Map>(mapSize * 10, 0.5);
    _shader.setUniform("fogDensity", 0.015f);
    _shader.setUniform("lightsPos", {mapSize.x * 5.f / 3.f, 20.0f, mapSize.y * 5.f / 3.f});
    _shader.setUniform("lightsColor", {0.5f, 0.5f, 0.5f});
    _shader.setUniform("lightsEnabled", 1);
    getTeamsPlace(_win);

    FMOD_RESULT result;
    result = FMOD::System_Create(&_system);
    if (result != FMOD_OK)
        throw std::runtime_error("FMOD error! (" + std::to_string(result) + ") " + FMOD_ErrorString(result));
    result = _system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK)
        throw std::runtime_error("FMOD error! (" + std::to_string(result) + ") " + FMOD_ErrorString(result));
    result = _system->createSound("assets/sounds/trantor.mp3", FMOD_LOOP_NORMAL, nullptr, &_sound);
    if (result != FMOD_OK)
        throw std::runtime_error("FMOD error! (" + std::to_string(result) + ") " + FMOD_ErrorString(result));
    result = _system->playSound(_sound, nullptr, false, &_channel);
    if (result != FMOD_OK)
        throw std::runtime_error("FMOD error! (" + std::to_string(result) + ") " + FMOD_ErrorString(result));
    _channel->setVolume(0.4f);
}
