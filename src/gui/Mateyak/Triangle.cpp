/*
** EPITECH PROJECT, 2023
** venom
** File description:
** Triangle
*/

#include "Mateyak/Window.hpp"

Mateyak::Triangle::Triangle()
{
    _a = Mateyak::Vec3f(0, 0, 0);
    _b = Mateyak::Vec3f(0, 0, 0);
    _c = Mateyak::Vec3f(0, 0, 0);
    _color = WHITE;
}

Mateyak::Triangle::Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Color color) : _a(a), _b(b), _c(c), _color(color)
{
    _color = color;
}