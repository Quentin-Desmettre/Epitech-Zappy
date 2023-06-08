/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** ServerInformations.hpp
*/

#pragma once

#include <string>
#include <vector>
#include <array>
#include <mutex>
#include "Mateyak/Vector2.hpp"
#include <algorithm>
#include "Informations/Player.hpp"

class ServerInformations
{
    public:
        ServerInformations() = default;
        ~ServerInformations() = default;

        void startComputing();
        void endComputing();

        void updatePlayer(std::unique_ptr<Player> &player);

        void setMapSize(int x, int y);
        void setTile(int x, int y, std::vector<int> values);
        void addTeam(std::string team);
        void addPlayer(std::string name, int x, int y, Player::ORIENTATION orientation, int level, std::string team);
        void movePlayer(std::string name, int x, int y, Player::ORIENTATION orientation);
        void setPlayerState(std::string name, Player::STATE state);
        void setPlayerLevel(std::string name, int level);
        void setPlayerInventory(std::string name, std::vector<int> inventory);
        void removePlayer(std::string name);
        void setPlayerDead(std::string name);

        Mateyak::Vec2f getMapSize() const;
        std::vector<std::vector<std::array<int, 7>>> getMap() const;
        std::vector<std::string> getTeams() const;
        std::vector<std::unique_ptr<Player>> &getPlayers();

    private:
        Mateyak::Vec2f mapSize;
        std::vector<std::vector<std::array<int, 7>>> map;
        std::vector<std::string> teams;
        std::vector<std::unique_ptr<Player>> players;
        std::mutex mutex;
};
