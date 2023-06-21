/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_CLIENT_CPP
** File description:
** client.cpp
*/

#include "Client/client.hpp"
#include "ErrorHandling.hpp"

void handleTimeout(const boost::system::error_code& error, boost::asio::ip::tcp::socket& socket, bool& timeout) {
    if (!error) {
        timeout = true;
        socket.cancel();
    }
}

GuiClient::GuiClient(ServerInformations &_serverInformations, const std::string &ip, const std::string &port) : _resolver(_io_context), _socket(_io_context), _serverInformations(_serverInformations) {
    _endpoints = _resolver.resolve(ip, port);
    bool timeout = false;
    _socket.async_connect(*_endpoints.begin(), [](const boost::system::error_code& error) {
        if (error)
            throw ErrorHandling::Error("Connection failed");
    });

    // Set the timeout duration (in seconds)
    int timeoutDuration = 1;
    std::cout << "Connecting to " << ip << ":" << port << "..." << std::endl;

    // Create a timer to handle the timeout
    boost::asio::deadline_timer timer(_io_context);
    timer.expires_from_now(boost::posix_time::seconds(timeoutDuration));
    timer.async_wait([this, &timeout](const boost::system::error_code& error) {
        handleTimeout(error, _socket, timeout);
    });

    // Run the io_context to start the asynchronous operations
    _io_context.run();
    std::cout << "Connected" << std::endl;
}

void GuiClient::CheckValidServer()
{
    std::string message = "GRAPHIC\n";
    std::string resp = getInformations();

    if (resp != "WELCOME\n")
        throw ErrorHandling::Error("Invalid server");
    _socket.send(boost::asio::buffer(message));
    resp = getInformations();
    parseOutput(resp);
}
