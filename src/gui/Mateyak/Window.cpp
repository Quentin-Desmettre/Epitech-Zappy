#include "Mateyak/Window.hpp"

Mateyak::Window::Window(int width, int height, std::string title, int fps)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(fps);
}

Mateyak::Window::~Window()
{
    CloseWindow();
}

bool Mateyak::Window::ShouldClose()
{
    return WindowShouldClose();
}

void Mateyak::Window::draw(const Triangle &triangle)
{
    DrawTriangle3D(triangle._a.getRayVec3(), triangle._b.getRayVec3(), triangle._c.getRayVec3(), triangle._color);
}

void Mateyak::Window::draw(const Triangle &triangle, Color color)
{
    DrawTriangle3D(triangle._a.getRayVec3(), triangle._b.getRayVec3(), triangle._c.getRayVec3(), color);
}

void Mateyak::Window::draw(const Model3D &model)
{
    DrawModel(model, model.getPos(), model.getScale(), model.getColor());
}

void Mateyak::Window::draw(const std::string &text, int x, int y, int fontSize, Color color)
{
    DrawText(text.c_str(), x, y, fontSize, color);
}

void Mateyak::Window::startDrawing(Color color)
{
    BeginDrawing();
    ClearBackground(color);
}

void Mateyak::Window::endDrawing()
{
    EndDrawing();
}

void Mateyak::Window::begin3D(const Mateyak::Camera &camera)
{
    BeginMode3D(camera.getRayCam());
}

void Mateyak::Window::end3D()
{
    EndMode3D();
}

void Mateyak::Window::draw(const Map &map)
{
    draw(map.getGround());
}