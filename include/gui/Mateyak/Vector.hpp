/*
** EPITECH PROJECT, 2023
** venom
** File description:
** window
*/

#pragma once

#include <raylib.h>
#include <string>
#include <raymath.h>

namespace Mateyak 
{

    template<typename T>
    class Vec3 {
        public:
            Vec3() {
                x = 0;
                y = 0;
                z = 0;
            }
            Vec3(Vector3 vec) {
                x = vec.x;
                y = vec.y;
                z = vec.z;
            }
            Vec3(T x, T y, T z) {
                this->x = x;
                this->y = y;
                this->z = z;
            }
            Vec3(const Vec3<T> &vec) {
                x = (T)vec.x;
                y = (T)vec.y;
                z = (T)vec.z;
            }
            Vec3 Normalize() const {
                float length = sqrt(x * x + y * y + z * z);
                return Vec3<T>{x / length, y / length, z / length};
            }

            Vec3 operator+(Vec3 vec) const {
                return Vec3<T> {x + vec.x, y + vec.y, z + vec.z};
            }
            void operator+=(Vec3 vec) {
                x += vec.x;
                y += vec.y;
                z += vec.z;
            }
            void operator-=(Vec3 vec) {
                x -= vec.x;
                y -= vec.y;
                z -= vec.z;
            }
            void operator=(const Vec3 &vec) {
                x = vec.x;
                y = vec.y;
                z = vec.z;
            }
            bool operator==(Vec3 vec) const {
                return (x == vec.x && y == vec.y && z == vec.z);
            }
            bool operator!=(Vec3 vec) const {
                return (x != vec.x || y != vec.y || z != vec.z);
            }
            Vec3 operator-(Vec3 vec) const {
                return Vec3<T> {x - vec.x, y - vec.y, z - vec.z};
            }
            Vec3 operator-() const {
                return Vec3<T> {-x, -y, -z};
            }
            Vec3 operator*(Vec3 vec) const {
                return Vec3<T> {x * vec.x, y * vec.y, z * vec.z};
            }
            Vec3 operator/(Vec3 vec) const {
                return Vec3<T> {x / vec.x, y / vec.y, z / vec.z};
            }

            template<typename U>
            Vec3 operator*(U scalar) const {
                return Vec3<T> {x * (T)scalar, y * (T)scalar, z * (T)scalar};
            }
            template<typename U>
            Vec3 operator/(U scalar) const {
                return Vec3<T> {x / (T)scalar, y / (T)scalar, z / (T)scalar};
            }            
            ~Vec3() = default;


            Vec3 CrossProduct(Vec3 b) const {
                return Vec3<T> {
                    y * b.z - z * b.y,
                    z * b.x - x * b.z,
                    x * b.y - y * b.z
                };
            }
            Vector3 getRayVec3() const {
                return Vector3 {(float)x, (float)y, (float)z};
            }
            operator Vector3() const {
                return Vector3 {(float)x, (float)y, (float)z};
            }

            float len() const {
                return sqrt(x * x + y * y + z * z);
            }
            float dif() const {
                return (abs(x) + abs(y) + abs(z)) / 3.0;
            }

            T x;
            T y;
            T z;

        private:
    };

    typedef Vec3<float> Vec3f;
}