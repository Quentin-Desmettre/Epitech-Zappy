/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Map
*/

//
// Created by Edo on 5/31/23.
//

#pragma once
#include "Mateyak/Vector2.hpp"
#include <ctime>
#include <vector>
#include "Perlin/Perlin.hpp"
#include "Mateyak/Sprite.hpp"

union color_u {
    struct {
        unsigned char r, g, b, a;
    };
    unsigned int value;
};

class Map {
    public:
        Map(Mateyak::Vec2f size, float zoom = 1.f);
        void generate();
        void generateColor();
        ~Map();
        Mateyak::Sprite getMap() const;
        Mateyak::Sprite getColor() const;
    private:
        unsigned char randomClr(int rand, int i, int j, int prop = 10);
        std::vector<int> mpp;
        std::vector<int> colors;
        Mateyak::Vec2f _size;
        Mateyak::Sprite map;
        Mateyak::Sprite clr;
        float _zoom;
};
