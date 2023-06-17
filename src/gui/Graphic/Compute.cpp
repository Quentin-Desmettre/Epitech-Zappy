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

Graphic::Graphic(Mateyak::Vec2f mapSize, Mateyak::Vec2f windowSize, ServerInformations &serverInformations) :
    _serverInformations(serverInformations),
    _windowWidth(windowSize.x), _windowHeight(windowSize.y),
    _mapSize(mapSize),
    _win(_windowWidth, _windowHeight, "Zappy", 400),
    _cam({5.0f, 5.0f, 5.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f),
    _map(mapSize * 10, 0.5),
    _flat(GenMeshPoly(10, 10000.0f), Mateyak::Vec3f{-500, -1, -500}, 1.0f, BLACK),
    _shader("src/gui/shader/base_lighting.vs", "src/gui/shader/test.fs")
{
    _shader.setUniform("fogDensity", 0.015f);
    _shader.setUniform("lightsPos", {mapSize.x * 5.f / 3.f, 20.0f, mapSize.y * 5.f / 3.f});
    _shader.setUniform("lightsColor", {0.5f, 0.5f, 0.5f});
    _shader.setUniform("lightsEnabled", 1);
    getTeamsPlace(_win);
}
