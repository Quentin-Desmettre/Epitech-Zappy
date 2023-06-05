/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Shader
*/

//
// Created by Edo on 5/31/23.
//

#include "Mateyak/Shaders.hpp"

Mateyak::Shaders::Shaders(const std::string &vertexShader, const std::string &fragmentShader)
{
    _shader = LoadShader(vertexShader.c_str(), fragmentShader.c_str());
}

Mateyak::Shaders::~Shaders()
{
    UnloadShader(_shader);
}

int Mateyak::Shaders::getUniformLocation(const std::string &uniformName)
{
    return GetShaderLocation(_shader, uniformName.c_str());
}

void Mateyak::Shaders::setUniform(const std::string &uniformName, int value)
{
    SetShaderValue(_shader, getUniformLocation(uniformName), &value, SHADER_UNIFORM_INT);
}

void Mateyak::Shaders::setUniform(const std::string &uniformName, float value)
{
    SetShaderValue(_shader, getUniformLocation(uniformName), &value, SHADER_UNIFORM_FLOAT);
}

void Mateyak::Shaders::setUniform(const std::string &uniformName, const Mateyak::Vec2f &value)
{
    float tmp[2] = {value.x, value.y};
    SetShaderValue(_shader, getUniformLocation(uniformName), tmp, SHADER_UNIFORM_VEC2);
}

void Mateyak::Shaders::setUniform(const std::string &uniformName, const Mateyak::Vec3f &value)
{
    float tmp[3] = {value.x, value.y, value.z};
    SetShaderValue(_shader, getUniformLocation(uniformName), tmp, SHADER_UNIFORM_VEC3);
}

void Mateyak::Shaders::setUniform(int loc, const Mateyak::Vec3f &value)
{
    float tmp[3] = {value.x, value.y, value.z};
    SetShaderValue(_shader, loc, tmp, SHADER_UNIFORM_VEC3);
}

Mateyak::Shaders::operator Shader() const
{
    return _shader;
}