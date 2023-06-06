/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_CLIENT_CPP
** File description:
** client.cpp
*/

#include "Client/client.hpp"

GuiClient::GuiClient(ServerInformations &_serverInformations, std::string ip, std::string port) : _resolver(_io_context), _socket(_io_context), _serverInformations(_serverInformations) {
    _endpoints = _resolver.resolve(ip, port);
    boost::asio::connect(_socket, _endpoints);
}

bool GuiClient::CheckValidServer()
{
    std::string message = "GRAPHIC\n";
    _socket.send(boost::asio::buffer(message));

    std::string resp = getInformations();
    if (resp != "WELCOME\n")
        return false;

    resp = getInformations();
    parseOutput(resp);
    return true;
}
