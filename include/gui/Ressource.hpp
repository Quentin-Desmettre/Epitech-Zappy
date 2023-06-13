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
            (Color){120, 0, 0, 100},
            (Color){100, 70, 0, 100},
            (Color){130, 50, 10, 100},
            (Color){80, 80, 80, 100},
            (Color){110, 110, 30, 100},
            (Color){130, 100, 100, 100},
        };

        Ressource(Mateyak::Vec2f pos, int type) {
            pos.x += rand() % 100 / 100.f;
            pos.y += rand() % 100 / 100.f;
            this->pos = pos;
            color = clr[type];
            scale = (rand() % 3) / 10.0 + 0.2;
            this->type = type;
        }
        ~Ressource() = default;

        Mateyak::Vec2f pos;
        Color color;
        float scale;
        int type;
};
