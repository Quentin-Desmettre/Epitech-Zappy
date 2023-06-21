#include <raylib.h>
#include "venom.hpp"
#include "Mateyak/Window.hpp"
#include "Mateyak/Vector.hpp"
#include "Map.hpp"
#include "Client/client.hpp"
#include "Mateyak/Vector2.hpp"
#include "Graphic.hpp"
#include "Utils3d.hpp"
#include "ErrorHandling.hpp"

#define MAX_PORT_LENGTH 6
#define MAX_IP_LENGTH 16

bool Graphic::loop()
{
    int seed = rand();
    int viewPos = _shader.getUniformLocation("viewPos");
    _map->setShader(_shader);
    _shader.setUniform("shaderEnabled", shaderEnabled);

    while (!WindowShouldClose() && _serverInformations.isRunning()) {
        handleEvent();
        if (IsKeyPressed(KEY_ESCAPE))
            return true;
        _shader.setUniform(viewPos, _cam._position);
        _cam.Update();
        _win.startDrawing(Color{255 / 10, 255 / 20, 255 / 20, 255});
        _win.begin3D(_cam);
        Mateyak::Window::draw(*_map);
        Mateyak::Window::draw(_flat);
        Venom::fpsHandler();
        if (drawGrid)
            Utils::drawGrid(_mapSize, 10 / 3.F, {0, 0, 0});
        _serverInformations.startComputing();
        _map->update(_serverInformations);
        for (auto &it : _serverInformations.getPlayers()) {
            if (!it) continue;
            it->ven.draw_ven(seed, _cam);
            _serverInformations.updatePlayer(it);
        }
        _win.end3D();
        DrawInfo();
        _serverInformations.audioActionsHandler(_cam);
        _serverInformations.updateTimeUnit();
        _serverInformations.endComputing();
        _win.endDrawing();
    }
    return false;
}

void Graphic::DrawPort(std::string &port, int &textActive)
{
    if (textActive == 1) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!port.empty()) port.erase(port.size() - 1);
        }
        int key = GetCharPressed();
        if ((key > 0) && ((key >= '0' && key <= '9'))) {
            if (textActive == 1 && port.size() < MAX_PORT_LENGTH - 1)
                port += (char)key;
        }
    }

    DrawText("Enter a port:", _windowWidth / 2 - 160, _windowHeight / 2 - 100, 20, DARKGRAY);
    DrawRectangleLines(_windowWidth / 2 + 50, _windowHeight / 2 - 100, _charSize.x * MAX_PORT_LENGTH, _charSize.y, DARKGRAY);
    DrawText((port + (textActive == 1 ? '|' : '\0')).c_str(), _windowWidth / 2 + 65, _windowHeight / 2 - 100, 20, MAROON);
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

bool Graphic::menu(std::string &ip, std::string &port, bool isError) {
    Image startImage = LoadImage("assets/GuiMenu/start.png");
    Image quitImage = LoadImage("assets/GuiMenu/quit.png");
    Image controlsImage = LoadImage("assets/GuiMenu/controls.png");
    Image controlMenu = LoadImage("assets/GuiMenu/menu.png");
    Image bg = LoadImage("assets/GuiMenu/vid.gif");



    ImageResize(&startImage, 120, 50);
    ImageResize(&quitImage, 120, 50);
    ImageResize(&controlsImage, 120, 50);
    ImageResize(&controlMenu, _windowWidth, _windowHeight);
    std::vector<Texture2D> textures;
    std::vector<Vector2> positions;
    std::vector<std::string> functions;

    textures.emplace_back(LoadTextureFromImage(bg));
    positions.emplace_back(Vector2{0, 0});
    functions.emplace_back("none");

    textures.emplace_back(LoadTextureFromImage(startImage));
    positions.emplace_back(Vector2{_windowWidth / 2 - textures[1].width / 2 - 80, _windowHeight / 2 + 40});
    functions.emplace_back("start");

    textures.emplace_back(LoadTextureFromImage(quitImage));
    positions.emplace_back(Vector2{_windowWidth / 2 - textures[2].width / 2 + 150, _windowHeight / 2 + 40});
    functions.emplace_back("quit");

    textures.emplace_back(LoadTextureFromImage(controlsImage));
    positions.emplace_back(Vector2{30, 30});
    functions.emplace_back("help");

    textures.emplace_back(LoadTextureFromImage(controlMenu));
    positions.emplace_back(Vector2{0, 0});
    functions.emplace_back("none");

    float scale = 1.f;
    int textActive = 0;

    int loop = true;

    while (!WindowShouldClose() && loop) {
        _win.startDrawing();
        ClearBackground(BLACK);

        if (onControlMenu) {
            DrawTextureEx(textures[4], positions[4], 0.0f, scale, WHITE);
            if (IsKeyPressed(KEY_BACKSPACE)) {
                onControlMenu = false;
            }
            _win.endDrawing();
            continue;
        }

        for (size_t i = 0; i < textures.size(); i++) {
            if (i == 4) continue;

            Color spriteTint = WHITE;
            bool isMouseOnSprite = CheckCollisionPointRec(GetMousePosition(),
            {positions[i].x, positions[i].y, static_cast<float>(textures[i].width), static_cast<float>(textures[i].height)});

            if (functions[i] != "none") {
                if (isMouseOnSprite) {
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                        spriteTint = RED;
                    } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                        if (functions[i] == "start") {
                            _win.endDrawing();
                            return true;
                        }
                        if (functions[i] == "quit") {
                            loop = false;
                        }
                        if (functions[i] == "help") {
                            onControlMenu = true;
                        }
                    }
                    else
                        spriteTint = YELLOW;
                } else {
                    spriteTint = WHITE;
                }
            }
            DrawTextureEx(textures[i], positions[i], 0.0f, scale, spriteTint);
        }

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

    for (auto &sprite : textures) {
        UnloadTexture(sprite);
    }
    return false;
}

void run_with_parameter(int ac, char **av)
{
    ServerInformations serverInformations;

    ErrorHandling errorHandling(ac, av);
    errorHandling.parse();
    GuiClient client(serverInformations, errorHandling.getIp(), errorHandling.getPort());
    client.CheckValidServer();
    Mateyak::Vec2f mapSize = serverInformations.getMapSize();
    Graphic graphic(mapSize, {1920 / 1.3, 1080 / 1.3}, serverInformations);
    std::thread t(&GuiClient::compute, &client);
    graphic.loop();
    client.stop();
    t.join();
}

void run_without_parameters()
{
    ServerInformations serverInformations;
    std::string ip = "127.0.0.1";
    std::string port = "4242";
    Graphic graphic({0, 0}, {1920 / 1.3, 1080 / 1.3}, serverInformations);
    int needToContinue;
    bool backMenu = false;
    bool error = false;

    while (true) {
        needToContinue = 0;
        if (!graphic.menu(ip, port, error))
            return;
        error = false;
        try {
            GuiClient client(serverInformations, ip, port);
            client.CheckValidServer();
            graphic.setMapSize(serverInformations.getMapSize());
            std::thread t(&GuiClient::compute, &client);
            SetExitKey(0);
            if (graphic.loop()) {
                backMenu = 1;
                SetExitKey(KEY_ESCAPE);
            }
            client.stop();
            t.join();
        } catch (const std::exception& ex) {
            needToContinue = 1;
        }
        if (needToContinue) {
            error = true;
            continue;
        }
        if (backMenu)
            continue;
        return;
    }
}

int main(int ac, char **av)
{
    try {
       if (ac == 1) {
           run_without_parameters();
       } else {
          run_with_parameter(ac, av);
       }
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
    return 0;
}
