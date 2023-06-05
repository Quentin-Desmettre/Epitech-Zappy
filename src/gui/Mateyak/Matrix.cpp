/*
** EPITECH PROJECT, 2023
** Epitech-Raytracer
** File description:
** Matrix
*/

#include "Mateyak/Matrix.hpp"
#include <stdexcept>

Mat4::Mat4()
{
    _matrix = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

Mat4::Mat4(std::array<float, 16> matrix)
{
    _matrix = matrix;
}

float &Mat4::operator[](int index)
{
    return _matrix[index];
}

float Mat4::operator[](int index) const
{
    return _matrix[index];
}

float &Mat4::operator()(int x, int y)
{
    return _matrix[x * 4 + y];
}

float Mat4::operator()(int x, int y) const
{
    return _matrix[x * 4 + y];
}

Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 result;
    float sum = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sum = 0;
            for (int k = 0; k < 4; k++)
                sum += _matrix[i * 4 + k] * other._matrix[k * 4 + j];
            result._matrix[i * 4 + j] = sum;
        }
    }
    return result;
}

Mateyak::Vec3f Mat4::operator*(const Mateyak::Vec3f &vec) const
{
    Mateyak::Vec3f result;

    result.x = _matrix[0] * vec.x + _matrix[1] * vec.y + _matrix[2] * vec.z + _matrix[3];
    result.y = _matrix[4] * vec.x + _matrix[5] * vec.y + _matrix[6] * vec.z + _matrix[7];
    result.z = _matrix[8] * vec.x + _matrix[9] * vec.y + _matrix[10] * vec.z + _matrix[11];
    return result;
}

Mat4 Mat4::operator*=(const Mat4 &other)
{
    *this = *this * other;
    return *this;
}

std::array<float, 4> Mat4::operator*(const std::array<float, 4> &vec) const
{
    std::array<float, 4> result;

    for (int i = 0; i < 4; i++) {
        result[i] = 0;
        for (int j = 0; j < 4; j++)
            result[i] += _matrix[i * 4 + j] * vec[j];
    }
    return result;
}

Mat4 Mat4::inverse() const
{
    Mat4 inv;
    float det;
    int i;

    inv[0] = _matrix[5]  * _matrix[10] * _matrix[15] -
             _matrix[5]  * _matrix[11] * _matrix[14] -
             _matrix[9]  * _matrix[6]  * _matrix[15] +
             _matrix[9]  * _matrix[7]  * _matrix[14] +
             _matrix[13] * _matrix[6]  * _matrix[11] -
             _matrix[13] * _matrix[7]  * _matrix[10];

    inv[4] = -_matrix[4]  * _matrix[10] * _matrix[15] +
             _matrix[4]  * _matrix[11] * _matrix[14] +
             _matrix[8]  * _matrix[6]  * _matrix[15] -
             _matrix[8]  * _matrix[7]  * _matrix[14] -
             _matrix[12] * _matrix[6]  * _matrix[11] +
             _matrix[12] * _matrix[7]  * _matrix[10];

    inv[8] = _matrix[4]  * _matrix[9] * _matrix[15] -
             _matrix[4]  * _matrix[11] * _matrix[13] -
             _matrix[8]  * _matrix[5] * _matrix[15] +
             _matrix[8]  * _matrix[7] * _matrix[13] +
             _matrix[12] * _matrix[5] * _matrix[11] -
             _matrix[12] * _matrix[7] * _matrix[9];

    inv[12] = -_matrix[4]  * _matrix[9] * _matrix[14] +
              _matrix[4]  * _matrix[10] * _matrix[13] +
              _matrix[8]  * _matrix[5] * _matrix[14] -
              _matrix[8]  * _matrix[6] * _matrix[13] -
              _matrix[12] * _matrix[5] * _matrix[10] +
              _matrix[12] * _matrix[6] * _matrix[9];

    inv[1] = -_matrix[1]  * _matrix[10] * _matrix[15] +
             _matrix[1]  * _matrix[11] * _matrix[14] +
             _matrix[9]  * _matrix[2] * _matrix[15] -
             _matrix[9]  * _matrix[3] * _matrix[14] -
             _matrix[13] * _matrix[2] * _matrix[11] +
             _matrix[13] * _matrix[3] * _matrix[10];

    inv[5] = _matrix[0]  * _matrix[10] * _matrix[15] -
             _matrix[0]  * _matrix[11] * _matrix[14] -
             _matrix[8]  * _matrix[2] * _matrix[15] +
             _matrix[8]  * _matrix[3] * _matrix[14] +
             _matrix[12] * _matrix[2] * _matrix[11] -
             _matrix[12] * _matrix[3] * _matrix[10];

    inv[9] = -_matrix[0]  * _matrix[9] * _matrix[15] +
             _matrix[0]  * _matrix[11] * _matrix[13] +
             _matrix[8]  * _matrix[1] * _matrix[15] -
             _matrix[8]  * _matrix[3] * _matrix[13] -
             _matrix[12] * _matrix[1] * _matrix[11] +
             _matrix[12] * _matrix[3] * _matrix[9];

    inv[13] = _matrix[0]  * _matrix[9] * _matrix[14] -
              _matrix[0]  * _matrix[10] * _matrix[13] -
              _matrix[8]  * _matrix[1] * _matrix[14] +
              _matrix[8]  * _matrix[2] * _matrix[13] +
              _matrix[12] * _matrix[1] * _matrix[10] -
              _matrix[12] * _matrix[2] * _matrix[9];

    inv[2] = _matrix[1]  * _matrix[6] * _matrix[15] -
             _matrix[1]  * _matrix[7] * _matrix[14] -
             _matrix[5]  * _matrix[2] * _matrix[15] +
             _matrix[5]  * _matrix[3] * _matrix[14] +
             _matrix[13] * _matrix[2] * _matrix[7] -
             _matrix[13] * _matrix[3] * _matrix[6];

    inv[6] = -_matrix[0]  * _matrix[6] * _matrix[15] +
             _matrix[0]  * _matrix[7] * _matrix[14] +
             _matrix[4]  * _matrix[2] * _matrix[15] -
             _matrix[4]  * _matrix[3] * _matrix[14] -
             _matrix[12] * _matrix[2] * _matrix[7] +
             _matrix[12] * _matrix[3] * _matrix[6];

    inv[10] = _matrix[0]  * _matrix[5] * _matrix[15] -
              _matrix[0]  * _matrix[7] * _matrix[13] -
              _matrix[4]  * _matrix[1] * _matrix[15] +
              _matrix[4]  * _matrix[3] * _matrix[13] +
              _matrix[12] * _matrix[1] * _matrix[7] -
              _matrix[12] * _matrix[3] * _matrix[5];

    inv[14] = -_matrix[0]  * _matrix[5] * _matrix[14] +
              _matrix[0]  * _matrix[6] * _matrix[13] +
              _matrix[4]  * _matrix[1] * _matrix[14] -
              _matrix[4]  * _matrix[2] * _matrix[13] -
              _matrix[12] * _matrix[1] * _matrix[6] +
              _matrix[12] * _matrix[2] * _matrix[5];

    inv[3] = -_matrix[1] * _matrix[6] * _matrix[11] +
             _matrix[1] * _matrix[7] * _matrix[10] +
             _matrix[5] * _matrix[2] * _matrix[11] -
             _matrix[5] * _matrix[3] * _matrix[10] -
             _matrix[9] * _matrix[2] * _matrix[7] +
             _matrix[9] * _matrix[3] * _matrix[6];

    inv[7] = _matrix[0] * _matrix[6] * _matrix[11] -
             _matrix[0] * _matrix[7] * _matrix[10] -
             _matrix[4] * _matrix[2] * _matrix[11] +
             _matrix[4] * _matrix[3] * _matrix[10] +
             _matrix[8] * _matrix[2] * _matrix[7] -
             _matrix[8] * _matrix[3] * _matrix[6];

    inv[11] = -_matrix[0] * _matrix[5] * _matrix[11] +
              _matrix[0] * _matrix[7] * _matrix[9] +
              _matrix[4] * _matrix[1] * _matrix[11] -
              _matrix[4] * _matrix[3] * _matrix[9] -
              _matrix[8] * _matrix[1] * _matrix[7] +
              _matrix[8] * _matrix[3] * _matrix[5];

    inv[15] = _matrix[0] * _matrix[5] * _matrix[10] -
              _matrix[0] * _matrix[6] * _matrix[9] -
              _matrix[4] * _matrix[1] * _matrix[10] +
              _matrix[4] * _matrix[2] * _matrix[9] +
              _matrix[8] * _matrix[1] * _matrix[6] -
              _matrix[8] * _matrix[2] * _matrix[5];

    det = _matrix[0] * inv[0] + _matrix[1] * inv[4] + _matrix[2] * inv[8] + _matrix[3] * inv[12];

    if (det == 0)
        return {};

    det = 1.0f / det;
    for (i = 0; i < 16; i++)
        inv[i] *= det;
    return inv;
}

Mateyak::Vec3f Mat4::vecRotate(Mateyak::Vec3f vec, Mateyak::Vec3f dir, unused Mateyak::Vec3f pos)
{
    Mat4 mat3 = Mat4({
        1, 0, 0, 0,
        0, cosf(dir.x), -sinf(dir.x), 0,
        0, sinf(dir.x), cosf(dir.x), 0,
        0, 0, 0, 1
    });
    Mat4 mat4 = Mat4({
        cosf(dir.y), 0, sinf(dir.y), 0,
        0, 1, 0, 0,
        -sinf(dir.y), 0, cosf(dir.y), 0,
        0, 0, 0, 1
    });
    Mat4 mat5 = Mat4({
        cosf(dir.z), -sinf(dir.z), 0, 0,
        sinf(dir.z), cosf(dir.z), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    });
    return mat5 * mat4 * mat3 * vec;
}

Mat4 Mat4::translate3D(const Mateyak::Vec3f &vec)
{
    return Mat4({
        1, 0, 0, vec.x,
        0, 1, 0, vec.y,
        0, 0, 1, vec.z,
        0, 0, 0, 1
    });
}

Mat4 Mat4::scale3D(const Mateyak::Vec3f &vec)
{
    return Mat4({
        vec.x, 0, 0, 0,
        0, vec.y, 0, 0,
        0, 0, vec.z, 0,
        0, 0, 0, 1
    });
}

Mat4 Mat4::rotate3D(char axis, float angle)
{
    switch (axis) {
        case 'x':
            return Mat4({
                1, 0, 0, 0,
                0, cosf(angle), -sinf(angle), 0,
                0, sinf(angle), cosf(angle), 0,
                0, 0, 0, 1
            });
        case 'y':
            return Mat4({
                cosf(angle), 0, sinf(angle), 0,
                0, 1, 0, 0,
                -sinf(angle), 0, cosf(angle), 0,
                0, 0, 0, 1
            });
        case 'z':
            return Mat4({
                cosf(angle), -sinf(angle), 0, 0,
                sinf(angle), cosf(angle), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            });
        default:
            throw std::invalid_argument("Invalid axis");
    }
}
