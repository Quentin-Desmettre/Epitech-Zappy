/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_TEAM_CPP
** File description:
** Team.cpp
*/

#include "Informations/Team.hpp"
#include <random>

Team::Team(const std::string &name)
{
    _name = name;
    std::random_device generator;
    std::uniform_real_distribution<> distribution(0, 255);

    auto r = static_cast<unsigned char>(distribution(generator));
    auto g = static_cast<unsigned char>(distribution(generator));
    auto b = static_cast<unsigned char>(distribution(generator));

    _color = {r, g, b, 255};
    this->width = 0;
    this->height = 0;
}

std::string Team::getName() const
{
    return _name;
}
Color Team::getColor() const
{
    return _color;
}

bool Team::operator ==(const Team &team) const
{
    return _name == team.getName();
}
