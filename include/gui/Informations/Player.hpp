/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** Player.hpp
*/

#pragma once
#include <string>
#include <vector>
#include <cstring>
#include "Mateyak/Vector2.hpp"
#include "venom.hpp"
#include "Team.hpp"
#include <array>

class Player
{
    public:
        enum STATE {INCANTING, DEAD, EGG, BROADCASTING, DROPING, TAKING, NONE};
        enum ORIENTATION {NORTH, EAST, SOUTH, WEST};
        Player(std::string name, int x, int y, Player::ORIENTATION orientation, int level, const Team& team, Mateyak::Vec2f mapSize);
        ~Player() = default;

        std::string getName() const;
        const Team &getTeam() const;
        void setPos(int x, int y);
        void setOrientation(Player::ORIENTATION orientation);
        void setState(STATE state);
        void setLevel(int level);
        void setInventory(std::vector<int> inventory);

        STATE getState() const;
        int getLevel() const;

        Venom ven;
        Mateyak::Vec2f _position;

    private:
        int _level;
        std::array<int, 7> _inventory;
        ORIENTATION _orientation;
        const Team &_team;
        std::string _name;
        STATE _state;
        int _eggTime;
        std::string _broadcastMessage;
};
