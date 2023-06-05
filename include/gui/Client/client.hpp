/*
** EPITECH PROJECT, 2023
** venom
** File description:
** Camera
*/

#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <memory>
#include <vector>
#include <cstring>
#include <thread>

#define bzero(b, len) (memset((b), '\0', (len)), (void) 0)

class Client
{
public:
    Client(std::string ip, int port);
    ~Client();
    int connect_client();
    void send_message(std::string str);
    std::string receive_message();
    void operator <<(std::string str);
    void operator >>(std::string &str);

private:
    void write_message(std::string str);
    int _sockfd;
    struct sockaddr_in _servaddr;
    std::string _ip;
    int _port;
    std::string _buffer;
};

void computeClient(Client &client);
