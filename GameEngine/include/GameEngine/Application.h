#pragma once

#include "networking//Network.h"

#include <chrono>

using namespace std;

class ApplicationWindow;
class ApplicationFactory;

enum class ApplicationState
{
    SettingUp,
    Running,
    Closing,
};

class Application
{
  private:
    bool client;
    string application_name;
    ApplicationState application_state;
    unique_ptr<Network> network;

    void Application_Loop();

  public:
    Application(std::string name, bool client);
    virtual ~Application() = default;

    virtual void update_function(chrono::milliseconds, Application&) = 0;
    virtual void render_function(chrono::milliseconds, Application&) = 0;

    void Start_Application();
    void Start_Server();
    void Start_Client();

    string Get_Name();
    Network* Get_Network();

    void Close_Application();
    void Close_Network();
};

void Create_Application(unique_ptr<ApplicationFactory> application_factory, bool client);
