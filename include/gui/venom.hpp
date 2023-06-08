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
    Mateyak::Vec3f pos;
    Mateyak::Vec3f c_pos;
public:
    float time;
    std::vector<Mateyak::Vec3f> pos_feet;
    Venom();
    ~Venom();
    void Draw_leg(Mateyak::Vec3f pos, int seed);
    void move_ven(Camera camera);
    void draw_ven(int seed, const Mateyak::Camera& camera);
    Mateyak::Vec3f getPos() const;
};

#endif
