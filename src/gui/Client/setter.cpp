/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_SETTER_CPP
** File description:
** setter.cpp
*/

#include "Client/client.hpp"

void GuiClient::setMapSize(std::vector<std::string> parameters)
{
    if (parameters.size() != 2) {
        std::cerr << "setMapSize: invalid number of parameters" << std::endl;
        return;
    }
    int x = std::stoi(parameters[0]);
    int y = std::stoi(parameters[1]);

    _serverInformations.setMapSize(x, y);
}

void GuiClient::setMapTiles(std::vector<std::string> parameters)
{
    if (parameters.size() != 9) {
        std::cerr << "setMapTiles: invalid number of parameters" << std::endl;
        return;
    }
    int x = std::stoi(parameters[0]);
    int y = std::stoi(parameters[1]);
    std::vector<int> res;

    for (int i = 2; i < 8; i++)
        res.push_back(std::stoi(parameters[i]));
    _serverInformations.setTile(x, y, res);
}

void GuiClient::setTeamNames(std::vector<std::string> parameters)
{
    if (parameters.size() != 1) {
        std::cerr << "setTeamNames: invalid number of parameters" << std::endl;
        return;
    }

    _serverInformations.addTeam(parameters[0]);
}

void GuiClient::NewPlayer(std::vector<std::string> parameters)
{
    if (parameters.size() != 6) {
        std::cerr << "NewPlayer: invalid number of parameters" << std::endl;
        return;
    }
    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
    int x = std::stoi(parameters[1]);
    int y = std::stoi(parameters[2]);
    Player::ORIENTATION orientation = static_cast<Player::ORIENTATION>(std::stoi(parameters[3]));
    int level = std::stoi(parameters[4]);
    std::string team = parameters[5];

    _serverInformations.addPlayer(name, x, y, orientation, level, team);
    _serverInformations.updateAudioAction(std::make_tuple(std::stoi(parameters[1]), std::stoi(parameters[2])), Mateyak::NEWPLAYER);
}

void GuiClient::MovePlayer(std::vector<std::string> parameters)
{
    if (parameters.size() != 4) {
        std::cerr << "MovePlayer: invalid number of parameters" << std::endl;
        return;
    }

    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
    int x = std::stoi(parameters[1]);
    int y = std::stoi(parameters[2]);
    Player::ORIENTATION orientation = static_cast<Player::ORIENTATION>(std::stoi(parameters[3]));

    _serverInformations.movePlayer(name, x, y, orientation);
}

void GuiClient::PlayerLevel(std::vector<std::string> parameters)
{
    if (parameters.size() != 2) {
        std::cerr << "PlayerLevel: invalid number of parameters" << std::endl;
        return;
    }

    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
    int level = std::stoi(parameters[1]);

    _serverInformations.setPlayerLevel(name, level);
}

void GuiClient::PlayerInventory(std::vector<std::string> parameters)
{
    if (parameters.size() != 10) {
        std::cerr << "PlayerInventory: invalid number of parameters" << std::endl;
        return;
    }

    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
//    int x = std::stoi(parameters[1]);
//    int y = std::stoi(parameters[2]);

    std::vector<int> res;

    for (int i = 3; i < 10; i++)
        res.push_back(std::stoi(parameters[i]));
    _serverInformations.setPlayerInventory(name, res);
}

void GuiClient::PlayerDeath(std::vector<std::string> parameters)
{
    if (parameters.size() != 1) {
        std::cerr << "PlayerDeath: invalid number of parameters" << std::endl;
        return;
    }

    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
    _serverInformations.setPlayerDead(name);
}

void GuiClient::PlayerBroadcast(std::vector<std::string> parameters)
{
    if (parameters.size() < 2) {
        std::cerr << "PlayerBroadcast: invalid number of parameters" << std::endl;
        return;
    }

    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
    parameters.erase(parameters.begin());
    std::string message = "";
    for (auto &it : parameters)
        message += it + " ";

    _serverInformations.addBroadCastMessage(name, message);
}

void GuiClient::PlayerExpulse(std::vector<std::string> parameters)
{
    if (parameters.size() != 1) {
        std::cerr << "PlayerExpulse: invalid number of parameters" << std::endl;
        return;
    }

    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
    // TODO: Verifier si le fait de le rendre mort est suffisant ou s'il faut vraiment l'expulser
    _serverInformations.setPlayerDead(name);
}

void GuiClient::PlayerFork(std::vector<std::string> parameters)
{
    if (parameters.size() != 1) {
        std::cerr << "PlayerFork: invalid number of parameters" << std::endl;
        return;
    }
    std::string name = parameters[0];
    if (name[0] == '#')
        name.erase(0, 1);
    _serverInformations.PlayerForkEgg(name);
}

void GuiClient::EggLaying(std::vector<std::string> parameters)
{
    if (parameters.size() != 4) {
        std::cerr << "EggLaying: invalid number of parameters" << std::endl;
        return;
    }
    std::string eggName = parameters[0];
    std::string name = parameters[1];
    if (name[0] == '#')
        name.erase(0, 1);
    int x = std::stoi(parameters[2]);
    int y = std::stoi(parameters[3]);

    _serverInformations.PlayerLayEgg(name, eggName, x, y);
}

void GuiClient::EggConnection(std::vector<std::string> parameters)
{
    if (parameters.size() != 1) {
        std::cerr << "EggConnection: invalid number of parameters" << std::endl;
        return;
    }
    std::string eggName = parameters[0];
    _serverInformations.EggConnection(eggName);
}

void GuiClient::EggDeath(std::vector<std::string> parameters)
{
    if (parameters.size() != 4) {
        std::cerr << "EggDeath: invalid number of parameters" << std::endl;
        return;
    }
    std::string eggName = parameters[0];
    _serverInformations.EggDeath(eggName);
}

void GuiClient::ServerTimeUnit(std::vector<std::string> parameters)
{
    if (parameters.size() != 1) {
        std::cerr << "ServerTimeUnit: invalid number of parameters" << std::endl;
        return;
    }
    int timeUnit = std::stoi(parameters[0]);
    _serverInformations.setTimeUnit(timeUnit);
}

void GuiClient::PlayerIncantation(std::vector<std::string> parameters)
{
    if (parameters.size() < 3) {
        std::cerr << "PlayerIncantationStart: invalid number of parameters" << std::endl;
        return;
    }

    float X = std::stof(parameters[0]);
    float Y = std::stof(parameters[1]);
    int level = std::stoi(parameters[2]);

    for (size_t i = 3; i < parameters.size(); i++) {
        std::string name = parameters[i];
        if (name[0] == '#')
            name.erase(0, 1);

        auto it = std::find_if(_serverInformations.getPlayers().begin(), _serverInformations.getPlayers().end(), [name](std::unique_ptr<Player> &player) {
            return (*player).getName() == name;
        });

        if (it == _serverInformations.getPlayers().end())
            continue;

        // TODO: Besoin de check ca ?? Pourquoi y a X Y ?
        int posX = (*it)->ven.getNextPos().x / (10 / 3.f);
        int posY = (*it)->ven.getNextPos().z / (10 / 3.f);
        if (posX != X || posY != Y)
            continue;

        _serverInformations.setIncantationLevel(name, level);
        _serverInformations.setPlayerState(name, Player::STATE::INCANTING);
    }
    _serverInformations.updateAudioAction(std::make_tuple(std::stoi(parameters[0]), std::stoi(parameters[1])), Mateyak::ELEVATIONSTART);
}

void GuiClient::PlayerIncantationEnd(std::vector<std::string> parameters)
{
    if (parameters.size() != 3) {
        std::cerr << "PlayerIncantationEnd: invalid number of parameters" << std::endl;
        return;
    }

    float X = std::stof(parameters[0]);
    float Y = std::stof(parameters[1]);
    int result = std::stoi(parameters[2]);

    for (auto &it : _serverInformations.getPlayers()) {
        int posX = it->ven.getNextPos().x / (10 / 3.f);
        int posY = it->ven.getNextPos().z / (10 / 3.f);

        if (posX != X || posY != Y)
            continue;
        if (it->getState() != Player::STATE::INCANTING)
            continue;

        if (result == 1) {
            _serverInformations.setPlayerState((*it).getName(), Player::STATE::NONE);
            _serverInformations.setPlayerLevel((*it).getName(), (*it).incantationLevel + 1);
            _serverInformations.setIncantationLevel((*it).getName(), -1);
        } else {
            _serverInformations.setPlayerState((*it).getName(), Player::STATE::NONE);
            _serverInformations.setIncantationLevel((*it).getName(), -1);
        }
    }
    _serverInformations.updateAudioAction(std::make_tuple(std::stoi(parameters[0]), std::stoi(parameters[1])), Mateyak::LEVELUP);

}

void GuiClient::PlayerTakeRessource(std::vector<std::string> parameters)
{
    if (parameters.size() != 2) {
        std::cerr << "PlayerTakeRessource: invalid number of parameters" << std::endl;
        return;
    }
    std::string name = parameters[0];
    int ressource = std::stoi(parameters[1]);

    if (name[0] == '#')
        name.erase(0, 1);
    if (ressource < 0 || ressource > 6) {
        std::cerr << "PlayerTakeRessource: invalid ressource" << std::endl;
        return;
    }
    _serverInformations.takeRessource(name, ressource);
}

void GuiClient::PlayerDropRessource(std::vector<std::string> parameters)
{
    if (parameters.size() != 2) {
        std::cerr << "PlayerDropRessource: invalid number of parameters" << std::endl;
        return;
    }
    std::string name = parameters[0];
    int ressource = std::stoi(parameters[1]);

    if (name[0] == '#')
        name.erase(0, 1);
    if (ressource < 0 || ressource > 6) {
        std::cerr << "PlayerDropRessource: invalid ressource" << std::endl;
        return;
    }
    _serverInformations.dropRessource(name, ressource);
}

void GuiClient::ServerEndGame(std::vector<std::string> parameters)
{
    if (parameters.size() != 1) {
        std::cerr << "ServerEndGame: invalid number of parameters" << std::endl;
        return;
    }
    std::string teamName = parameters[0];
    _serverInformations.setWinner(teamName);
    _serverInformations.setRunning(false);
}