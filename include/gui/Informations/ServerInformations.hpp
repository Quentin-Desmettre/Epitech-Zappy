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
#include <queue>
#include <chrono>

typedef std::vector<std::vector<std::vector<Ressource>>> ZappyMap;


class Message {
    public:
        Message(const std::string &name, const std::string &message, Color color);
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

        bool isRunning() const;
        void setRunning(bool running);

        void updatePlayer(std::unique_ptr<Player> &player);

        void setMapSize(int x, int y);
        void setTile(int x, int y, std::vector<int> values);
        void addTeam(const std::string &team);
        void addPlayer(const std::string &name, int x, int y, Player::ORIENTATION orientation, int level, const std::string &team);
        void movePlayer(const std::string &name, int x, int y, Player::ORIENTATION orientation);
        void setPlayerState(const std::string &name, Player::STATE state);
        void setPlayerLevel(const std::string &name, int level);
        void setPlayerInventory(const std::string &name, std::vector<int> inventory);
        void removePlayer(const std::string &name);
        void setPlayerDead(const std::string &name);
        void addBroadCastMessage(const std::string &name, const std::string &message);
        void setIncantationLevel(const std::string &name, int level);
        void PlayerForkEgg(const std::string &name);
        void PlayerLayEgg(const std::string &name, const std::string &eggName, int x, int y);
        void EggConnection(const std::string &eggName);
        void EggDeath(const std::string &eggName);
        void setTimeUnit(int timeUnit);
        int getTimeUnit() const;
        void takeRessource(const std::string &name, int ressource);
        void dropRessource(const std::string &name, int ressource);
        Mateyak::Vec2f getMapSize() const;
        void updateAudioAction(std::tuple<int, int> pos, enum Mateyak::action_type type);
        void audioActionsHandler(Mateyak::Camera &camera);
        ZappyMap getMap() const;
        std::vector<Team> getTeams() const;
        std::vector<std::unique_ptr<Player>> &getPlayers();
        std::vector<Message> &getBroadCastMessage();

        std::queue<std::string> &getCommandQueue();
        void addCommand(std::string command);
        std::string getCommand();
        bool hasCommand() const;

        void updateTimeUnit();
        void setWinner(const std::string &teamName);

    private:
        FMOD::System *_systemAudio;
        Mateyak::Vec2f mapSize {0, 0};
        ZappyMap map;
        std::string _winner;
        std::vector<Team> teams{};
        std::vector<std::unique_ptr<Player>> players{};
        std::vector<Message> broadCastMessage{};
        std::mutex mutex;
        int _timeUnit = 0;
        int _newTimeUnit = -1;
        std::chrono::time_point<std::chrono::system_clock> _lastKeyPressedTime;

        bool _serverRunning = true;
        std::vector<std::tuple<short, std::vector<std::tuple<int, int, std::shared_ptr<Mateyak::Audio>>>>> audioAction;

        std::queue<std::string> _commandQueue;
};
