#include <math.h>
#include "venom.hpp"
#define FLOAT_NB (float)circlePerLeg
#define DIS 3.f
#define DIS2 2.5f
#include "Utils3d.hpp"
#include "Mateyak/Vector.hpp"
#include "Perlin/Perlin.hpp"

int Venom::nbLegs = 5;
int Venom::circlePerLeg = 30;
int Venom::pointPerCircle = 4;

Venom::Venom(Mateyak::Vec2f pos, Mateyak::Vec2f mapSize): mapSize(mapSize)
{
    for (int i = 0; i < 5000; i++) {
        Mateyak::Vec3f ps;
        float x = GetRandomValue(0, 100) / 100.0;
        float y = GetRandomValue(0, 100) / 100.0;
        ps.x = x + i % 100;
        ps.z = (y + i / 100) * 2;
        ps.y = 0;
        pos_feet.push_back(ps);
    }
    _pos = {(pos.x * 10 + 5) / 3.F, 0.75, (pos.y * 10 + 5) / 3.F};
    _nextPosition = _pos;
}

Venom::~Venom()
{
    UnloadTexture(this->texture);
}

void Venom::Draw_leg(Mateyak::Vec3f leg, int seed)
{
    Mateyak::Vec3f vec(_pos.x - leg.x, 0, _pos.z - leg.z);
    float len = 100 - (c_pos - leg).len();
    vec = -vec;
    int nb = circlePerLeg;
    std::vector<Mateyak::Vec3f> center;
    std::vector<Mateyak::Vec3f> angles;
    center.reserve(nb + 1);
    angles.reserve(nb + 1);
    float dis = (_pos - leg).len();
    vec.x /= FLOAT_NB;
    vec.z /= FLOAT_NB;
    if (dis > DIS2)
        nb = circlePerLeg * (1 - (dis - DIS2) / (DIS - DIS2));
    Mateyak::Vec3f last = _pos;
    last.y = sinf((-1 / FLOAT_NB * PI * 4 + PI) / 5.0);
    Mateyak::Vec3f tmp = _pos;
    for (int i = 0; i <= nb; i++) {
        tmp += vec;
        tmp.y = sinf((i / FLOAT_NB * PI * 4 + PI) / 5.0f);

        center.push_back(tmp);
        angles.push_back(tmp - last);
        last = tmp;
    }

    if (center.size() <= static_cast<size_t>(pointPerCircle))
        return;

    std::vector<Mateyak::Vec3f> points;
    points.reserve(pointPerCircle * center.size());

    for (int h = 0; h < nbLegs; h++) {
        points.clear();
        Mateyak::Vec3f noise = {0, 0, 0};

        for (size_t i = 0; i < center.size(); i++) {
            noise.x = Perlin::Noise2D(seed * h, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0) / 2;
            noise.z = Perlin::Noise2D(seed * h * 2, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0) / 2;
            Utils::generateCirclePoints(center[i] + noise, angles[i], ((nb - i) / FLOAT_NB) / ((h - 1) * 3), pointPerCircle, points);
        }
        std::vector<Mateyak::Triangle> triangles = Utils::connectPointsWithTriangles(points, pointPerCircle);
        if (triangles.empty())
            return;
        for (size_t i = 0; i < triangles.size(); i++) {
            Mateyak::Triangle tr = triangles[i];
            float clr_mul = (float)i / (float)triangles.size();
            Mateyak::Window::draw(tr, Color{u_char(80 - (clr_mul * 80)), 0, 0, (unsigned char)(2.55 * len)});
        }
    }
}

void Venom::move_ven(Camera camera)
{
    Mateyak::Vec3f vec;

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
        vec = {camera.position.x - camera.target.x, 0, camera.position.z - camera.target.z};
        vec = vec.Normalize();
        vec = vec / 10.0;
    }
    if (IsKeyDown(KEY_UP)) {
        _pos.x -= vec.x;
        _pos.z -= vec.z;
    }
    if (IsKeyDown(KEY_DOWN)) {
        _pos.x += vec.x;
        _pos.z += vec.z;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        _pos.x += vec.z;
        _pos.z -= vec.x;
    }
    if (IsKeyDown(KEY_LEFT)) {
        _pos.x -= vec.z;
        _pos.z += vec.x;
    }
}

void Venom::move_ven()
{
    float norm = 0;
    Vector3 vec;

    if (_pos == _nextPosition) {
        return;
    }
    vec = {_nextPosition.x - _pos.x, _pos.y, _nextPosition.z - _pos.z};
    if (std::abs(vec.x) > 3.334 || std::abs(vec.z) > 3.334) {
        vec = {-vec.x, vec.y, -vec.z};
    }
    norm = sqrt(pow(vec.x, 2) + pow(vec.z, 2));

    if (norm < 0.1) {
        _pos = _nextPosition;
    } else {
        _pos.x += vec.x / norm / 20.f;
        _pos.z += vec.z / norm / 20.f;
    }
    if (_pos.x > mapSize.x * 10 / 3) {
        _pos.x -= mapSize.x * 10 / 3;
    }
    if (_pos.x < 0) {
        _pos.x += mapSize.x * 10 / 3;
    }
    if (_pos.z > mapSize.y * 10 / 3) {
        _pos.z -= mapSize.y * 10 / 3;
    }
    if (_pos.z < 0) {
        _pos.z += mapSize.y * 10 / 3;
    }
}

void Venom::draw_ven(int seed, const Mateyak::Camera& camera)
{
    c_pos = camera._position;
    for (int i = 0; i < 5000; i++) {
        time = GetTime();
        if ((_pos - pos_feet[i]).len() < DIS && (c_pos - pos_feet[i]).len() < 100) {
            if (Utils::differenceAngle((c_pos - camera._target).Normalize(), (c_pos - pos_feet[i]).Normalize()) > 45)
                continue;
            Draw_leg(pos_feet[i], seed + i * 1000);
        }
    }
}

Mateyak::Vec3f Venom::getPos() const
{
    return _pos;
}

Mateyak::Vec3f &Venom::getPosition()
{
    return _pos;
}

void Venom::setPos(const Mateyak::Vec3f &pos)
{
    _nextPosition = {(pos.x * 10 + 5) / 3.F, _pos.y, (pos.y * 10 + 5) / 3.F};
}

void Venom::fpsHandler() {
    int fps = GetFPS();
    if (fps < 60) {
        if (nbLegs > 1) {
            nbLegs--;
        } else if (pointPerCircle > 3) {
            pointPerCircle--;
        } else if (circlePerLeg > 6) {
            circlePerLeg--;
        }
    }
    if (fps > 60) {
        if (circlePerLeg < 15) {
            circlePerLeg++;
        } else if (nbLegs < 3) {
            nbLegs++;
        } else if (pointPerCircle < 4) {
            pointPerCircle++;
        } else if (circlePerLeg < 30) {
            circlePerLeg++;
        } if (nbLegs < 5) {
            nbLegs++;
        }
    }
}
