#pragma once

#include "networking//Network.h"

#include <chrono>
#include <cstdint>

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
    Application(std::string name, bool client, uint16_t screen_width, uint16_t screen_height);
    virtual ~Application() = default;

    uint16_t screen_width;
    uint16_t screen_height;

    // Required logical update function
    virtual void Update(chrono::milliseconds, Application&) = 0;
    // Optional client functions
    virtual void Init_Client() {}
    virtual void Render(chrono::milliseconds, Application&) {};

    void Start_Application();
    void Start_Server();
    void Start_Client();

    string Get_Name();
    Network* Get_Network();

    void Close_Application();
    void Close_Network();
};

void Create_Application(unique_ptr<ApplicationFactory> application_factory, bool client);
