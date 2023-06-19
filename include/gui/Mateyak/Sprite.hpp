/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Sprite
*/

//
// Created by Edo on 6/2/23.
//

#pragma once
#include <raylib.h>
#include "Mateyak/Vector2.hpp"
#include <string>

namespace Mateyak
{
    class Sprite {
        public:
            Sprite(Mateyak::Vec2f size, int format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, void *data = nullptr, Mateyak::Vec2f position = {0, 0}, Color color = WHITE);
            Sprite(const std::string &path, Mateyak::Vec2f position = {0, 0}, Color color = WHITE);
            ~Sprite() = default;

            void setData(void *data);
            void setPosition(Mateyak::Vec2f position);
            void setColor(Color color);
            void setSize(Mateyak::Vec2f size);
            void setFormat(int format);

            void *getData() const;
            Mateyak::Vec2f getPosition() const;
            Color getColor() const;
            Mateyak::Vec2f getSize() const;
            int getFormat() const;

            operator Texture2D() const;
            operator Image() const;
        private:
            Image _image;
            Mateyak::Vec2f _position;
            Color _color;

    };
} // namespace Mateyak

