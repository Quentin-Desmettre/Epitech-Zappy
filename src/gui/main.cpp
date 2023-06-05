/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** main
*/

#include "Client/client.hpp"
#include "Mateyak/Vector2.hpp"


class Player
{
    enum STATE {INCANTING, DEAD, EGG, BROADCASTING, DROPING, TAKING};

    public:
        Player() = default;
        ~Player() = default;
    private:
        Mateyak::Vec2f position;
        int level;
        int inventory[7];
        int orientation;
        int team;
        STATE state;
        int eggTime;
        std::string broadcastMessage;
};


class ServerInformations
{
    public:
        ServerInformations() = default;
        ~ServerInformations() = default;
    private:
        Mateyak::Vec2f mapSize;
        std::vector<std::vector<int>> map;
        std::vector<std::string> teams;
        std::vector<Player> players;
};

int main()
{
    std::cout << "Awaiting connection" << std::endl;
    Client client("127.0.0.1", 4242);

    if (client.connect_client()) {
        std::cout << "Not connected to a nice server" << std::endl;
        return 84;
    }
    client.receive_message();

    std::cout << "Game can start now" << std::endl;
    std::thread t1(computeClient, std::ref(client));

    t1.join();
    return 0;
}

//(bct) ([0-9]+ ){8}[0-9]+
//(msz) [0-9]+ [0-9]+
//(sgt) [0-9]+
//(tna) [a-zA-Z0-9]+
