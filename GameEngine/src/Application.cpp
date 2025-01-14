#include "Application.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>
#include "ApplicationFactory.h"
#include "ApplicationWindow.h"

using namespace std;

void Create_Application(ApplicationFactory& application_factory, bool client) {
    cout << "Creating application: " + application_factory.Get_Name() << endl << endl;
    const auto application = new Application(application_factory, client);
    application->Start_Application();
    delete application;
}

Application::Application(ApplicationFactory& application_factory, bool client): client(client),
    application_name(application_factory.Get_Name()), application_state(ApplicationState::SettingUp),
    application_window(application_factory.Create_Window(*this)), network(nullptr) {}

void Application::Start_Application() {
    cout << "Starting application: " + Get_Name() << endl;
    if (client) {
        application_window->Start_Application_Window();
    } else {
        Start_Server();
    }

    Application_Loop();
}

void Application::Start_Server() {
    network = new Network(true);
}

void Application::Start_Client() {
    network = new Network(false);
}



string Application::Get_Name() {
    return application_name;
}


void Application::Application_Loop() {
    application_state = ApplicationState::Running;
    chrono::time_point<chrono::system_clock> frame_end_time = chrono::system_clock::now();
    while (application_state == ApplicationState::Running) {
        chrono::time_point<chrono::system_clock> frame_start_time = chrono::system_clock::now();

        if (client) {
            application_window->Render_Update(
                chrono::duration_cast<std::chrono::milliseconds>(frame_start_time - frame_end_time));
        }

        this_thread::sleep_until(frame_start_time + 16ms);
        frame_end_time = chrono::system_clock::now();
    }
    Close_Application();
}

void Application::Close_Application() {
    application_state = ApplicationState::Closing;
}

Application::~Application() {
    delete application_window;
    GameNetworkingSockets_Kill();
};
