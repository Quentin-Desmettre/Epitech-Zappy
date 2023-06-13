/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** Graphic.hpp
*/

#ifndef EPITECH_ZAPPY_GRAPHIC_HPP
#define EPITECH_ZAPPY_GRAPHIC_HPP

#include "Informations/ServerInformations.hpp"

class Graphic {
    public:
        void loop(Mateyak::Vec2f mapSize);
        ServerInformations &getServerInformations() { return _serverInformations; }
        void drawTeams();
        void getTeamsPlace(Mateyak::Window &win);
        void drawBroadCastMessage(Mateyak::Window &win);
    private:
        ServerInformations _serverInformations;
        float _windowWidth;
        float _windowHeight;
        int _boxSize = 50;
        int _maxSize = 0;
        int _teamNumber = 0;
        Vector2 _charSize{0, 0};
};

#endif //EPITECH_ZAPPY_GRAPHIC_HPP