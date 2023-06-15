/*
** EPITECH PROJECT, 2023
** Epitech-Zappy
** File description:
** ServerInformations.hpp
*/

#pragma once

#include <string>
#include <vector>
#include <array>
#include <mutex>
#include "Mateyak/Vector2.hpp"
#include "Mateyak/Audio.hpp"
#include "Mateyak/Wave.hpp"
#include <algorithm>
#include "Informations/Player.hpp"
#include "Informations/Team.hpp"
#include "Ressource.hpp"
#include <memory>
typedef std::vector<std::vector<std::vector<Ressource>>> ZappyMap;


class Message {
    public:
        Message(std::string name, std::string message, Color color);
        void FormatMessage(int maxLineSize);
        ~Message() = default;
        bool _formated;
        std::string _name;
        std::string _message;
        std::vector<std::string> _lines;
        Color _color;
};

class ServerInformations
{
    public:
        ServerInformations();
        ~ServerInformations();

        void startComputing();
        void endComputing();

        void updatePlayer(std::unique_ptr<Player> &player);

        void setMapSize(int x, int y);
        void setTile(int x, int y, std::vector<int> values);
        void addTeam(std::string team);
        void addPlayer(std::string name, int x, int y, Player::ORIENTATION orientation, int level, std::string team);
        void movePlayer(std::string name, int x, int y, Player::ORIENTATION orientation);
        void setPlayerState(std::string name, Player::STATE state);
        void setPlayerLevel(std::string name, int level);
        void setPlayerInventory(std::string name, std::vector<int> inventory);
        void removePlayer(std::string name);
        void setPlayerDead(std::string name);
        void addBroadCastMessage(std::string name, std::string message);
        void setIncantationLevel(std::string name, int level);
        void PlayerForkEgg(std::string name);
        void PlayerLayEgg(std::string name, std::string eggName, int x, int y);
        void EggConnection(std::string eggName);
        void EggDeath(std::string eggName);

        void updateAudioAction(std::tuple<int, int> pos, enum Mateyak::action_type type);
        void audioActionsHandler(Mateyak::Camera &camera);

    Mateyak::Vec2f getMapSize() const;
        ZappyMap getMap() const;
        std::vector<Team> getTeams() const;
        std::vector<std::unique_ptr<Player>> &getPlayers();
        std::vector<Message> &getBroadCastMessage();

    private:
        FMOD::System *_systemAudio;
        Mateyak::Vec2f mapSize {0, 0};
        ZappyMap map;
        std::vector<Team> teams{};
        std::vector<std::unique_ptr<Player>> players{};
        std::vector<Message> broadCastMessage{};
        std::mutex mutex;
        bool _serverRunning = true;
        std::vector<std::tuple<short, std::vector<std::tuple<int, int, std::shared_ptr<Mateyak::Audio>>>>> audioAction;
};
