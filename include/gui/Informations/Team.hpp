/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** Team.hpp
*/

#ifndef EPITECH_ZAPPY_TEAM_HPP
#define EPITECH_ZAPPY_TEAM_HPP

#include <string>
#include <raylib.h>

class Team {
    public:
        Team(const std::string &name);
        ~Team() = default;

        std::string getName() const;
        Color getColor() const;

        bool operator ==(const Team &team) const;

        int width;
        int height;

    private:
        std::string _name;
        Color _color{};
};

#endif //EPITECH_ZAPPY_TEAM_HPP