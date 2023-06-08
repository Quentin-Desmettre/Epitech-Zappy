#include <math.h>
#include "venom.hpp"
#define BALL_NB 30
#define FLOAT_NB (float)BALL_NB
#define DIS 3.f
#define DIS2 2.5f
#define PRECI 4
#define LEG 5
#include "Utils3d.hpp"
#include "Mateyak/Vector.hpp"
#include "Perlin/Perlin.hpp"

Venom::Venom(Mateyak::Vec2f pos)
{
    for (int i = 0; i < 5000; i++) {
        Mateyak::Vec3f ps;
        float x = GetRandomValue(0, 100) / 100.0;
        float y = GetRandomValue(0, 100) / 100.0;
        ps.x = x + i % 100 - 50;
        ps.z = (y + i / 100 - 50) * 2 + 50;
        ps.y = 0;
        pos_feet.push_back(ps);
    }
    _pos = {pos.x + 5/3.f, 0.75, pos.y + 5/3.f};
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
    int nb = BALL_NB;
    std::vector<Mateyak::Vec3f> center;
    std::vector<Mateyak::Vec3f> angles;
    center.reserve(nb + 1);
    angles.reserve(nb + 1);
    float dis = (_pos - leg).len();
    vec.x /= FLOAT_NB;
    vec.z /= FLOAT_NB;
    if (dis > DIS2)
        nb = BALL_NB * (1 - (dis - DIS2) / (DIS - DIS2));
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

    if (center.size() <= PRECI)
        return;

    std::vector<Mateyak::Vec3f> points;
    points.reserve(PRECI * center.size());

    for (int h = 0; h < LEG; h++) {
        points.clear();
        Mateyak::Vec3f noise = {0, 0, 0};

        for (size_t i = 0; i < center.size(); i++) {
            noise.x = Perlin::Noise2D(seed * h, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0) / 2;
            noise.z = Perlin::Noise2D(seed * h * 2, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0) / 2;
            Utils::generateCirclePoints(center[i] + noise, angles[i], ((nb - i) / FLOAT_NB) / ((h - 1) * 3), PRECI, points);
        }
        std::vector<Mateyak::Triangle> triangles = Utils::connectPointsWithTriangles(points, PRECI);
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
    float norm = 0;
    Vector3 vec;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
        vec = {camera.position.x - camera.target.x, 0, camera.position.z - camera.target.z};
        norm = sqrt(pow(vec.x, 2) + pow(vec.z, 2));
    }
    if (IsKeyDown(KEY_UP)) {
        _pos.x -= vec.x / norm / 20.0;
        _pos.z -= vec.z / norm / 20.0;
    }
    if (IsKeyDown(KEY_DOWN)) {
        _pos.x += vec.x / norm / 20.0;
        _pos.z += vec.z / norm / 20.0;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        _pos.x += vec.z / norm / 20.0;
        _pos.z -= vec.x / norm / 20.0;
    }
    if (IsKeyDown(KEY_LEFT)) {
        _pos.x -= vec.z / norm / 20.0;
        _pos.z += vec.x / norm / 20.0;
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
