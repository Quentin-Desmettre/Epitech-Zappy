/*
** EPITECH PROJECT, 2023
** venom
** File description:
** Camera
*/

#pragma once

#include "Vector.hpp"
#include "Vector2.hpp"
#include "Matrix.hpp"

namespace Mateyak
{
    class Camera {
        public:
            Camera(Vec3f position = {0, 0, 0}, Vec3f target = {1, 1, 1}, Vec3f up = {0, 1, 0}, float fov = 90, int mode = CAMERA_CUSTOM);
            ~Camera() = default;

            void Update();
            Camera3D getRayCam() const;
            void findClickPos(const Mateyak::Vec3f &dir);
            void RecalculateCamTarget(Mateyak::Vec2f mouseOffset);
            void RecalculateCamPos();

            Vec3f _position;
            Vec3f _target;
            Vec3f _up;
            float _fov;
            int _type;
            Camera3D _cam;
            int _state;
            Vec2f _lastClickPos {-1, -1};
            Vec3f _lastClickDir {-1, -1, -1};
            int lastSelectedPlayer = -1;

        private:
            Vec2f _oldCamPos;
            Vec2f _camPos;
            
    };
} // namespace Mateyak

