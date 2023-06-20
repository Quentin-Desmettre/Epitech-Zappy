/*
** EPITECH PROJECT, 2023
** venom
** File description:
** window
*/

#pragma once

#include <raylib.h>
#include <string>
#include <raymath.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "Triangle.hpp"
#include "Camera.hpp"
#include "Vector2.hpp"
#include "Model3D.hpp"
#include "Map.hpp"
//
//#ifndef RLIGHTS_IMPLEMENTATION
//    #include "rlights.h"
//    #define RLIGHTS_IMPLEMENTATION
//#endif
//// #include <rlights.h>
//#if defined(PLATFORM_DESKTOP)
//    #define GLSL_VERSION            330
//#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB -> Not supported at this moment
//    #define GLSL_VERSION            100
//#endif

namespace Mateyak
{
    class Window {
    public:
        Window(int width = 1920, int height = 1080, std::string title = "Mateyak", int fps = 60);
        ~Window();
        bool ShouldClose();

        void startDrawing(Color color = BLACK);
        void begin3D(const Mateyak::Camera &camera);
        static void draw(const Triangle &triangle);
        static void draw(const Triangle &triangle, Color color);
        static void draw(const Model3D &model);
        static void draw(const std::string &text, float x = 0, float y = 0, int fontSize = 15, Color color = DARKGRAY);
        static void draw(const Map &map);
        static void drawBox(float posX, float posY, float width, float height, Color color);
        void end3D();
        void endDrawing();

        int width;
        int height;
        static Font _font;

        Vec2f getMousePosition() const {
            return Vec2f{static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())};
        }

        static double time;
        static double timePass;

    private:
    };
}