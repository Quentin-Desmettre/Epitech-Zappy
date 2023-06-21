/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_ERRORHANDLING_CPP
** File description:
** ErrorHandling.cpp
*/

#include "ErrorHandling.hpp"

ErrorHandling::ErrorHandling(int ac, char **av) : _ac(ac), _av(av)
{
    if (!(ac == 3 || ac == 5))
        throw Error("Invalid number of arguments");
}

void ErrorHandling::parse()
{
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