#pragma once
#include <chrono>

#include "networking//Network.h"

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
    Network* network;

    void Application_Loop();

public:
    Application(ApplicationFactory& application_factory, bool client);
    void Start_Application();
    void Start_Server();
    void Start_Client();
    string Get_Name();
    void Close_Application();
    ~Application();
};

void Create_Application(ApplicationFactory& application_factory, bool client);