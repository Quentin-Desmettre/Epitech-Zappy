/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_CLIENT_CPP
** File description:
** client.cpp
*/

#include "Client/client.hpp"

Client::Client(std::string ip, int port) : _ip(ip), _port(port)
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd == -1) {
        std::cerr << "socket creation failed...\n";
        return;
    }

    bzero(&_servaddr, sizeof(_servaddr));
    _servaddr.sin_family = AF_INET;
    _servaddr.sin_addr.s_addr = inet_addr(_ip.c_str());
    _servaddr.sin_port = htons(_port);

    if (connect(_sockfd, (struct sockaddr*)&_servaddr, sizeof(_servaddr)) != 0) {
        std::cerr << "connection with the server failed...\n";
        return;
    }
    else
        std::cout << "connected to the server\n";
}

Client::~Client()
{
    close(_sockfd);
}

int Client::connect_client()
{
    std::string str = "GRAPHIC\n";
    std::string resp = "WELCOME\n";
    char buffer[1024];

    write(_sockfd, str.c_str(), str.size());
    bzero(buffer, 1024);
    read(_sockfd, buffer, 1024);
    for (int i = 0; i < 9; i++)
        if (buffer[i] != resp[i])
            return 84;
    std::cout << buffer << std::endl;
    return 0;
}
