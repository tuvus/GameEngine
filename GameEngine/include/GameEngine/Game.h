#pragma once
#include <chrono>
// Neded to include glad
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <steam/isteamnetworkingsockets.h>

using namespace std;

class Game {
private:
    void GameLoop();
    GLFWwindow* window;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    HSteamNetConnection remote_host_connection;
    ISteamNetworkingSockets *connection_api;

public:
    Game();
    virtual string GetName() = 0;
    virtual void StartGame(bool server);
    /**
     * Handles the main game logic and is called every frame.
     * @param deltaTime The game time of this frame in milliseconds
     */
    virtual void UpdateGame(std::chrono::milliseconds deltaTime) = 0;
    virtual void EndGame();
    virtual ~Game();
    GLFWwindow& GetWindow() const;
};

void CreateGame(Game& game, bool server);