/*
** EPITECH PROJECT, 2023
** venom
** File description:
** Utils
*/
#pragma once

#include "Mateyak/Vector.hpp"
#include "Mateyak/Vector2.hpp"
#include "Mateyak/Camera.hpp"
#include "Mateyak/Window.hpp"
#include "Mateyak/Triangle.hpp"
#include <vector>

class Utils {
    public:
        Utils();
        ~Utils();

        static void generateCirclePoints(Mateyak::Vec3f pos, Mateyak::Vec3f dir, float rayon, int nb_point, std::vector<Mateyak::Vec3f> &vec);
        static std::vector<Mateyak::Triangle> connectPointsWithTriangles(std::vector<Mateyak::Vec3f>& points, int pointsPerRow);
        static float differenceAngle(Mateyak::Vec3f v1, Mateyak::Vec3f v2);


    protected:
    private:
};

