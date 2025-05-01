#include "Application.h"
#include "ApplicationFactory.h"

#include <iostream>
#include <raylib.h>
#include <thread>

void Create_Application(unique_ptr<ApplicationFactory> application_factory, bool client)
{
    const auto application = application_factory->Create_Application(client);
    cout << "Creating application: " << application->Get_Name() << endl << endl;
    application->Start_Application();
}

Application::Application(std::string name, bool client, uint16_t screen_width,
                         uint16_t screen_height)
    : application_name(name), client(client), screen_width(screen_width),
      screen_height(screen_height), application_state(ApplicationState::SettingUp), network(nullptr)
{
}

void Application::Start_Application()
{
    cout << "Starting application: " + Get_Name() << endl;
    if (client)
    {
        Start_Client();
    }
    else
    {
        Start_Server();
    }

    Application_Loop();
}

void Application::Start_Server()
{
    network = make_unique<Network>(true, [this] { this->Close_Network(); });
}

void Application::Start_Client()
{
    network = make_unique<Network>(false, [this] { this->Close_Network(); });

    InitWindow(screen_width, screen_height, application_name.c_str());
    SetTargetFPS(60);
    init_client();
}

void Application::Close_Network()
{
    network.reset();
}

string Application::Get_Name()
{
    return application_name;
}

Network* Application::Get_Network()
{
    return network.get();
}

void Application::Application_Loop()
{
    application_state = ApplicationState::Running;
    chrono::time_point<chrono::system_clock> frame_end_time = chrono::system_clock::now();
    while (application_state == ApplicationState::Running)
    {
        chrono::time_point<chrono::system_clock> frame_start_time = chrono::system_clock::now();
        auto delta_time =
            /*chrono::duration_cast<std::chrono::milliseconds>(frame_start_time - frame_end_time);*/
            16ms; // TODO: ???

        if (network)
            network->Network_Update();

        update(delta_time, *this);

        if (client)
        {
            render(delta_time, *this);
        }

        this_thread::sleep_until(frame_start_time + 16ms);
        frame_end_time = chrono::system_clock::now();
    }
    Close_Application();
}

void Application::Close_Application()
{
    application_state = ApplicationState::Closing;
}
