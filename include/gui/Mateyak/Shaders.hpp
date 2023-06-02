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

namespace Mateyak {
    class Shaders {
        public:
            Shaders(const char *vertexShader, const char *fragmentShader);
            ~Shaders();
        private:
            Shader _shader;
            unsigned int _id;

    };
}

