/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_SERVERINFORMATIONS_CPP
** File description:
** ServerInformations.cpp
*/

#include "Informations/ServerInformations.hpp"
#include <iostream>

void Message::FormatMessage(int maxLineSize)
{
    std::string line;
    int i = 0;

    line.insert(0, _name + ": ");

    while (i < static_cast<int>(_message.size())) {
        if (i % maxLineSize == 0 && i != 0) {
            if (!_formated) {
                line.erase(0, _name.size() + 2);
                i -= _name.size() - 1;
            }
            _lines.push_back(line);
            line.clear();
            _formated = true;
        }
        line += _message[i];
        i++;
    }
    if (!_formated)
        line.erase(0, _name.size() + 2);
    _lines.push_back(line);
    _formated = true;

    for (auto &it : _lines)
        std::cout << it << std::endl;
}

Message::Message(std::string name, std::string message, Color color) :
_formated(false), _name(name), _message(message), _lines(), _color(color) {}
void ServerInformations::updatePlayer(std::unique_ptr<Player> &player)
{
    Player::STATE state = player->getState();
    if (state == Player::STATE::NONE) {
        player->ven.move_ven();
        return;
    }
    if (state == Player::STATE::DEAD)
        player->ven.getPosition().y += 0.01;
    if (player->ven.getPos().y > 3.0) {
        removePlayer(player->getName());
    }
    player->ven.setLevel(player->getLevel());
}

void ServerInformations::setMapSize(int x, int y)
{
    mapSize.x = x;
    mapSize.y = y;
    map.resize(y);
    for (int i = 0; i < y; i++) {
        map[i].resize(x);
    }
}

void ServerInformations::setTile(int x, int y, std::vector<int> values)
{
    int number_in_tile;
    int number_to_erase;
    int number_to_add;

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        number_in_tile = 0;
        number_to_erase = 0;
        number_to_add = 0;
        for (auto &it : map[y][x])
            if (it.type == i)
                number_in_tile++;
        if (number_in_tile == values[i])
            continue;
        if (number_in_tile < values[i]) {
            number_to_add = values[i] - number_in_tile;
            for (int j = 0; j < number_to_add; j++) {
                Mateyak::Vec2f pos = {static_cast<float>(x), static_cast<float>(y)};
                map[y][x].emplace_back(pos, i);
            }
        }
        else {
            number_to_erase = number_in_tile - values[i];
            for (size_t j = 0; j < map[y][x].size(); j++)
                if (map[y][x][j].type == i && number_to_erase > 0) {
                    number_to_erase--;
                    map[y][x].erase(map[y][x].begin() + j);
                }
        }
    }
}

void ServerInformations::addTeam(std::string team)
{
    if (std::find(teams.begin(), teams.end(), team) == teams.end()) {
        teams.emplace_back(team);
    }
}

void ServerInformations::addPlayer(std::string name, int x, int y, Player::ORIENTATION orientation, int level, std::string team)
{
    std::unique_ptr<Player> player;
    for (size_t i = 0; i < teams.size(); i++) {
        if (teams[i].getName() == team) {
            player = std::make_unique<Player>(name, x, y, orientation, level, teams[i], mapSize);
            break;
        }
    }
    players.push_back(std::move(player));
}

void ServerInformations::movePlayer(std::string name, int x, int y, Player::ORIENTATION orientation)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setPos(x, y);
            players[i]->setOrientation(orientation);
            return;
        }
    }
}

void ServerInformations::setPlayerState(std::string name, Player::STATE state)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setState(state);
            return;
        }
    }
}

void ServerInformations::setPlayerLevel(std::string name, int level)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setLevel(level);
            return;
        }
    }
}

void ServerInformations::setPlayerInventory(std::string name,
    std::vector<int> inventory)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setInventory(inventory);
            return;
        }
    }
}

void ServerInformations::removePlayer(std::string name)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players.erase(players.begin() + i);
            return;
        }
    }
}

void ServerInformations::setPlayerDead(std::string name)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setState(Player::STATE::DEAD);
            return;
        }
    }
}

void ServerInformations::addBroadCastMessage(std::string name, std::string message)
{
    for (auto &it : players) {
        if (it->getName() == name) {
            broadCastMessage.emplace_back(name, message, it->getTeam().getColor());
            return;
        }
    }
}

Mateyak::Vec2f ServerInformations::getMapSize() const
{
    return mapSize;
}
ZappyMap ServerInformations::getMap() const
{
    return map;
}
std::vector<Team> ServerInformations::getTeams() const
{
    return teams;
}
std::vector<std::unique_ptr<Player>> &ServerInformations::getPlayers()
{
    return players;
}

std::vector<Message> &ServerInformations::getBroadCastMessage()
{
    return broadCastMessage;
}

void ServerInformations::startComputing()
{
    mutex.lock();
}

void ServerInformations::endComputing()
{
    mutex.unlock();
}

void ServerInformations::setIncantationLevel(std::string name, int level)
{
    for (auto &it : players) {
        if (it->getName() == name) {
            it->incantationLevel = level;
            return;
        }
    }
}
