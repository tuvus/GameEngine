#pragma once

#include <chrono>
#include <functional>
#include <memory>

using namespace std;

#include "networking//Network.h"

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
    function<void(std::chrono::milliseconds, Application&)> update_function;

    void Application_Loop();

  public:
    Application(unique_ptr<ApplicationFactory> application_factory, bool client);
    void Start_Application();
    void Start_Server();
    void Start_Client();
    void Close_Network();
    string Get_Name();
    void Close_Application();
    Network* Get_Network();
    ~Application() = default;
};

void Create_Application(unique_ptr<ApplicationFactory> application_factory, bool client);
