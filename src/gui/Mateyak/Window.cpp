#include "Mateyak/Window.hpp"

double Mateyak::Window::timePass = 0;
double Mateyak::Window::time = GetTime();


Font Mateyak::Window::_font = GetFontDefault();

Mateyak::Window::Window(int width, int height, std::string title, int fps)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(fps);
    Mateyak::Window::_font = LoadFont("assets/arial.ttf");
    if (!_font.texture.id) {
        Mateyak::Window::_font = GetFontDefault();
    }
    this->width = width;
    this->height = height;
}

Mateyak::Window::~Window()
{
    UnloadFont(Mateyak::Window::_font);
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

void Mateyak::Window::draw(const std::string &texts, float x, float y, int fontSize, Color color)
{
    const char* text = texts.c_str();
    Vector2 textPosition = { x, y };

    DrawTextEx(_font, text, textPosition, (float)fontSize, 1, color);
}

void Mateyak::Window::drawBox(float posX, float posY, float width, float height, Color color)
{
    Vector2 pos = {posX, posY};
    Vector2 size = {width, height};

    DrawRectangleV(pos, size, color);
}

void Mateyak::Window::startDrawing(Color color)
{
    timePass = GetTime() - time;
    time = GetTime();
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