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

    Mateyak::Window::drawBox(x - 10, y - 10, _boxSize + numberTeams * 20, 4 * 20 + 20, {0, 39, 97, 110});
    for (auto &it : _serverInformations.getTeams()) {
        Mateyak::Window::draw(it.getName(), x, y, 15, it.getColor());
        if (y == 80) {
            x += _maxSize + 20;
            y = 20;
        } else
            y += 20;
    }
}

void Graphic::drawBroadCastMessage(Mateyak::Window &win)
{
    float boxPosY = _windowHeight - _windowHeight / 3 - 30;
    float boxWidth = _windowWidth / 3 - 20;
    float boxHeight = _windowHeight / 3 + 20;

    Mateyak::Window::drawBox(10, boxPosY, boxWidth, boxHeight, {0, 39, 97, 94});

    if (_charSize.x == 0 && _charSize.y == 0)
        _charSize = MeasureTextEx(win._font, "Z", 15, 1);

    int maxCharInLine;
    float maxLineNumber = (boxHeight) / (_charSize.y + 5);
    float yStart = _windowHeight - 30;
    std::vector<Message> &mes = _serverInformations.getBroadCastMessage();
    for (int index = mes.size() - 1; index >= 0; index--) {
        if (!mes[index]._formated) {
            maxCharInLine = boxWidth / _charSize.x;
            mes[index].FormatMessage(maxCharInLine);
        }
        for (int i = mes[index]._lines.size() - 1; i >= 0; i--) {
            if (i == 0) {
                Mateyak::Window::draw(mes[index]._name + ":", 20, yStart, 15, mes[index]._color);
                Mateyak::Window::draw(mes[index]._lines[i], 20 + ((mes[index]._name.size() + 1) * _charSize.x), yStart, 15, {255, 255, 255, 255});
            } else {
                Mateyak::Window::draw(mes[index]._lines[i], 20, yStart, 15, {255, 255, 255, 255});
            }
            yStart -= 20;
            maxLineNumber--;
            if (maxLineNumber <= 0)
                return;
        }
    }
}

void Graphic::drawTileInformation(Mateyak::Window &win)
{
    float boxPosY = _windowHeight - _windowHeight / 3 - 30;
    float boxPosX = _windowWidth - (_windowWidth / 3 - 20) - 20;
    float boxWidth = _windowWidth / 3 - 10;
    float boxHeight = _windowHeight / 3 + 20;

    if (_charSize.x == 0 && _charSize.y == 0)
        _charSize = MeasureTextEx(win._font, "Z", 15, 1);

    float tileY = 5;
    float tileX = 0;
    Mateyak::Vec2f mapSize = _serverInformations.getMapSize();

    if (tileY >= mapSize.y || tileX >= mapSize.x)
        return;

    Mateyak::Window::drawBox(boxPosX, boxPosY, boxWidth, boxHeight, {0, 39, 97, 94});
    ZappyMap map = _serverInformations.getMap();
    std::vector<Ressource> tileInfo = map[tileY][tileX];

    std::map<std::string, std::pair<int, int>> tileResources;

    for (auto &it : tileInfo) {
        if (tileResources.find(Ressource::resourceName[it.type]) == tileResources.end())
            tileResources[Ressource::resourceName[it.type]].first = 1;
        else
            tileResources[Ressource::resourceName[it.type]].first += 1;
        tileResources[Ressource::resourceName[it.type]].second = it.type;
    }

    for (auto &it : tileResources) {
        std::string str = it.first + ": " + std::to_string(it.second.first);
        Mateyak::Window::draw(str, boxPosX + 20, boxPosY + 20, 25, Ressource::clr[it.second.second]);
        boxPosY += 30;
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
    _windowWidth = 1920 / 1.3;
    _windowHeight = 1080 / 1.3;
    Mateyak::Window win(_windowWidth, _windowHeight, "Zappy", 400);
    int seed = rand();
    Mateyak::Camera cam({5.0f, 5.0f, 5.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f);
    Map map(mapSize * 10, 0.5);
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
        cam.Update();
        win.startDrawing();
        ClearBackground(Color{255 / 10, 255 / 20, 255 / 20, 255});
        win.begin3D(cam);
        Mateyak::Window::draw(map);
        Mateyak::Window::draw(flat);
        Venom::fpsHandler();
        _serverInformations.startComputing();
        map.update(_serverInformations);
        for (auto &it : _serverInformations.getPlayers()) {
            it->ven.draw_ven(seed, cam);
            _serverInformations.updatePlayer(it);
        }
        if (drawGrid) {
            Utils::drawGrid(mapSize, 10 / 3.F, {0, 0, 0});
        }
        win.end3D();
        DrawFPS(10, 10);
        drawTeams();
        drawBroadCastMessage(win);
        drawTileInformation(win);
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
