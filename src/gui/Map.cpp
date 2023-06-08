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


Map::Map(Mateyak::Vec2f size, float zoom):
    _size(size),
    map(size),
    clr(size),
    _zoom(zoom)
{
    srand(time(NULL));
    generate();
    generateColor();
}

void Map::generate()
{
    std::vector<char> pixels;
    mpp.clear();
    int rng = rand();
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
//void set_map(Image &image, Image &color, int rng)
//{
//    char *pixels = new char[400 * 400];
//    int *pixels_clr = new int[400 * 400];
//    unsigned char noise = 0;
//    image.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
//    image.mipmaps = true;
//    color.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
//    color.mipmaps = true;
//    float zoom = 1.0;
//    for (int i = 0; i < 400; i++)
//        for (int j = 0; j < 400; j++) {
//            noise = ((Perlin::Noise2D(rng, i / zoom, j / zoom, 5)) + 1) * 127;
//            noise = noise / 255.0 * 127;
//            pixels[i * 400 + j] = noise;
//        }
//    int max = 0;
//    int min = 255;
//    for (int i = 0; i < 400; i++)
//        for (int j = 0; j < 400; j++) {
//            if (pixels[i * 400 + j] > max)
//                max = pixels[i * 400 + j];
//            if (pixels[i * 400 + j] < min)
//                min = pixels[i * 400 + j];
//        }
//    for (int i = 0; i < 400; i++)
//        for (int j = 0; j < 400; j++) {
//            pixels[i * 400 + j] = (pixels[i * 400 + j] - min) / float(max - min) * 255;
//            noise = pixels[i * 400 + j];
//            color_u tmp;
//            tmp.value = 0;//rgba(121,4,235,255)
//            tmp.r = 0;
//            tmp.g = (255 - noise) / 255.0 * 255;
//            tmp.b = (255 - noise) / 255.0 * 255;
//            tmp.a = 255;
//            pixels_clr[i * 400 + j] = (255) << 24 | tmp.value;
//        }
//    image.data = pixels;
//    image.height = 400;
//    image.width = 400;
//    color.data = pixels_clr;
//    color.height = 400;
//    color.width = 400;
//}