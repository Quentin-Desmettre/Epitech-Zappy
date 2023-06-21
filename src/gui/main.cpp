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
        _serverInformations.audioActionsHandler(_cam);
        DrawInfo();
        _serverInformations.updateTimeUnit();
        _serverInformations.endComputing();
        _win.endDrawing();
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

void tmp(ServerInformations &serverInformations, const std::string &ip, const std::string &port, Graphic &graphic, bool &backMenu)
{
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
        serverInformations.clear();
        if (!graphic.menu(ip, port, error))
            return;
        error = false;
        try {
            tmp(serverInformations, ip, port, graphic, backMenu);
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
        std::string error = ex.what();
        if (error == "Connection failed") {
            std::cerr << "Please verify if the server is started" << std::endl;
        } else {
            std::cerr << "Exception: " << ex.what() << std::endl;
            std::cout << "USAGE:\t./zappy_ai -p port -h machine" << std::endl;
            std::cout << "\tport\tis the port number" << std::endl;
            std::cout << "\tmachine\tis the name of the machine; localhost by default" << std::endl;
        }
    }
    return 0;
}
