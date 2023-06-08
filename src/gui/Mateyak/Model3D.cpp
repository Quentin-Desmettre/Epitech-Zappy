/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Model3D
*/

//
// Created by Edo on 6/5/23.
//

#include "Mateyak/Model3D.hpp"

Mateyak::Model3D::Model3D(Mesh mesh, Vec3f pos, float scale, Color color)
    : _pos(pos), _scale(scale), _color(color)
{
    _model = LoadModelFromMesh(mesh);
}

void Mateyak::Model3D::operator=(Mesh mesh)
{
    _model = LoadModelFromMesh(mesh);
}

Mateyak::Model3D::Model3D(const std::string &path, Vec3f pos, float scale, Color color)
    : _pos(pos), _scale(scale), _color(color)
{
    _model = LoadModel(path.c_str());
}

Mateyak::Model3D::Model3D(): _scale(1), _color(WHITE)
{
    _pos = {0, 0, 0};
    _model = LoadModelFromMesh(GenMeshCube(1, 1, 1));
}

Mateyak::Model3D::~Model3D()
{
    UnloadModel(_model);
}

void Mateyak::Model3D::setTexture(const Mateyak::Sprite &texture)
{
    _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

void Mateyak::Model3D::setShader(const Mateyak::Shaders &shader)
{
    _model.materials[0].shader = shader;
}

void Mateyak::Model3D::setScale(float scale)
{
    _scale = scale;
}

void Mateyak::Model3D::setPos(const Mateyak::Vec3f &pos)
{
    _pos = pos;
}

void Mateyak::Model3D::setColor(const Color &color)
{
    _color = color;
}

void Mateyak::Model3D::unsetShader()
{
    _model.materials[0].shader = {0};
}

Mateyak::Model3D::operator Model() const
{
    return _model;
}