/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_SEND_CPP
** File description:
** send.cpp
*/

#include "Client/client.hpp"

void Client::write_message(std::string str)
{
    _buffer += str;
    size_t size = write(_sockfd, _buffer.c_str(), str.size());

    if (size > 0 && size != str.size()) {
        _buffer = _buffer.substr(size);
        std::cout << "sent but rest" << std::endl;
    } else {
        _buffer = "";
        std::cout << "sent" << std::endl;
    }
}

void Client::send_message(std::string str)
{
    fd_set writefds;
    int max_sd = _sockfd;
    timeval tv = {0, 0};

    FD_SET(_sockfd, &writefds);
    int activity = select(max_sd + 1, NULL, &writefds, NULL, &tv);
    if (activity < 0)
        std::cerr << "select error" << std::endl;
    if (FD_ISSET(_sockfd, &writefds)) {
        write_message(str);
    } else {
        _buffer += str;
    }
}

void Client::operator <<(std::string str)
{
    send_message(str);
}
