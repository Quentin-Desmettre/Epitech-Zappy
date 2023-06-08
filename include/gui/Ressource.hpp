/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** Rock
*/

//
// Created by Edo on 6/8/23.
//

#pragma once

#include "Mateyak/Vector2.hpp"

class Ressource {
    public:
        Color clr[7] = {
            (Color){255, 255, 255, 255},
            (Color){150, 0, 0, 100},
            (Color){150, 150, 0, 100},
            (Color){200, 100, 0, 100},
            (Color){100, 100, 100, 100},
            (Color){150, 150, 80, 100},
            (Color){0, 150, 150, 100},
        };

        Ressource(Mateyak::Vec2f pos, int type) {
            pos.x += rand() % 100 / 100.f;
            pos.y += rand() % 100 / 100.f;
            this->pos = pos;
            color = clr[type];
            scale = rand() % 10 / 100.0 + 0.1;
            this->type = type;
        }
        ~Ressource() = default;

        Mateyak::Vec2f pos;
        Color color;
        float scale;
        int type;
};
