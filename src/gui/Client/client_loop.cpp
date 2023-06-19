/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_CLIENT_LOOP_CPP
** File description:
** client_loop.cpp
*/

#include "Client/client.hpp"

void GuiClient::parseOutput(std::string re) {
    std::map<std::string, void (GuiClient::*)(std::vector<std::string>)> commands = {
            {"msz", &GuiClient::setMapSize},
            {"bct", &GuiClient::setMapTiles},
            {"tna", &GuiClient::setTeamNames},
            {"pnw", &GuiClient::NewPlayer},
            {"ppo", &GuiClient::MovePlayer},
            {"plv", &GuiClient::PlayerLevel},
            {"pin", &GuiClient::PlayerInventory},
            {"pdi", &GuiClient::PlayerDeath},
            {"pbc", &GuiClient::PlayerBroadcast},
            {"pex", &GuiClient::PlayerExpulse},
            {"pic", &GuiClient::PlayerIncantation},
            {"pie", &GuiClient::PlayerIncantationEnd},
            {"pfk", &GuiClient::PlayerFork},
            {"enw", &GuiClient::EggLaying},
            {"ebo", &GuiClient::EggConnection},
            {"edi", &GuiClient::EggDeath},
            /*
            {"pdr", &GuiClient::PlayerDropRessource},
            {"pgt", &GuiClient::PlayerTakeRessource},
            {"eht", &GuiClient::EggHatching},
            {"sgt", &GuiClient::ServerTimeUnit},
            {"seg", &GuiClient::ServerEndGame},
            {"smg", &GuiClient::ServerMessage},
            {"suc", &GuiClient::ServerUnknownCommand},
            {"sbp", &GuiClient::ServerBadParameter},*/
    };
    std::vector<std::string> res = splitStrings(re, "\n");

    _serverInformations.startComputing();

    for (auto &s : res)
        std::cout << s << std::endl;

    for (auto &resp : res) {
        std::stringstream ss(resp);
        std::string token;
        std::string command;
        std::vector<std::string> parameters;

        ss >> command;
        while (ss >> token)
            parameters.push_back(token);
        if (commands.find(command) != commands.end()) {
            try {
                std::invoke(commands[command], this, parameters);
            } catch (std::exception &e) {}
        } else
            std::cout << "token: " << command << std::endl;
    }
    _serverInformations.endComputing();
}

void GuiClient::stop()
{
    _loop = false;
}

void GuiClient::compute()
{
    std::string resp;

    try {
        while (_loop) {
            if (!_socket.is_open()) {
                std::cout << "Socket closed" << std::endl;
            }
            if (_socket.available() > 0) {
                resp = getInformations();
                parseOutput(resp);
            }
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}