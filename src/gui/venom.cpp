#include <math.h>
#include "venom.hpp"
#define FLOAT_NB (float)circlePerLeg
#define DIS 2.f
#define DIS2 1.5f
#include "Utils3d.hpp"
#include "Mateyak/Vector.hpp"
#include "Perlin/Perlin.hpp"

int Venom::nbLegs = 5;
int Venom::circlePerLeg = 30;
int Venom::pointPerCircle = 4;
bool Venom::usePerlin = true;
double Venom::time = 0;
std::vector<std::vector<std::vector<Mateyak::Vec3f>>> Venom::feet_pos;

Venom::Venom(Mateyak::Vec2f pos, Mateyak::Vec2f mapSize, Color clr): mapSize(mapSize),
    _clr(clr)
{
    rnd = {rand() % 100 / 100.f - 0.5f, 0, rand() % 100 / 100.f - 0.5f};
    _pos = {(pos.x * 10 + 5) / 3.F, 3, (pos.y * 10 + 5) / 3.F};
    _nextPosition = _pos;
    _nextPosition.y = 0.75;
    level = 1;
    state = Player::STATE::SPAWNING;
    isSelected = false;
}

Venom::~Venom()
{
    //UnloadTexture(this->texture);
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
        tmp.y = sinf((i / FLOAT_NB * PI * 4 + PI) / 5.0f) * 0.7;
        if (state == Player::STATE::INCANTING)
            tmp.y *= time;

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
            if (usePerlin) {
                noise.x = Perlin::Noise2D(seed * h, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0) / 2;
                noise.z = Perlin::Noise2D(seed * h * 2, i / (FLOAT_NB / 12.5), seed, 1) * ((i / FLOAT_NB + 0.5) / 2.0) / 2;
            }
            float raylevel = level / 8.0 + 0.5;
            Utils::generateCirclePoints(center[i] + noise, angles[i], ((nb - i) / FLOAT_NB) / ((h - 1) * 6) * raylevel, pointPerCircle, points);
        }
        std::vector<Mateyak::Triangle> triangles = Utils::connectPointsWithTriangles(points, pointPerCircle);
        if (triangles.empty())
            return;
        for (size_t i = 0; i < triangles.size(); i++) {
            Mateyak::Triangle tr = triangles[i];
            float clr_mul = 1 - (float)i / (float)triangles.size();
            if (isSelected) {
                Mateyak::Window::draw(tr, _clr);
            } else
                Mateyak::Window::draw(tr, Color{u_char(_clr.r * clr_mul), u_char(_clr.g * clr_mul), u_char(_clr.b * clr_mul), (unsigned char)(2.55 * len)});
        }
    }
}

void Venom::move_ven(Camera camera)
{
    Mateyak::Vec3f vec;

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
        vec = {camera.position.x - camera.target.x, 0, camera.position.z - camera.target.z};
        vec = vec.Normalize();
        vec = vec * Mateyak::Window::time * 15.0;
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
    if (IsKeyPressed(KEY_KP_ADD)) {
        state = (state + 1) % 6;
    }
    if (IsKeyPressed(KEY_KP_SUBTRACT)) {
        level = (level + 1 % 9);
    }
}

bool sameSign(float a, float b)
{
    return (a >= 0) ^ (b < 0);
}

void Venom::move_ven(int timeunit)
{
    float norm = 0;
    Vector3 vec;

    if (_pos == _nextPosition) {
        return;
    }
    vec = {_nextPosition.x - _pos.x, _pos.y, _nextPosition.z - _pos.z};
    norm = sqrt(pow(vec.x, 2) + pow(vec.z, 2));
    if (std::abs(vec.x) > 3.334 || std::abs(vec.z) > 3.334) {
        vec = {-vec.x, vec.y, -vec.z};
    }
    if (norm < 0.1) {
        _pos = _nextPosition;
    } else {
        _pos.x += (vec.x / norm) * Mateyak::Window::timePass * 22.0 * (timeunit / 100.0);
        _pos.z += (vec.z / norm) * Mateyak::Window::timePass * 22.0 * (timeunit / 100.0);
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
    if (Utils::differenceAngle((c_pos - camera._target).Normalize(), (c_pos - _pos).Normalize()) > 45)
        return;
    if (state == Player::STATE::EGGFORKED || state == Player::STATE::EGGHATCHING || state == Player::STATE::EGGLAYING) {
        Mateyak::Vec3f egg_pos = _pos + rnd;
        _pos.y = 0.5;
        DrawSphere(egg_pos, state == Player::STATE::EGGHATCHING ? time / 2.0 : 0.5, state == Player::STATE::EGGFORKED ? Color{120, 120, 120, 255} : _clr);
        return;
    }
    _pos = _pos + rnd;
    int x = _pos.x / (10 / 3.f);
    int y = _pos.z / (10 / 3.f);
    int minY = (y - 1 > 0 ? y - 1 : 0);
    int maxY = (y + 1 < mapSize.y ? y + 1 : mapSize.y);
    int minX = (x - 1 > 0 ? x - 1 : 0);
    int maxX = (x + 1 < mapSize.x ? x + 1 : mapSize.x);

    for (int i = minY; i <= maxY; i++) {
        for (int j = minX; j <= maxX; j++) {
            for (auto &it : feet_pos[j][i]) {
                if ((it - _pos).len() < DIS) {
                    Draw_leg(it, seed + i * 1000);
                }
            }
        }
    }
    _pos = _pos - rnd;
}

Mateyak::Vec3f Venom::getPos() const
{
    return _pos;
}

Mateyak::Vec3f Venom::getNextPos() const
{
    return _nextPosition;
}


Mateyak::Vec3f &Venom::getPosition()
{
    return _pos;
}

void Venom::setNextPos(const Mateyak::Vec3f &pos)
{
    _pos = _nextPosition;
    _nextPosition = {(pos.x * 10 + 5) / 3.F, _pos.y, (pos.y * 10 + 5) / 3.F};
}

void Venom::setPos(const Mateyak::Vec3f &pos)
{
    _pos = {(pos.x * 10 + 5) / 3.F, _pos.y, (pos.y * 10 + 5) / 3.F};
}
double EaseInOut(double t)
{
    return 0.5 * (1.0 - cos(t * PI));
}

void Venom::fpsHandler() {

    time = GetTime();
    time *= 3.0;
    time = time - (int)time;
    if (time < 0.5) {
        time = 1 - time;
    }
    time = EaseInOut(time) + 0.5;
    int fps = GetFPS();
    if (fps < 60) {
        if (nbLegs > 1) {
            nbLegs--;
        } else if (pointPerCircle > 3) {
            pointPerCircle--;
        } else if (circlePerLeg > 6) {
            circlePerLeg--;
        } else if (usePerlin) {
            usePerlin = false;
        }
    }
    if (fps > 60) {
        if (!usePerlin) {
            usePerlin = true;
        } else if (circlePerLeg < 15) {
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

int Venom::getLevel() const {
    return level;
}

void Venom::setLevel(int level)
{
    this->level = level;
}

void Venom::setState(int state)
{
    this->state = state;
}

int Venom::getState() const {
    return state;
}
