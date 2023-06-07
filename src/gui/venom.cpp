#include <math.h>
#include "venom.hpp"
#define BALL_NB 6
#define FLOAT_NB (float)BALL_NB
#define DIS 3.f
#define DIS2 2.5f
#define PRECI 3
#define LEG 1
#include "Utils3d.hpp"
#include "Mateyak/Vector.hpp"
#include "Perlin/Perlin.hpp"
#include "Map.hpp"

Venom::Venom()
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
    pos = {2.5, 0.75, 2.5};

}


Venom::~Venom()
{
    UnloadTexture(this->texture);
}

void Venom::Draw_leg(Mateyak::Vec3f leg, int seed)
{
    Mateyak::Vec3f vec(pos.x - leg.x, 0, pos.z - leg.z);
    float len = 80 - (c_pos - leg).len();
    vec = -vec;
    int nb = BALL_NB;
    std::vector<Mateyak::Vec3f> points;
    float dis = (pos - leg).len();
    vec.x /= FLOAT_NB;
    vec.z /= FLOAT_NB;
    if (dis > DIS2)
        nb = BALL_NB * (1 - (dis - DIS2) / (DIS - DIS2));
    Mateyak::Vec3f last = pos;
    for (int j = 0; j < LEG; j++) {
        last = pos;
        last.y = sinf((-1 / FLOAT_NB * PI * 4 + PI) / 5.0);
        Mateyak::Vec3f tmp = pos;
        points.clear();
        for (int i = 0; i <= nb; i++) {
            tmp += vec;
            Mateyak::Vec3f noise = {0, 0, 0};
            tmp.y = sinf((i / FLOAT_NB * PI * 4 + PI) / 5.0);

            Mateyak::Vec3f angle = tmp - last;
            noise.x = Perlin::Noise2D(seed * j, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0);
            noise.z = Perlin::Noise2D(seed * j * 2, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0);
            Utils::generateCirclePoints(last + noise / 2, angle, ((nb - i) / FLOAT_NB) / ((j + 1) * 3), PRECI, points);
            last = tmp;
        }

        if (points.size() <= PRECI)
            return;
        std::vector<Mateyak::Triangle> triangles = Utils::connectPointsWithTriangles(points, PRECI);
        int line = triangles.size() / PRECI;
        if (triangles.empty())
            return;
        for (int i = 0; i < PRECI; i++) {
            for (int j = 0; j < line; j++) {
                Mateyak::Triangle tr = triangles[j * PRECI + i];
                Mateyak::Window::draw(tr, Color{u_char(80 - (j * 80) / line), 0, 0, (unsigned char)(3.1875 * len)});
            }
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
        pos.x -= vec.x / norm / 20.0;
        pos.z -= vec.z / norm / 20.0;
    }
    if (IsKeyDown(KEY_DOWN)) {
        pos.x += vec.x / norm / 20.0;
        pos.z += vec.z / norm / 20.0;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        pos.x += vec.z / norm / 20.0;
        pos.z -= vec.x / norm / 20.0;
    }
    if (IsKeyDown(KEY_LEFT)) {
        pos.x -= vec.z / norm / 20.0;
        pos.z += vec.x / norm / 20.0;
    }
}

void Venom::draw_ven(int seed, const Mateyak::Camera& camera)
{
    c_pos = camera._position;
    for (int i = 0; i < 5000; i++) {
        time = GetTime();
        if ((pos - pos_feet[i]).len() < DIS && (c_pos - pos_feet[i]).len() < 80) {
            if (Utils::differenceAngle((c_pos - camera._target).Normalize(), (c_pos - pos_feet[i]).Normalize()) > 45)
                continue;
            Draw_leg(pos_feet[i], seed + i * 1000);
        }
    }
}

Mateyak::Vec3f Venom::getPos() const
{
    return pos;
}