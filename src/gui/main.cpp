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
    Venom ven = Venom();
    int viewPos = _shader.getUniformLocation("viewPos");
    _map.setShader(_shader);
    bool shaderEnabled = true;
    bool drawGrid = false;
    _shader.setUniform("shaderEnabled", shaderEnabled);

    while (!WindowShouldClose()) {
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
        Mateyak::Window::draw(_map);
        Mateyak::Window::draw(_flat);
        Venom::fpsHandler();
        ven.move_ven(_cam._cam);
        ven.draw_ven(seed, _cam);
        _serverInformations.startComputing();
        _map.update(_serverInformations);
        for (auto &it : _serverInformations.getPlayers()) {
            if (!it) continue;
            it->ven.draw_ven(seed, _cam);
            _serverInformations.updatePlayer(it);
        }
        if (drawGrid) {
            Utils::drawGrid(_mapSize, 10 / 3.F, {0, 0, 0});
        }
        _win.end3D();
        DrawFPS(10, 10);
        drawTeams();
        drawBroadCastMessage(_win);
        drawTileInformation(_win, _cam);
        drawPlayerInformation(_win, _cam);
        _serverInformations.endComputing();
        _win.endDrawing();
    }
}

int main(int ac, char **av)
{
    ServerInformations serverInformations;

    try {
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
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
