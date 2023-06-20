#pragma once
#include <utility>
#include "raylib.h"
#include <math.h>
#include <vector>
#include "Mateyak/Vector.hpp"
#include "Mateyak/Camera.hpp"

class Venom
{
private:
    Model models;
    Mesh mesh;
    Texture2D texture;
    Mateyak::Vec3f _pos;
    Mateyak::Vec3f _nextPosition;
    Mateyak::Vec3f c_pos;
    Mateyak::Vec2f mapSize;
    Color _clr;
    Mateyak::Vec3f rnd;
    int level;
    int state;
    static int nbLegs;
    static int nbBigLegs;
    static int circlePerLeg;
    static int pointPerCircle;
    static bool usePerlin;
    static double time;
    std::vector<Mateyak::Vec3f> pos_feet;

public:
    bool isSelected;
    void setLevel(int level);
    int getLevel() const;
    void setState(int state);
    [[nodiscard]] int getState() const;
    static void fpsHandler();
    static std::vector<std::vector<std::vector<Mateyak::Vec3f>>> feet_pos;
    Venom(Mateyak::Vec2f pos = {0, 0}, Mateyak::Vec2f mapSize = {10, 10}, Color clr = WHITE);
    ~Venom();
    void Draw_leg(Mateyak::Vec3f pos, int seed);
    void move_ven(Camera camera);
    void move_ven(int timeUnit);
    void draw_ven(int seed, const Mateyak::Camera& camera);
    Mateyak::Vec3f getPos() const;
    Mateyak::Vec3f getNextPos() const;
    Mateyak::Vec3f &getPosition();
    void setPos(const Mateyak::Vec3f &pos);
    void setNextPos(const Mateyak::Vec3f &pos);
};
