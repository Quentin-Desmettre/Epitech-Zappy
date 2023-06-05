/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_RECEIVE_CPP
** File description:
** receive.cpp
*/

#include "Client/client.hpp"

std::string Client::receive_message()
{
    fd_set readfds;
    int max_sd = _sockfd;

    FD_SET(_sockfd, &readfds);
    int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    if (activity < 0)
        std::cerr << "select error" << std::endl;
    if (FD_ISSET(_sockfd, &readfds)) {
        int readableSize = 0;

        if (ioctl(_sockfd, FIONREAD, &readableSize) < 0) {
            std::cerr << "ioctl error" << std::endl;
            return "";
        }
        auto buffer = std::make_unique<char[]>(readableSize);
        read(_sockfd, buffer.get(), readableSize);
        return buffer.get();
    }
    return "";
}
void Client::operator >>(std::string &str)
{
    str = receive_message();
}
