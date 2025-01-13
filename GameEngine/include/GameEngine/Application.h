#pragma once
#include <chrono>
#include <steam/isteamnetworkingsockets.h>

using namespace std;

class ApplicationWindow;
class ApplicationFactory;

enum class ApplicationState {
    SettingUp,
    Running,
    Closing,
};

class Application {
private:
    bool client;
    string application_name;
    ApplicationState application_state;
    ApplicationWindow* application_window;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    HSteamNetConnection remote_host_connection;
    ISteamNetworkingSockets *connection_api;
    void Application_Loop();

public:
    Application(ApplicationFactory& application_factory, bool client);
    void Start_Application();
    string Get_Name();
    void Close_Application();
    ~Application();
};

void Create_Application(ApplicationFactory& application_factory, bool client);