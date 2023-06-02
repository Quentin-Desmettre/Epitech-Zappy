/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Sprite
*/

//
// Created by Edo on 6/2/23.
//

#include "Mateyak/Sprite.hpp"

Mateyak::Sprite::Sprite(Mateyak::Vec2f size, int format, void *data, Mateyak::Vec2f position, Color color)
{
    _image.width = size.x;
    _image.height = size.y;
    _image.format = format;
    _image.data = data;
    _image.mipmaps = true;

    _position = position;
    _color = color;
}

Mateyak::Sprite::operator Texture2D() const
{
    return LoadTextureFromImage(_image);
}

Mateyak::Sprite::operator Image() const
{
    return _image;
}

void Mateyak::Sprite::setData(void *data)
{
    _image.data = data;
}

void Mateyak::Sprite::setPosition(Mateyak::Vec2f position)
{
    _position = position;
}

void Mateyak::Sprite::setColor(Color color)
{
    _color = color;
}

void Mateyak::Sprite::setSize(Mateyak::Vec2f size)
{
    _image.width = size.x;
    _image.height = size.y;
}

void Mateyak::Sprite::setFormat(int format)
{
    _image.format = format;
}

void *Mateyak::Sprite::getData() const
{
    return _image.data;
}

Mateyak::Vec2f Mateyak::Sprite::getPosition() const
{
    return _position;
}

Color Mateyak::Sprite::getColor() const
{
    return _color;
}

Mateyak::Vec2f Mateyak::Sprite::getSize() const
{
    return {static_cast<float>(_image.width), static_cast<float>(_image.width)};
}

int Mateyak::Sprite::getFormat() const
{
    return _image.format;
}

Mateyak::Sprite::Sprite(const std::string &path, Mateyak::Vec2f position, Color color)
{
    _image = LoadImage(path.c_str());
    _position = position;
    _color = color;
}
