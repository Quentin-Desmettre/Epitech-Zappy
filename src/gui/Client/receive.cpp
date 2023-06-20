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
        for (size_t i = 0; i < bytesReceived; i++)
            buf += response[i];
        if (bytesReceived == 0)
            throw std::runtime_error("Server disconnected");
    } while (bytesReceived == 1024);
    return buf;
}

std::vector<std::string> GuiClient::splitStrings(std::string toParse, std::string delimiter)
{
    size_t pos;
    std::string token;
    std::vector<std::string> res;
    std::string str = _buffer + toParse;

    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    _buffer.clear();
    if (str.length() > 0)
        _buffer += str;
    return res;
}
