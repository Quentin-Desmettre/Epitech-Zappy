/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_MENU_CPP
** File description:
** menu.cpp
*/

#include "Graphic.hpp"
#define MAX_PORT_LENGTH 6
#define MAX_IP_LENGTH 16

#define BG 0
#define BUTTON_START 1
#define BUTTON_QUIT 2

void Graphic::DrawPort(std::string &port, int &textActive) {
    if (textActive == 1) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!port.empty()) port.erase(port.size() - 1);
        }
        int key = GetCharPressed();
        if ((key > 0) && ((key >= '0' && key <= '9'))) {
            if (textActive == 1 && port.size() < MAX_PORT_LENGTH - 1)
                port += (char) key;
        }
    }

    DrawText("Enter a port:", _windowWidth / 2 - 160, _windowHeight / 2 - 100,
             20, DARKGRAY);
    DrawRectangleLines(_windowWidth / 2 + 50, _windowHeight / 2 - 100,
                       _charSize.x * MAX_PORT_LENGTH, _charSize.y, DARKGRAY);
    DrawText((port + (textActive == 1 ? '|' : '\0')).c_str(),
             _windowWidth / 2 + 55, _windowHeight / 2 - 100, 20, MAROON);
}

void Graphic::DrawIp(std::string &ip, int &textActive)
{
    if (textActive == 2) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!ip.empty()) ip.erase(ip.size() - 1);
        }
        int key = GetCharPressed();
        if ((key > 0) && ((key >= '0' && key <= '9') || key == '.')) {
            if (ip.size() < MAX_IP_LENGTH - 1)
                ip += (char)key;
        }
    }

    DrawText("Enter a ip:", _windowWidth / 2 - 160, _windowHeight / 2 - 50, 20, DARKGRAY);
    DrawRectangleLines(_windowWidth / 2 + 50, _windowHeight / 2 - 50, _charSize.x * MAX_IP_LENGTH, _charSize.y, DARKGRAY);
    DrawText((ip + (textActive == 2 ? '|' : '\0')).c_str(), _windowWidth / 2 + 65, _windowHeight / 2 - 50, 20, MAROON);
}

void Graphic::initMenu()
{
    _charSize = MeasureTextEx(_win._font, "Z", 20, 1);
    static bool isInit = false;

    if (isInit) return;
    _menuImage.emplace_back(LoadImage("assets/GuiMenu/bg.png"));
    _menuImage.emplace_back(LoadImage("assets/GuiMenu/start.png"));
    _menuImage.emplace_back(LoadImage("assets/GuiMenu/quit.png"));

    ImageResize(&_menuImage[BUTTON_START], 120, 50);
    ImageResize(&_menuImage[BUTTON_QUIT], 120, 50);
    ImageCrop(&_menuImage[BG], Rectangle{0, 0, _windowWidth, _windowHeight});

    _textures.emplace_back(LoadTextureFromImage(_menuImage[BG]));
    _positions.emplace_back(Vector2{0, 0});
    _functions.emplace_back("bg");

    _textures.emplace_back(LoadTextureFromImage(_menuImage[BUTTON_START]));
    _positions.emplace_back(Vector2{_windowWidth / 2 - _textures[BUTTON_START].width / 2.f - 80, _windowHeight / 2 + 40});
    _functions.emplace_back("start");


    _textures.emplace_back(LoadTextureFromImage(_menuImage[BUTTON_QUIT]));
    _positions.emplace_back(Vector2{_windowWidth / 2 - _textures[BUTTON_QUIT].width / 2.f + 150, _windowHeight / 2 + 40});
    _functions.emplace_back("quit");

    isInit = true;
}

bool Graphic::DrawTextMenu(int &loop)
{
    float scale = 1.f;

    for (size_t i = 0; i < _textures.size(); i++) {
        Color spriteTint = WHITE;
        bool isMouseOnSprite = CheckCollisionPointRec(GetMousePosition(),
        {_positions[i].x, _positions[i].y,
        static_cast<float>(_textures[i].width),
        static_cast<float>(_textures[i].height)});

        if (_functions[i] == "bg") {
            DrawTextureEx(_textures[i], _positions[i], 0.0f, scale, spriteTint);
            continue;
        }

        spriteTint = isMouseOnSprite ? (IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? RED : YELLOW) : WHITE;

        if (isMouseOnSprite && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (_functions[i] == "start") {
                _win.endDrawing();
                return true;
            }
            if (_functions[i] == "quit") {
                loop = false;
            }
        }

        DrawTextureEx(_textures[i], _positions[i], 0.0f, scale, spriteTint);
    }
    return false;
}

bool Graphic::menu(std::string &ip, std::string &port, bool isError) {
    initMenu();
    int textActive = 0;

    int loop = true;

    while (!WindowShouldClose() && loop) {
        _win.startDrawing(WHITE);
        if (DrawTextMenu(loop))
            return true;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            bool isMouseOnRec = CheckCollisionPointRec(GetMousePosition(),
            {_windowWidth / 2 + 65, _windowHeight / 2 - 100, static_cast<float>(_charSize.x * MAX_PORT_LENGTH), _charSize.y});
            textActive = isMouseOnRec;
            bool isMouseOnRec2 = CheckCollisionPointRec(GetMousePosition(),
            {_windowWidth / 2 + 65, _windowHeight / 2 - 50, static_cast<float>(_charSize.x * MAX_IP_LENGTH), _charSize.y});
            textActive = isMouseOnRec2 ? 2 : textActive;
        }
        DrawText(std::string("Zappy").c_str(), _windowWidth / 2 - 90, 50, 80, RED);
        DrawRectangleLines(_windowWidth / 2 - 200, _windowHeight / 2 - 200, 480, 350, DARKGRAY);
        DrawPort(port, textActive);
        DrawIp(ip, textActive);
        if (isError == 1) {
            DrawText(std::string("No Zappy server found at " + ip + " : " + port + " !\nCheck if the server you are trying to reach is online !").c_str(), 20, _windowHeight - 80, 20, RED);
        }
        _win.endDrawing();
    }
    return false;
}
