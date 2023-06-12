/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** Map
*/

//
// Created by Edo on 5/31/23.
//

#include "Map.hpp"
#include "Mateyak/Window.hpp"
#include "venom.hpp"

Map::Map(Mateyak::Vec2f size, float zoom):
    _size(size),
    map(size),
    clr(size),
    _rock("assets/rock.obj", Mateyak::Vec3f{0, 0, 0}, 0.3f, WHITE),
    _ground(),
    _food("assets/ham.obj", Mateyak::Vec3f{0, 0, 0}, 0.01f, WHITE),
    _zoom(zoom)
{
    srand(time(NULL));
    generate();
    generateColor();
    _food.setTexture({"assets/ham_text.png"});

    _ground = GenMeshHeightmap(map, (Vector3){_size.x / 3, 1, _size.y / 3});
    _ground.setPos({0, -0.5, 0});
    _ground.setTexture(clr);

    for (int i = 0; i < 5000; i++) {
        Mateyak::Vec3f ps;
        float x = GetRandomValue(0, 100) / 100.0;
        float y = GetRandomValue(0, 100) / 100.0;
        ps.x = x + i % 100;
        ps.z = (y + i / 100) * 2;
        ps.y = 0;
        Venom::pos_feet.push_back(ps);
    }
}

void Map::generate()
{
    std::vector<char> pixels;
    int rng = rand();
    mpp.clear();
    unsigned char noise = 0;
    for (int j = 0; j < _size.y; j++) {
        for (int i = 0; i < _size.x; i++) {
            noise = ((Perlin::Noise2D(rng, i / _zoom, j / _zoom, 5)) + 1) * 127;
            noise = noise / 255.0 * 127;
            pixels.push_back(noise);
        }
    }
    int max = 0;
    int min = 255;
    for (int i = 0; i < _size.x; i++)
        for (int j = 0; j < _size.y; j++) {
            if (pixels[i * _size.y + j] > max)
                max = pixels[i * _size.y + j];
            if (pixels[i * _size.y + j] < min)
                min = pixels[i * _size.y + j];
        }
    for (int i = 0; i < _size.x; i++)
        for (int j = 0; j < _size.y; j++) {
            pixels[i * _size.y + j] = (pixels[i * _size.y + j] - min) / float(max - min) * 255;
            color_u tmp;
            tmp.r = (255 - pixels[i * _size.y + j]);
            tmp.g = (255 - pixels[i * _size.y + j]);
            tmp.b = (255 - pixels[i * _size.y + j]);
            tmp.a = 255;
            mpp.push_back(tmp.value);
        }
    map.setData(mpp.data());

}

unsigned char Map::randomClr(int rand, int i, int j, int prop)
{
    float fl = ((Perlin::Noise2D(rand, i / _zoom / 5.0, j / _zoom / 5.0, 5)) + 1);
    unsigned char tmp = fl * 127 / 255.0 * 127;
    color_u tmp2;
    tmp2.value = mpp[i * _size.y + j];
    float hight = (tmp2.r);
    tmp = (hight * (10 - prop) + tmp * prop) / 10;
    return tmp;
}

void Map::generateColor()
{
    int rng = rand();
    int rng2 = rand();
    int rng3 = rand();
    colors.clear();
    color_u tmp;
    tmp.a = 255;
    for (int i = 0; i < _size.x; i++)
        for (int j = 0; j < _size.y; j++) {
            tmp.r = Map::randomClr(rng, i, j, 1) / 3;
            tmp.g = Map::randomClr(rng2, i, j, 2) / 3;
            tmp.b = Map::randomClr(rng3, i, j, 4) / 3;
            colors.push_back(tmp.value);
        }
    clr.setData(colors.data());
}


Map::~Map()
{
}

Mateyak::Sprite Map::getMap() const
{
    return map;
}

Mateyak::Sprite Map::getColor() const
{
    return clr;
}

const Mateyak::Model3D &Map::getGround() const
{
    return _ground;
}

void Map::setShader(const Mateyak::Shaders &shader) {
    _ground.setShader(shader);
    _rock.setShader(shader);
    _food.setShader(shader);
}

void Map::update(const ServerInformations &infos) {
    ZappyMap zpMap = infos.getMap();

    for (auto &col : zpMap) {
        for (auto &pos : col) {
            for (auto &obj : pos) {
                if (obj.type == 0) {
                    _food.setPos({obj.pos.x * 10 / 3.f, 0, obj.pos.y * 10 / 3.f});
                    Mateyak::Window::draw(_food);
                    continue;
                }
                _rock.setColor(obj.color);
                _rock.setPos({obj.pos.x * 10 / 3.f, 0, obj.pos.y * 10 / 3.f});
                _rock.setScale(obj.scale);
                Mateyak::Window::draw(_rock);
            }
        }
    }
}