/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Model3D
*/

//
// Created by Edo on 6/5/23.
//

#pragma once
#include <raylib.h>
#include "Mateyak/Sprite.hpp"
#include "Mateyak/Shaders.hpp"

namespace Mateyak {
    class Model3D {
    public:
        Model3D(Mesh mesh, Vec3f pos = {0, 0, 0}, float scale = 1, Color color = WHITE);
        Model3D(const std::string &path, Vec3f pos = {0, 0, 0}, float scale = 1, Color color = WHITE);
        Model3D();
        ~Model3D();

        void operator=(Mesh mesh);
        void setTexture(const Sprite &texture);
        void setShader(const Shaders &shader);
        void setScale(float scale);
        void setPos(const Vec3f &pos);
        void setColor(const Color &color);

        void unsetShader();

        float getScale() const { return _scale; }
        Vec3f getPos() const { return _pos; }
        Color getColor() const { return _color; }

        operator Model() const;
    private:
        Model _model;
        Vec3f _pos;
        float _scale;
        Color _color;
    };
}