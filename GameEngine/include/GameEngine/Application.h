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

    uint16_t screen_width;
    uint16_t screen_height;

    void Application_Loop();

  public:
    Application(std::string name, bool client, uint16_t screen_width, uint16_t screen_height);
    virtual ~Application() = default;

    // required logical update function
    virtual void update(chrono::milliseconds, Application&) = 0;
    // optional client functions
    virtual void init_client() {}
    virtual void render(chrono::milliseconds, Application&) {};

    void Start_Application();
    void Start_Server();
    void Start_Client();

    string Get_Name();
    Network* Get_Network();

    void Close_Application();
    void Close_Network();
};

void Create_Application(unique_ptr<ApplicationFactory> application_factory, bool client);
