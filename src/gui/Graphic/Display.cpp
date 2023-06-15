/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_DISPLAY_CPP
** File description:
** Display.cpp
*/

#include "Graphic.hpp"
#include <map>

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

void Graphic::drawTileInformation(Mateyak::Window &win, Mateyak::Camera &cam)
{
    float boxPosY = _windowHeight - _windowHeight / 3 - 30;
    float boxPosX = _windowWidth - (_windowWidth / 3 - 20) - 20;
    float boxWidth = _windowWidth / 3 - 10;
    float boxHeight = _windowHeight / 3 + 20;

    if (_charSize.x == 0 && _charSize.y == 0)
        _charSize = MeasureTextEx(win._font, "Z", 15, 1);

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

    boxPosY += 30;
    for (auto &it : tileResources) {
        std::string str = it.first + ": " + std::to_string(it.second.first);
        Color tmp = Ressource::clr[it.second.second];
        tmp.a = 255;
        Mateyak::Window::draw(str, boxPosX + 20, boxPosY + 20, 25, tmp);
        boxPosY += 30;
    }
}

void Graphic::drawPlayerInformation(Mateyak::Window &win, Mateyak::Camera &cam)
{
    int playerSelected = -1;
    float min = 0.5f;
    float diff;
    float len = 0;
    std::vector<std::unique_ptr<Player>> &players = _serverInformations.getPlayers();
    Mateyak::Vec3f dir = cam._lastClickDir.Normalize();
    Mateyak::Vec3f camPos = cam._position;
    Mateyak::Vec3f playerPos;

    for (int i = 0; i < players.size(); i++) {
        playerPos = players[i]->ven.getPos();
        playerPos.y = 0.3;
        diff = ((playerPos - camPos).Normalize() - dir).len();
        len = (playerPos - camPos).len();
        std::cout << diff * len << std::endl;
        if (diff * len < min) {
            min = diff;
            playerSelected = i;
        }
    }
    if (playerSelected == -1) {
//        if (players.size() > 0)
//            players[0]->ven.setState(Player::STATE::NONE);
        //std::cout << "No player selected" << std::endl;
        return;
    }
//    players[playerSelected]->ven.setState(Player::STATE::EGGHATCHING);
    std::cout << "Player selected: " << playerSelected << std::endl;
}
