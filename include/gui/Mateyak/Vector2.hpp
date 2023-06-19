#pragma once

#include <raylib.h>
#include <string>
#include <raymath.h>

namespace Mateyak 
{
    template<typename T>
    class Vec2 {
    public:
        Vec2() {
            x = 0;
            y = 0;
        }
        Vec2(Vector2 vec) {
            x = vec.x;
            y = vec.y;
        }
        Vec2(T x, T y) {
            this->x = x;
            this->y = y;
        }
        Vec2(const Vec2<T> &vec) {
            x = vec.x;
            y = vec.y;
        }
        Vec2 Normalize() const {
            float length = sqrt(x * x + y * y);
            return Vec2<T>{x / length, y / length};
        }

        Vec2 operator+(Vec2 vec) const {
            return Vec2<T> {x + vec.x, y + vec.y};
        }
        void operator+=(Vec2 vec) {
            x += vec.x;
            y += vec.y;
        }
        void operator-=(Vec2 vec) {
            x -= vec.x;
            y -= vec.y;
        }
        void operator=(const Vec2 &vec) {
            x = vec.x;
            y = vec.y;
        }
        bool operator==(Vec2 vec) const {
            return (x == vec.x && y == vec.y);
        }
        bool operator!=(Vec2 vec) const {
            return (x != vec.x || y != vec.y);
        }
        Vec2 operator-(Vec2 vec) const {
            return Vec2<T> {x - vec.x, y - vec.y};
        }
        Vec2 operator*(Vec2 vec) const {
            return Vec2<T> {x * vec.x, y * vec.y};
        }
        Vec2 operator/(Vec2 vec) const {
            return Vec2<T> {x / vec.x, y / vec.y};
        }

        template<typename U>
        Vec2 operator*(U scalar) const {
            return Vec2<T> {x * scalar, y * scalar};
        }
        template<typename U>
        Vec2 operator/(U scalar) const {
            return Vec2<T> {x / scalar, y / scalar};
        }            
        ~Vec2() = default;
        Vector2 getRayVec2() const {
            return Vector2 {(float)x, (float)y};
        }

        float len() {
            return sqrt(x * x + y * y);
        }

        T x;
        T y;
    };

    typedef Vec2<float> Vec2f;
}
