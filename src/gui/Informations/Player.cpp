/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_PLAYER_CPP
** File description:
** Player.cpp
*/

#include "Informations/Player.hpp"

Player::Player(std::string name, int x, int y, Player::ORIENTATION orientation, int level, std::string team)
{
    _nextPosition.x = _position.x = x;
    _nextPosition.y = _position.y = y;
    _level = level;
    _inventory.fill(0);
    _orientation = orientation;
    _team = team;
    _name = name;
    _state = Player::STATE::NONE;
    _eggTime = 0;
    _broadcastMessage = "";
}

std::string Player::getName() const
{
    return _name;
}

void Player::setPos(int x, int y)
{
    _nextPosition.x = x;
    _nextPosition.y = y;
}

void Player::setOrientation(Player::ORIENTATION orientation)
{
    _orientation = orientation;
}

void Player::setState(Player::STATE state)
{
    _state = state;
}

void Player::setLevel(int level)
{
    _level = level;
}

void Player::setInventory(std::vector<int> inventory)
{
    for (size_t i = 0; i < inventory.size(); i++)
        _inventory[i] = inventory[i];
}
