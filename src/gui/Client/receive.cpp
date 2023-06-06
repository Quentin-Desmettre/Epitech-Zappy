/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_RECEIVE_CPP
** File description:
** receive.cpp
*/

#include "Client/client.hpp"

std::string GuiClient::getInformations()
{
    char response[1024];
    std::string buf;
    size_t bytesReceived;

    do {
        bzero(response, 1024);
        bytesReceived = _socket.read_some(boost::asio::buffer(response));
        buf += response;
    } while (bytesReceived == 1024);
    return buf;
}

std::vector<std::string> GuiClient::splitStrings(std::string str, std::string delimiter)
{
    size_t pos;
    std::string token;
    std::vector<std::string> res;

    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (str.length() > 0)
        res.push_back(str);
    return res;
}
