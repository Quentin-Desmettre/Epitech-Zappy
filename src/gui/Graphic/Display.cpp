/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_DISPLAY_CPP
** File description:
** Display.cpp
*/

#include "Graphic.hpp"
#include "Utils3d.hpp"
#include <map>

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
    return !WindowShouldClose();
}

void Graphic::drawTeams()
{
    int numberTeams = (_teamNumber / 4) + (_teamNumber % 4 > 0 ? 1 : 0);
    int x = _windowWidth - _boxSize - (numberTeams * 20);
    int y = 20;

    Mateyak::Window::drawBox(x - 10, y - 10, _boxSize + (numberTeams + 1) * 20, 4 * 20 + 20, {0, 39, 97, 110});
    for (auto &it : _serverInformations.getTeams()) {
        int maxlvl = 0;

        for (auto &it2 : _serverInformations.getPlayers()) {
            if (it2->getTeam() == it.getName()) {
                if (it2->getLevel() > maxlvl)
                    maxlvl = it2->getLevel();
            }
        }
        Mateyak::Window::draw(it.getName() + ": " + std::to_string(maxlvl), x, y, 15, it.getColor());
        if (y == 80) {
            x += _maxSize + 20;
            y = 20;
        } else
            y += 20;
    }
}

void Graphic::drawBroadCastMessage()
{
    float boxPosY = _windowHeight - _windowHeight / 3 - 30;
    float boxWidth = _windowWidth / 3 - 20;
    float boxHeight = _windowHeight / 3 + 20;

    Mateyak::Window::drawBox(10, boxPosY, boxWidth, boxHeight, {0, 39, 97, 94});

    int maxCharInLine;
    float maxLineNumber = (boxHeight) / (_charSize.y + 5);
    float yStart = _windowHeight - 30;
    int index;
    std::vector<Message> &mes = _serverInformations.getBroadCastMessage();
    for (index = mes.size() - 1; index >= 0; index--) {
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
            if (maxLineNumber <= 0) {
                if (index > 0) mes.erase(mes.begin(), mes.begin() + index);
                return;
            }
        }
    }

}

void Graphic::drawTileInformation(Mateyak::Camera &cam)
{
    float boxPosY = _windowHeight - _windowHeight / 3 - 30;
    float boxPosX = _windowWidth - (_windowWidth / 3 - 20) - 20;
    float boxWidth = _windowWidth / 3 - 10;
    float boxHeight = _windowHeight / 3 + 20;

    float tileY = cam._lastClickPos.y;
    float tileX = cam._lastClickPos.x;
    Mateyak::Vec2f mapSize = _serverInformations.getMapSize();

    if (tileY >= mapSize.y || tileX >= mapSize.x || tileY < 0 || tileX < 0)
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

    Mateyak::Window::draw("Tile: (" + std::to_string((int)tileX) + ", " + std::to_string((int)tileY) + ")", boxPosX + 20, boxPosY + 20, 25, {255, 255, 255, 255});

    _win.begin3D(cam);
    Mateyak::Window::draw({(int(tileX) * (10 / 3.f)) + 5 / 3.f , 0.3, (int(tileY) * (10 / 3.f)) + 5 / 3.f}, {10 / 3.f, 10 / 3.f}, {255, 255, 255, 100});
    _win.end3D();
    boxPosY += 30;
    for (auto &it : tileResources) {
        std::string str = it.first + ": " + std::to_string(it.second.first);
        Color color = Ressource::clr[it.second.second];
        color.a = 255;
        Mateyak::Window::draw(str, boxPosX + 20, boxPosY + 20, 25, color);
        boxPosY += 30;
    }
}

void Graphic::drawPlayerInformation(Mateyak::Camera &cam)
{
    int playerSelected = -1;
    float min = 0.5f;
    float diff;
    float len = 0;
    std::vector<std::unique_ptr<Player>> &players = _serverInformations.getPlayers();
    Mateyak::Vec3f dir = cam._lastClickDir.Normalize();
    Mateyak::Vec3f camPos = cam._position;
    Mateyak::Vec3f playerPos;

    for (size_t i = 0; i < players.size(); i++) {
        playerPos = players[i]->ven.getPos();
        players[i]->ven.isSelected = false;
        playerPos.y = 0.3;
        diff = ((playerPos - camPos).Normalize() - dir).len();
        len = (playerPos - camPos).len();
        if (diff * len < min) {
            min = diff;
            playerSelected = i;
        }
    }

    if (playerSelected != -1 || cam.lastSelectedPlayer == -1)
        cam.lastSelectedPlayer = playerSelected;
    if (cam.lastSelectedPlayer == -1)
        return;

    // Display player information
    float boxPosY = _windowHeight - _windowHeight / 3 - 30;
    float boxPosX = _windowWidth - (_windowWidth / 3 - 20) - 20;
    float boxWidth = _windowWidth / 3 - 10;
    float boxHeight = _windowHeight / 3 + 20;
    const Color white = {255, 255, 255, 255};

    // Get player inventory
    _serverInformations.addCommand(std::string("pin #" + std::to_string(cam.lastSelectedPlayer) + "\n"));

    if (players.empty() || static_cast<size_t>(cam.lastSelectedPlayer) >= players.size())
        return;

    Mateyak::Window::drawBox(boxPosX, boxPosY, boxWidth, boxHeight, {0, 39, 97, 94});
    Mateyak::Vec2f position = players[cam.lastSelectedPlayer]->_position;

    // Name + Position
    Mateyak::Window::draw("Player: " + players[cam.lastSelectedPlayer]->getName() + " on tile (" + std::to_string((int)position.x) + ", " + std::to_string((int)position.y) + ")", boxPosX + 20, boxPosY + 20, 25, white);

    // Team
    Mateyak::Window::draw("Team: " + players[cam.lastSelectedPlayer]->getTeam().getName(), boxPosX + 20, boxPosY + 50, 25, white);

    // Level
    Mateyak::Window::draw("Level: " + std::to_string(players[cam.lastSelectedPlayer]->getLevel()), boxPosX + 20, boxPosY + 80, 25, white);

    // Inventory
    std::array<int, 7> inventory = players[cam.lastSelectedPlayer]->getInventory();
    players[cam.lastSelectedPlayer]->ven.isSelected = true;
    boxPosY += 130;
    for (size_t i = 0; i < inventory.size(); i++) {
        Color color = Ressource::clr[i];
        color.a = 255;

        Mateyak::Window::draw(Ressource::resourceName[i] + ": " + std::to_string(inventory[i]), boxPosX + 20, boxPosY, 15, color);
        boxPosY += 20;
    }
}

void Graphic::drawTimeUnit()
{
    std::string str = "Time unit: " + std::to_string(_serverInformations.getTimeUnit());
    Mateyak::Window::draw(str, 10, 35, 25, {255, 255, 255, 255});
}

Graphic::~Graphic()
{
    for (auto &sprite : _textures) {
        UnloadTexture(sprite);
    }
    for (auto &sprite : _menuImage) {
        UnloadImage(sprite);
    }
    _channel->stop();
    _sound->release();
    _system->close();
    _system->release();
}

void Graphic::DrawInfo()
{
    DrawFPS(10, 10);
    drawTeams();
    drawBroadCastMessage();
    drawTileInformation(_cam);
    drawPlayerInformation(_cam);
    drawTimeUnit();
}

void Graphic::handleEvent() {
    if (IsKeyPressed(KEY_F1)) {
        shaderEnabled = !shaderEnabled;
        _shader.setUniform("shaderEnabled", shaderEnabled);
    }
    if (IsKeyPressed(KEY_F2))
        drawGrid = !drawGrid;
}
