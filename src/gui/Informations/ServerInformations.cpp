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
}

Message::Message(const std::string &name, const std::string &message, Color color) :
_formated(false), _name(name), _message(message), _lines(), _color(color) {}

bool ServerInformations::isRunning() const
{
    return _serverRunning;
}

void ServerInformations::setRunning(bool running)
{
    _serverRunning = running;
}

void ServerInformations::updatePlayer(std::unique_ptr<Player> &player)
{
    Player::STATE state = player->getState();
    if (state == Player::STATE::NONE) {
        player->ven.move_ven(_timeUnit);
        return;
    }
    if (state == Player::STATE::DEAD)
        player->ven.getPosition().y += 0.01;
    if (player->ven.getPos().y > 3.0) {
        removePlayer(player->getName());
        return;
    }
    player->ven.setLevel(player->getLevel());
    player->ven.setState(player->getState());
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

    if (y >= static_cast<int>(map.size()) || x >= static_cast<int>(map[y].size())) {
        throw std::runtime_error("Error: setTile: x or y out of range");
    }

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

void ServerInformations::addTeam(const std::string &team)
{
    if (std::find(teams.begin(), teams.end(), team) == teams.end()) {
        teams.emplace_back(team);
    }
}

void ServerInformations::addPlayer(const std::string &name, int x, int y, Player::ORIENTATION orientation, int level, const std::string &team)
{
    std::unique_ptr<Player> player;
    bool team_exist = false;

    for (size_t i = 0; i < teams.size(); i++) {
        if (teams[i].getName() == team) {
            player = std::make_unique<Player>(name, x, y, orientation, level, teams[i], mapSize);
            team_exist = true;
            break;
        }
    }

    for (auto &it : players) {
        int posX = it->ven.getPos().x / (10 / 3.f);
        int posY = it->ven.getPos().z / (10 / 3.f);
        std::cout << posX << " == " << x << " && " << posY << " == " << y << std::endl;
        std::cout << it->getState() << " == " << Player::STATE::EGGHATCHING << std::endl;
        std::cout << "===============" << std::endl;
        if (posX == x && posY == y && it->getState() == Player::STATE::EGGHATCHING) {
            players.erase(std::find(players.begin(), players.end(), it));
            break;
        }
    }
    std::cout << std::endl;
    if (!team_exist)
        throw std::runtime_error("Team doesn't exist");
    players.push_back(std::move(player));
}

void ServerInformations::movePlayer(const std::string &name, int x, int y, Player::ORIENTATION orientation)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setPos(x, y);
            players[i]->setOrientation(orientation);
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::setPlayerState(const std::string &name, Player::STATE state)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setState(state);
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::setPlayerLevel(const std::string &name, int level)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setLevel(level);
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::setPlayerInventory(const std::string &name,
    std::vector<int> inventory)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setInventory(inventory);
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::removePlayer(const std::string &name)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players.erase(players.begin() + i);
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::setPlayerDead(const std::string &name)
{
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == name) {
            players[i]->setState(Player::STATE::DEAD);
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::addBroadCastMessage(const std::string &name, const std::string &message)
{
    for (auto &it : players) {
        if (it->getName() == name) {
            broadCastMessage.emplace_back(name, message, it->getTeam().getColor());
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::PlayerForkEgg(const std::string &name)
{
    for (auto &it : players) {
        if (it->getName() == name) {
            int x = it->ven.getPos().x / (10 / 3.f);
            int y = it->ven.getPos().z / (10 / 3.f);
            std::unique_ptr<Player> tmp = std::make_unique<Player>(it->getName(), x, y, Player::ORIENTATION::NORTH, 1, it->getTeam(), mapSize);
            tmp->setState(Player::STATE::EGGFORKED);
            players.push_back(std::move(tmp));
            return;
        }
    }
    throw std::runtime_error("Player doesn't exist");
}

void ServerInformations::PlayerLayEgg(const std::string &name, const std::string &eggName, int posX, int posY)
{
    for (auto &it : players) {
        if (it->getName() == name && it->getState() == Player::STATE::EGGFORKED) {
            int venX = it->ven.getPos().x / (10 / 3.f);
            int venY = it->ven.getPos().z / (10 / 3.f);
            if (venX == posX && venY == posY) {
                it->setEggName(eggName);
                it->setState(Player::STATE::EGGLAYING);
                return;
            }
        }
    }
    throw std::runtime_error("Player egg doesn't exist");
}

void ServerInformations::EggConnection(const std::string &eggName)
{
    std::cerr << "EggConnection" << std::endl;
    for (auto &it : players) {
        if (it->getEggName() == eggName) {
            it->setState(Player::STATE::EGGHATCHING);
            return;
        }
    }
    throw std::runtime_error("Egg doesn't exist");
}

void ServerInformations::EggDeath(const std::string &eggName)
{
    players.erase(std::remove_if(players.begin(), players.end(), [eggName](std::unique_ptr<Player> &player) {
    return player->getEggName() == eggName;
    }), players.end());
}

void ServerInformations::setTimeUnit(int timeUnit)
{
    _timeUnit = timeUnit;
}

int ServerInformations::getTimeUnit() const
{
    return _timeUnit;
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

void ServerInformations::setIncantationLevel(const std::string &name, int level)
{
    for (auto &it : players) {
        if (it->getName() == name) {
            it->incantationLevel = level;
            return;
        }
    }
}

void ServerInformations::takeRessource(const std::string &name, int ressource)
{
    int x = -1;
    int y = -1;

    for (auto &it : players) {
        if (it->getName() == name) {
            x = it->ven.getPos().x / (10 / 3.f);
            y = it->ven.getPos().z / (10 / 3.f);
        }
    }
    if (x == -1 || y == -1)
        return;
    for (size_t j = 0; j < map[y][x].size(); j++)
        if (map[y][x][j].type == ressource) {
            map[y][x].erase(map[y][x].begin() + j);
            return;
        }
}

void ServerInformations::dropRessource(const std::string &name, int ressource)
{
    int x = -1;
    int y = -1;

    for (auto &it : players) {
        if (it->getName() == name) {
            x = it->ven.getPos().x / (10 / 3.f);
            y = it->ven.getPos().z / (10 / 3.f);
        }
    }
    if (x == -1 || y == -1)
        return;
    Mateyak::Vec2f pos = {static_cast<float>(x), static_cast<float>(y)};
    map[y][x].emplace_back(pos, ressource);
}

std::queue<std::string> &ServerInformations::getCommandQueue()
{
    return _commandQueue;
}

void ServerInformations::addCommand(std::string command)
{
    _commandQueue.push(command);
}

bool ServerInformations::hasCommand() const
{
    return !_commandQueue.empty();
}

std::string ServerInformations::getCommand()
{
    std::string command = _commandQueue.front();
    _commandQueue.pop();
    return command;
}
