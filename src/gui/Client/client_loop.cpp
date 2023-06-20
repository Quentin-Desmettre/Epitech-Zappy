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
            {"sgt", &GuiClient::ServerTimeUnit},
            {"pgt", &GuiClient::PlayerTakeRessource},
            {"pdr", &GuiClient::PlayerDropRessource},
            /*
            {"eht", &GuiClient::EggHatching},
            {"smg", &GuiClient::ServerMessage},
            {"seg", &GuiClient::ServerEndGame},
            {"suc", &GuiClient::ServerUnknownCommand},
            {"sbp", &GuiClient::ServerBadParameter},*/
    };
    std::vector<std::string> res = splitStrings(re, "\n");

    _serverInformations.startComputing();

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
                if (command == "pfk" || command == "enw" || command == "ebo" || command == "edi") {
                    std::cout << command << " ";
                    for (auto &param : parameters)
                        std::cout << param << " ";
                    std::cout << std::endl;
                }
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

void resetFds(fd_set &tmp, int socketFD)
{
    FD_ZERO(&tmp);
    FD_SET(socketFD, &tmp);
}

void GuiClient::compute()
{
    std::string resp;
    int socket_descriptor = _socket.native_handle();
    fd_set read_fds;
    fd_set write_fds;

    try {
        while (_loop) {
            if (!_socket.is_open())
                break;
            resetFds(read_fds, socket_descriptor);
            resetFds(write_fds, socket_descriptor);
            select(socket_descriptor + 1, &read_fds, &write_fds, NULL, NULL);
            if (FD_ISSET(socket_descriptor, &read_fds)) {
                resp = getInformations();
                parseOutput(resp);
            }
            if (FD_ISSET(socket_descriptor, &write_fds) && _serverInformations.hasCommand()) {
                _serverInformations.startComputing();
                std::string command = _serverInformations.getCommand();
                _socket.write_some(boost::asio::buffer(command));
                _serverInformations.endComputing();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    _serverInformations.setRunning(false);
}