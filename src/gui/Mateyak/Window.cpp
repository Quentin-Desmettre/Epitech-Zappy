#include "Mateyak/Window.hpp"

Font Mateyak::Window::_font = GetFontDefault();

Mateyak::Window::Window(int width, int height, std::string title, int fps)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(fps);
    Mateyak::Window::_font = LoadFont("assets/arial.ttf");
    if (!_font.texture.id) {
        Mateyak::Window::_font = GetFontDefault();
    }
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

void Mateyak::Window::draw(const std::string &textss, int x, int y, int fontSize, Color color)
{
    const char* text = "dôfjepaiuhfpuihafpuihaeuihdu";
    Vector2 size = MeasureTextEx(_font, text, fontSize, 1);
    double textWidth = size.x;
    double textHeight = size.y;

    float rectX = 600;
    float rectY = 10;
    auto rectWidth = static_cast<float>(textWidth);
    auto rectHeight = static_cast<float>(textHeight);

    Vector2 rectPosition = { rectX, rectY };
    Vector2 rectSize = { rectWidth, rectHeight };
    DrawRectangleV(rectPosition, rectSize, DARKGRAY);

    Vector2 textPosition = { rectX, rectY };
    Color textColor = { 0xFF, 0x00, 0x00, 0xFF };
    DrawTextEx(_font, text, textPosition, (float)fontSize, 1, textColor);
    DrawText(textss.c_str(), x, y, fontSize, color);
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