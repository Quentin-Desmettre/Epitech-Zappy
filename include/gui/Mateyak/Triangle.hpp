/*
** EPITECH PROJECT, 2023
** venom
** File description:
** Triangle
*/

#pragma once
#include <raylib.h>
#include "Vector.hpp"

namespace Mateyak {
    class Triangle {
        public:
            Triangle();
            Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Color color = WHITE);
            ~Triangle() = default;

            Vec3f _a;
            Vec3f _b;
            Vec3f _c;
            Color _color;
        private:
    };
}

