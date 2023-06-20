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
#include <fmod.hpp>
#include <fmod_errors.h>

class Graphic {
    public:
        Graphic(const Mateyak::Vec2f &mapSize, const Mateyak::Vec2f &windowSize, ServerInformations &serverInformations);
        ~Graphic();
        void loop();
        bool menu(std::string &ip, std::string &port);
        void drawTeams();
        void getTeamsPlace(Mateyak::Window &win);
        void drawBroadCastMessage(Mateyak::Window &win);
        void drawTileInformation(Mateyak::Window &win, Mateyak::Camera &cam);
        void drawPlayerInformation(Mateyak::Window &win, Mateyak::Camera &cam);
        void setMapSize(const Mateyak::Vec2f &mapSize);
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
        std::unique_ptr<Map> _map;
        Mateyak::Model3D _flat;
        Mateyak::Shaders _shader;
        FMOD::System *_system;
        FMOD::Sound *_sound;
        FMOD::Channel *_channel;
};

#endif //EPITECH_ZAPPY_GRAPHIC_HPP