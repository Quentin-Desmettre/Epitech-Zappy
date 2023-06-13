/*
** EPITECH PROJECT, 2023
** venom
** File description:
** Camera
*/

#include "Mateyak/Camera.hpp"
#include <raymath.h>
#include <iostream>
#include "Mateyak/Window.hpp"

Mateyak::Camera::Camera(Vec3f position, Vec3f target, Vec3f up, float fov, int mode) : _position(position), _target(target), _up(up), _fov(fov), _type(mode)
{
    _cam.target = _target;
    _cam.position = _position;
    _cam.up = _up;
    _cam.fovy = _fov;
    _cam.projection = CAMERA_PERSPECTIVE;
    _state = 1;

    _oldCamPos = {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())};
    DisableCursor();
}

Matrix ConvertVectorToMatrix(const Vector3& vector) {
    Matrix matrix = MatrixIdentity();
    matrix.m0 = vector.x;
    matrix.m5 = vector.y;
    matrix.m10 = vector.z;
    return matrix;
}

void Mateyak::Camera::RecalculateCamTarget(Mateyak::Vec2f mouseOffset) {
    Mat4 rotateY = Mat4::rotate3D('y', -mouseOffset.x * 0.01);
    Mateyak::Vec3f trget = _target - _position;
    trget = rotateY * trget;
    trget = trget.Normalize();
    trget.y += -mouseOffset.y * 0.01;
    if (trget.y > 0.99)
        trget.y = 0.99;
    if (trget.y < -0.99)
        trget.y = -0.99;
    trget = trget.Normalize();
    _target = _position + trget;
    _cam.target = _target;
}

void Mateyak::Camera::Update()
{
    if (IsKeyPressed(KEY_F3)) {
        _state = !_state;
        if (_state == 1)
            DisableCursor();
        else
            EnableCursor();
    }

    Vector2 tmp = GetMousePosition();
    _camPos = {tmp.x, tmp.y};
    Vec2f mouseOffset = _camPos - _oldCamPos;
    const float mouseSensitivity = 0.1f;
    mouseOffset = mouseOffset * mouseSensitivity;

    if (_state == 1) {
        RecalculateCamTarget(mouseOffset);
        RecalculateCamPos();
    }
    _oldCamPos = _camPos;
}

Camera3D Mateyak::Camera::getRayCam() const
{
    return _cam;
}

void Mateyak::Camera::RecalculateCamPos()
{
    Vec3f forward = _target;
    _target -= _position;
    Mateyak::Vec3f dir = (forward - _position).Normalize() * Mateyak::Window::timePass * 10.0;
    Mateyak::Vec3f right = (dir).CrossProduct(_up).Normalize() * Mateyak::Window::timePass * 10.0;

    if (IsKeyDown(KEY_W))
        _position += dir;
    if (IsKeyDown(KEY_S)) {
        _position -= dir;
    }
    if (IsKeyDown(KEY_D)) {
        _position += right;
    }
    if (IsKeyDown(KEY_A))
        _position -= right;
    if (IsKeyDown(KEY_SPACE))
        _position += _up * Mateyak::Window::timePass * 10.0;
    if (IsKeyDown(KEY_LEFT_SHIFT))
        _position -= _up * Mateyak::Window::timePass * 10.0;
    
    _target += _position;
    _cam.target = _target;
    _cam.position = _position;
}
