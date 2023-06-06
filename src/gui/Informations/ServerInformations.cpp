/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_SERVERINFORMATIONS_CPP
** File description:
** ServerInformations.cpp
*/

#include "Informations/ServerInformations.hpp"

void ServerInformations::setMapSize(int x, int y)
{
    mapSize.x = x;
    mapSize.y = y;
    map.resize(y);
    for (int i = 0; i < y; i++)
        map[i].resize(x);
}

void ServerInformations::setTile(int x, int y, std::vector<int> values)
{
    for (size_t i = 0; i < values.size(); i++)
        map[y][x + i] = values[i];
}

void ServerInformations::addTeam(std::string team)
{
    if (std::find(teams.begin(), teams.end(), team) == teams.end())
        teams.push_back(team);
}

void ServerInformations::addPlayer(std::string name, int x, int y, Player::ORIENTATION orientation, int level, std::string team)
{
    Player player(name, x, y, orientation, level, team);
    players.push_back(player);
}


void ServerInformations::movePlayer(std::string name, int x, int y, Player::ORIENTATION orientation)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].getName() == name) {
            players[i].setPos(x, y);
            players[i].setOrientation(orientation);
            return;
        }
    }
}

void ServerInformations::setPlayerState(std::string name, Player::STATE state)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].getName() == name) {
            players[i].setState(state);
            return;
        }
    }
}

void ServerInformations::setPlayerLevel(std::string name, int level)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].getName() == name) {
            players[i].setLevel(level);
            return;
        }
    }
}

void ServerInformations::setPlayerInventory(std::string name,
    std::vector<int> inventory)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].getName() == name) {
            players[i].setInventory(inventory);
            return;
        }
    }
}

void ServerInformations::startComputing()
{
    mutex.lock();
}

void ServerInformations::endComputing()
{
    mutex.unlock();
}
