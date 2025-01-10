#pragma once
#include <chrono>
// Neded to include glad
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <steam/isteamnetworkingsockets.h>

using namespace std;

class Application {
private:
    void Application_Loop();
    GLFWwindow* window;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    HSteamNetConnection remote_host_connection;
    ISteamNetworkingSockets *connection_api;

public:
    Application();
    virtual string Get_Name() = 0;
    virtual void Start_Application(bool server);
    /**
     * Handles the main game logic and is called every frame.
     * @param deltaTime The game time of this frame in milliseconds
     */
    virtual void Update_Application(std::chrono::milliseconds deltaTime) = 0;
    virtual void End_Application();
    virtual ~Application();
    GLFWwindow& Get_Window() const;
};

void Create_Application(Application& application, bool server);