/*
** EPITECH PROJECT, 2023
** venom
** File description:
** Camera
*/

#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <regex>
#include <map>
#include "Informations/ServerInformations.hpp"

#define bzero(b, len) (memset((b), '\0', (len)), (void) 0)

class GuiClient
{
    public:
        GuiClient(ServerInformations &_serverInformations, const std::string& ip, const std::string& port);
        void CheckValidServer();
        std::string getInformations();
        std::vector<std::string> splitStrings(std::string str, std::string delimiter);
        void compute();
        void parseOutput(std::string output);
        ~GuiClient() = default;
        void stop();

        void setMapSize(std::vector<std::string> parameters);
        void setMapTiles(std::vector<std::string> parameters);
        void setTeamNames(std::vector<std::string> parameters);
        void NewPlayer(std::vector<std::string> parameters);
        void MovePlayer(std::vector<std::string> parameters);
        void PlayerLevel(std::vector<std::string> parameters);
        void PlayerInventory(std::vector<std::string> parameters);
        void PlayerDeath(std::vector<std::string> parameters);
        void PlayerIncantation(std::vector<std::string> parameters);
        void PlayerIncantationEnd(std::vector<std::string> parameters);
        void PlayerBroadcast(std::vector<std::string> parameters);
        void PlayerExpulse(std::vector<std::string> parameters);
        void PlayerFork(std::vector<std::string> parameters);
        void EggLaying(std::vector<std::string> parameters);
        void EggHatching(std::vector<std::string> parameters);
        void EggConnection(std::vector<std::string> parameters);
        void EggDeath(std::vector<std::string> parameters);
        void ServerTimeUnit(std::vector<std::string> parameters);
        void PlayerTakeRessource(std::vector<std::string> parameters);
        void PlayerDropRessource(std::vector<std::string> parameters);
        void ServerEndGame(std::vector<std::string> parameters);

private:
        boost::asio::io_context _io_context;
        boost::asio::ip::tcp::resolver _resolver;
        boost::asio::ip::tcp::resolver::results_type _endpoints;
        boost::asio::ip::tcp::socket _socket;
        ServerInformations &_serverInformations;
        bool _loop = true;
        std::string _buffer;
};
