/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Shader
*/

//
// Created by Edo on 5/31/23.
//

#pragma once

#include <raylib.h>
#include <raymath.h>
#include <string>
#include "Mateyak/rlights.hpp"
#include "Mateyak/Vector2.hpp"
#include "Mateyak/Vector.hpp"

namespace Mateyak {
    class Shaders {
        public:
            Shaders(const std::string &ertexShader, const std::string &fragmentShader);
            ~Shaders();
            int getUniformLocation(const std::string &uniformName);
            void setUniform(const std::string &uniformName, int value);
            void setUniform(const std::string &uniformName, float value);
            void setUniform(const std::string &uniformName, const Mateyak::Vec2f &value);
            void setUniform(const std::string &uniformName, const Mateyak::Vec3f &value);
            void setUniform(int loc, const Mateyak::Vec3f &value);

            operator Shader() const;
        private:
            Shader _shader;

    };
}

