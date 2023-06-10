#ifndef PLAYER_H
    #define PLAYER_H
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
public:
    float time;
    std::vector<Mateyak::Vec3f> pos_feet;
    Venom(Mateyak::Vec2f pos = {0, 0});
    ~Venom();
    void Draw_leg(Mateyak::Vec3f pos, int seed);
    void move_ven(Camera camera);
    void move_ven();
    void draw_ven(int seed, const Mateyak::Camera& camera);
    Mateyak::Vec3f getPos() const;
    Mateyak::Vec3f &getPosition();
    void setPos(const Mateyak::Vec3f &pos);
};

#endif
