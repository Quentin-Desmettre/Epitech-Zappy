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
    private:
        ServerInformations _serverInformations;
};

#endif //EPITECH_ZAPPY_GRAPHIC_HPP