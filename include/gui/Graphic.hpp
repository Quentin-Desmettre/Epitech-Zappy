/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** Graphic.hpp
*/

#ifndef EPITECH_ZAPPY_GRAPHIC_HPP
#define EPITECH_ZAPPY_GRAPHIC_HPP

#include "Informations/ServerInformations.hpp"
#include "Mateyak/Window.hpp"

class Graphic {
    public:
        Graphic(Mateyak::Vec2f mapSize, Mateyak::Vec2f windowSize, ServerInformations &serverInformations);
        ~Graphic() = default;
        void loop();
        void drawTeams();
        void getTeamsPlace(Mateyak::Window &win);
        void drawBroadCastMessage(Mateyak::Window &win);
        void drawTileInformation(Mateyak::Window &win, Mateyak::Camera &cam);
        void drawPlayerInformation(Mateyak::Window &win, Mateyak::Camera &cam);
        void drawTimeUnit();

    private:
        ServerInformations &_serverInformations;
        float _windowWidth;
        float _windowHeight;
        int _boxSize = 50;
        int _maxSize = 0;
        int _teamNumber = 0;
        Mateyak::Vec2f _mapSize;
        Vector2 _charSize{0, 0};
        Mateyak::Window _win;
        Mateyak::Camera _cam;
        Map _map;
        Mateyak::Model3D _flat;
        Mateyak::Shaders _shader;
};

#endif //EPITECH_ZAPPY_GRAPHIC_HPP