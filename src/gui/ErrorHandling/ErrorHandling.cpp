/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_ERRORHANDLING_CPP
** File description:
** ErrorHandling.cpp
*/

#include "ErrorHandling.hpp"
#include <thread>
#include <string>
#include "Graphic.hpp"
#include "Client/client.hpp"

void ErrorHandling::RunningLoop(const std::string &ip, const std::string &port, Graphic &graphic, bool &backMenu)
{
    GuiClient client(serverInformations, ip, port);
    client.CheckValidServer();
    graphic.setMapSize(serverInformations.getMapSize());
    std::thread t(&GuiClient::compute, &client);
    SetExitKey(0);
    if (graphic.loop()) {
        backMenu = true;
        SetExitKey(KEY_ESCAPE);
    }
    client.stop();
    t.join();
}

void ErrorHandling::run_without_parameters()
{
    std::string ip = "127.0.0.1";
    std::string port = "4242";
    Graphic graphic({0, 0}, {1920 / 1.2, 1080 / 1.2}, serverInformations);
    int needToContinue;
    bool backMenu = false;
    bool error = false;

    while (true) {
        std::string winner = serverInformations.getWinner();
        if (!winner.empty()) {
            winner.insert(0, "The winner is ");
            winner.insert(winner.size(), " !");
        }
        needToContinue = 0;
        serverInformations.clear();
        if (!graphic.menu(ip, port, error, winner))
            return;
        error = false;
        try {
            RunningLoop(ip, port, graphic, backMenu);
        } catch (const std::exception& ex) {
            needToContinue = 1;
        }
        if (needToContinue) {
            error = true;
            continue;
        }
        if (backMenu) continue;
        return;
    }
}

void ErrorHandling::run_with_parameter()
{
    GuiClient client(serverInformations, getIp(), getPort());
    client.CheckValidServer();
    Graphic graphic(serverInformations.getMapSize(), {1920 / 1.2, 1080 / 1.2}, serverInformations);
    std::thread t(&GuiClient::compute, &client);
    graphic.loop();
    client.stop();
    t.join();
    const std::string winner = serverInformations.getWinner();
    if (!winner.empty()) {
        std::cout << "The winner is " << winner << std::endl;
    }
}

ErrorHandling::ErrorHandling(int ac, char **av) : _ac(ac), _av(av)
{
    if (ac == 1) return;
    if (!(ac == 3 || ac == 5))
        throw Error("Invalid number of arguments");
}

void ErrorHandling::parse()
{
    if (_ac == 1) return;
    for (int i = 1; i < _ac; i++) {
        if (std::string(_av[i]) == "-p") {
            if (i + 1 >= _ac)
                throw Error("Port not defined after -p");
            _port = _av[i + 1];
            i++;
        }
        else if (std::string(_av[i]) == "-h") {
            if (i + 1 >= _ac)
                throw Error("Ip not defined after -h");
            _ip = _av[i + 1];
            i++;
        }
        else {
            std::string test(_av[i]);
            throw Error(test + std::string(" is not a valid argument"));
        }
    }
    if (_port.empty())
        throw Error("Port not defined");
}

std::string ErrorHandling::getPort() const
{
    return _port;
}

std::string ErrorHandling::getIp() const
{
    if (_ip.empty())
        return std::string("127.0.0.1");
    return _ip;
}

void ErrorHandling::Run()
{
    if (_ac == 1) {
        run_without_parameters();
    } else {
        run_with_parameter();
    }
}