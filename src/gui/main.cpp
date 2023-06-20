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

void Graphic::loop()
{
    int seed = rand();
    int viewPos = _shader.getUniformLocation("viewPos");
    _map->setShader(_shader);
    bool shaderEnabled = true;
    bool drawGrid = false;
    _shader.setUniform("shaderEnabled", shaderEnabled);

    while (!WindowShouldClose() && _serverInformations.isRunning()) {
        if (IsKeyPressed(KEY_F1)) {
            shaderEnabled = !shaderEnabled;
            _shader.setUniform("shaderEnabled", shaderEnabled);
        }
        if (IsKeyPressed(KEY_F2)) {
            drawGrid = !drawGrid;
        }
        _shader.setUniform(viewPos, _cam._position);
        _cam.Update();
        _win.startDrawing();
        ClearBackground(Color{255 / 10, 255 / 20, 255 / 20, 255});
        _win.begin3D(_cam);
        Mateyak::Window::draw(*_map);
        Mateyak::Window::draw(_flat);
        Venom::fpsHandler();
        _serverInformations.startComputing();
        _map->update(_serverInformations);
        for (auto &it : _serverInformations.getPlayers()) {
            if (!it) continue;
            it->ven.draw_ven(seed, _cam);
            _serverInformations.updatePlayer(it);
        }
        if (drawGrid) {
            Utils::drawGrid(_mapSize, 10 / 3.F, {0, 0, 0});
        }
        _win.end3D();
        _serverInformations.audioActionsHandler(_cam);
        DrawFPS(10, 10);
        drawTeams();
        drawBroadCastMessage(_win);
        drawTileInformation(_win, _cam);
        drawPlayerInformation(_win, _cam);
        drawTimeUnit();
        _serverInformations.updateTimeUnit();
        _serverInformations.endComputing();
        _win.endDrawing();
    }
}

bool Graphic::menu(std::string &ip, std::string &port) {
    Image image = LoadImage("assets/GuiMenu/button.png");
    Image bg = LoadImage("assets/GuiMenu/vid.gif");
    ImageResize(&image, 200, 100);
    std::vector<Texture2D> textures;
    std::vector<Vector2> positions;
    std::vector<std::string> functions;

    textures.emplace_back(LoadTextureFromImage(bg));
    positions.emplace_back(Vector2{0, 0});
    functions.emplace_back("none");

    textures.emplace_back(LoadTextureFromImage(image));
    positions.emplace_back(Vector2{_windowWidth / 2 - textures[1].width / 2, _windowHeight / 1.4f - textures[1].height / 2});
    functions.emplace_back("start");

    textures.emplace_back(LoadTextureFromImage(image));
    positions.emplace_back(Vector2{_windowWidth / 2 - textures[2].width / 2, _windowHeight / 1.4f - textures[2].height / 2 + 150});
    functions.emplace_back("quit");
    float scale = 1.f;

    int loop = true;

    while (!WindowShouldClose() && loop) {
        _win.startDrawing();
        ClearBackground(RAYWHITE);
        for (size_t i = 0; i < textures.size(); i++) {
            Color spriteTint = WHITE;
            bool isMouseOnSprite = CheckCollisionPointRec(GetMousePosition(),
            {positions[i].x, positions[i].y, static_cast<float>(textures[i].width), static_cast<float>(textures[i].height)});

            if (functions[i] != "none") {
                if (isMouseOnSprite) {
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                        spriteTint = RED;
                    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                        if (functions[i] == "start") {
                            _win.endDrawing();
                            return true;
                        }
                        if (functions[i] == "quit") {
                            loop = false;
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

    while (true) {
        needToContinue = 0;
        if (!graphic.menu(ip, port))
            return;
        try {
            GuiClient client(serverInformations, ip, port);
            client.CheckValidServer();
            graphic.setMapSize(serverInformations.getMapSize());
            std::thread t(&GuiClient::compute, &client);
            graphic.loop();
            client.stop();
            t.join();
        } catch (const std::exception& ex) {
            needToContinue = 1;
        }
        if (needToContinue)
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
