#include <raylib.h>
#include "venom.hpp"
#include "Mateyak/Window.hpp"
#include "Mateyak/Vector.hpp"
#include "Map.hpp"
#include "Mateyak/Sprite.hpp"
#include "Mateyak/Shaders.hpp"
#include "Client/client.hpp"
#include "Mateyak/Vector2.hpp"
#include "Mateyak/Model3D.hpp"
#include "Graphic.hpp"
#include "Utils3d.hpp"


void Graphic::drawTeams()
{
    int numberTeams = (_teamNumber / 4) + (_teamNumber % 4 > 0 ? 1 : 0);
    int x = _windowWidth - _boxSize - (numberTeams * 20);
    int y = 20;

    Mateyak::Window::drawBox(x - 10, y - 10, _boxSize + numberTeams * 20, 4 * 20 + 20, {0, 0, 0, 100});
    for (auto &it : _serverInformations.getTeams()) {
        Mateyak::Window::draw(it.getName(), x, y, 15, it.getColor());
        if (y == 80) {
            x += _maxSize + 20;
            y = 20;
        } else
            y += 20;
    }
}

void Graphic::getTeamsPlace(Mateyak::Window &win)
{
    for (auto &it : _serverInformations.getTeams()) {
        it.getName();
        Vector2 size = MeasureTextEx(win._font, it.getName().c_str(), 15, 1);
        it.width = size.x;
        it.height = size.y;
        if (size.x > _maxSize)
            _maxSize = size.x;
    }
    _teamNumber = _serverInformations.getTeams().size();
    _boxSize = _maxSize * (_teamNumber / 4) + (_teamNumber % 4 > 0 ? _maxSize : 0);
}

void Graphic::loop(Mateyak::Vec2f mapSize)
{
    _windowWidth = 1920 / 2;
    _windowHeight = 1080 / 2;
    Mateyak::Window win(_windowWidth, _windowHeight, "Zappy", 400);
    int seed = rand();
    Mateyak::Camera cam({5.0f, 5.0f, 5.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f);
    Map map(mapSize * 10, 0.5);
    Venom ven({0, 0});
    Mateyak::Model3D flat(GenMeshPoly(10, 10000.0f), Mateyak::Vec3f{-500, -1, -500}, 1.0f, BLACK);
    Mateyak::Shaders shader("src/gui/shader/base_lighting.vs", "src/gui/shader/test.fs");

    shader.setUniform("fogDensity", 0.015f);
    shader.setUniform("lightsPos", {mapSize.x * 5.f / 3.f, 20.0f, mapSize.y * 5.f / 3.f});
    shader.setUniform("lightsColor", {0.5f, 0.5f, 0.5f});
    shader.setUniform("lightsEnabled", 1);
    int viewPos = shader.getUniformLocation("viewPos");
    map.setShader(shader);
    bool shaderEnabled = true;
    bool drawGrid = false;
    shader.setUniform("shaderEnabled", shaderEnabled);

    _serverInformations.startComputing();
    getTeamsPlace(win);
    _serverInformations.endComputing();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F1)) {
            shaderEnabled = !shaderEnabled;
            shader.setUniform("shaderEnabled", shaderEnabled);
        }
        if (IsKeyPressed(KEY_F2)) {
            drawGrid = !drawGrid;
        }
        shader.setUniform(viewPos, cam._position);
        ven.move_ven(cam.getRayCam());
        cam.Update();
        win.startDrawing();
        ClearBackground(Color{255 / 10, 255 / 20, 255 / 20, 255});
        win.begin3D(cam);
        Mateyak::Window::draw(map);
        Mateyak::Window::draw(flat);
        ven.draw_ven(seed, cam);
        _serverInformations.startComputing();
        map.update(_serverInformations);
        for (auto &it : _serverInformations.getPlayers()) {
            it->ven.draw_ven(seed, cam);
            _serverInformations.updatePlayer(it);
        }
        if (drawGrid) {
            Utils::drawGrid(mapSize, 10 / 3.F, {0, 0, 0});
        }
        _serverInformations.endComputing();
        if (drawGrid)
            Utils::drawGrid(mapSize, 10/3.f, {0, 0, 0});
        win.end3D();
        DrawFPS(10, 10);
        drawTeams();
        _serverInformations.endComputing();
        win.endDrawing();
    }
}

class ErrorHandling {
    public:
        class Error : public std::exception {
            public:
                Error(std::string const &message) : _message(message) {}
                const char *what() const noexcept override { return _message.c_str(); }
            private:
                std::string _message;
        };
        void parse();
        ErrorHandling(int ac, char **av);
        std::string getPort() const;
        std::string getIp() const;
        ~ErrorHandling() = default;
    private:
        int _ac;
        char **_av;
        std::string _port;
        std::string _ip;
};

ErrorHandling::ErrorHandling(int ac, char **av) : _ac(ac), _av(av)
{
    if (!(ac == 3 || ac == 5))
        throw Error("Invalid number of arguments");
}

void ErrorHandling::parse()
{
    for (int i = 1; i < _ac; i++) {
        if (std::string(_av[i]) == "-p") {
            if (i + 1 >= _ac)
                throw Error("Port not defined after -p");
            _port = _av[i + 1];
            i++;
        }
        else if (std::string(_av[i]) == "-h") {
            if (i + 1 >= _ac)
                throw Error("Ip not defined after -h");
            _ip = _av[i + 1];
            i++;
        }
        else {
            std::string test(_av[i]);
            throw Error(test + std::string(" is not a valid argument"));
        }
    }
    if (_port.empty())
        throw Error("Port not defined");
}

std::string ErrorHandling::getPort() const
{
    return _port;
}

std::string ErrorHandling::getIp() const
{
    if (_ip.empty())
        return std::string("localhost");
    return _ip;
}

int main(int ac, char **av)
{
    Graphic graphic;

    try {
        ErrorHandling errorHandling(ac, av);
        errorHandling.parse();
        GuiClient client(graphic.getServerInformations(), errorHandling.getIp(), errorHandling.getPort());

        if (!client.CheckValidServer())
            return 84;
        Mateyak::Vec2f mapSize = graphic.getServerInformations().getMapSize();

       std::thread t(&GuiClient::compute, &client);
        graphic.loop(mapSize);
        client.stop();
        t.join();

    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
