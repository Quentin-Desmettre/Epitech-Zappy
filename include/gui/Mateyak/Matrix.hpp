/*
** EPITECH PROJECT, 2023
** Epitech-Raytracer
** File description:
** Matrix
*/

#pragma once

#include <array>
#include <cstdio>
#include "Vector.hpp"
#define argunused [[maybe_unused]]

class Mat4 {
    public: 
        Mat4();
        Mat4(std::array<float, 16> matrix);
        ~Mat4() = default;
        Mat4 operator*(const Mat4 &other) const;
        Mat4 operator*=(const Mat4 &other);
        Mateyak::Vec3f operator*(const Mateyak::Vec3f &vec) const;
        float &operator[](int index);
        float operator[](int index) const;
        float &operator()(int x, int y);
        float operator()(int x, int y) const;
        std::array<float, 4> operator*(const std::array<float, 4> &vec) const;
        Mat4 inverse() const;

        static Mat4 translate3D(const Mateyak::Vec3f &vec);
        static Mat4 scale3D(const Mateyak::Vec3f &vec);
        static Mat4 rotate3D(char axis, float angle);
        static Mateyak::Vec3f vecRotate(Mateyak::Vec3f vec, Mateyak::Vec3f dir, Mateyak::Vec3f pos);

    protected:
    private:
        std::array<float, 16> _matrix;
};
