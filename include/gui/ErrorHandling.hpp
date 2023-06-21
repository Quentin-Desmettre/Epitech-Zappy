/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** ErrorHandling.hpp
*/

#ifndef EPITECH_ZAPPY_ERRORHANDLING_HPP
#define EPITECH_ZAPPY_ERRORHANDLING_HPP

#include <exception>
#include <string>
#include "Graphic.hpp"

class ErrorHandling {
public:
    class Error : public std::exception {
    public:
        Error(std::string const &message) : _message(message) {}
        const char *what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message;
    };
    void parse();
    ErrorHandling(int ac, char **av);
    std::string getPort() const;
    std::string getIp() const;
    ~ErrorHandling() = default;
    void RunningLoop(const std::string &ip, const std::string &port, Graphic &graphic, bool &backMenu);
    void run_without_parameters();
    void run_with_parameter();
    void Run();
private:
    int _ac;
    char **_av;
    std::string _port;
    std::string _ip;
    ServerInformations serverInformations;
};


#endif //EPITECH_ZAPPY_ERRORHANDLING_HPP