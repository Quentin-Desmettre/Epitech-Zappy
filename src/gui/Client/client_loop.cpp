/*
** EPITECH PROJECT, 2023
** EPITECH_ZAPPY_CLIENT_LOOP_CPP
** File description:
** client_loop.cpp
*/

#include "Client/client.hpp"

void computeClient(Client &client)
{
    std::string resp;
    std::vector<std::string> commands = {"msz", "bct", "tna", "pnw", "ppo",
        "plv", "pin", "pex", "pbc", "pic", "pie", "pfk", "pdr", "pgt", "pdi", "enw",
        "eht", "ebo", "edi", "sgt", "seg", "smg", "suc", "sbp"};


    for (auto &command : commands) {
        client << command + "\n";
        client >> resp;
        std::cout << resp << std::endl;
    }
}
