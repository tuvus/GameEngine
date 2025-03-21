#include "Application.h"

#include <chrono>
#include <iostream>
#include <thread>
#include "ApplicationFactory.h"
#include "ApplicationWindow.h"

using namespace std;

void Create_Application(unique_ptr<ApplicationFactory> application_factory, bool client) {
    cout << "Creating application: " + application_factory->Get_Name() << endl << endl;
    const auto application = make_unique<Application>(move(application_factory), client);
    application->Start_Application();
}

Application::Application(unique_ptr<ApplicationFactory> application_factory, bool client): client(client),
    application_name(application_factory->Get_Name()), application_state(ApplicationState::SettingUp), network(nullptr),
    update_function(application_factory->Create_Update_Function()) {
    if (client) application_window = application_factory->Create_Window(*this);
}

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
    network = make_unique<Network>(true, [this] { this->Close_Network(); });
}

void Application::Start_Client() {
    network = make_unique<Network>(false, [this] { this->Close_Network(); });
}

void Application::Close_Network() {
    network.reset();
}

string Application::Get_Name() {
    return application_name;
}

Network& Application::Get_Network() {
    return *network;
}

void Application::Application_Loop() {
    application_state = ApplicationState::Running;
    chrono::time_point<chrono::system_clock> frame_end_time = chrono::system_clock::now();
    while (application_state == ApplicationState::Running) {
        chrono::time_point<chrono::system_clock> frame_start_time = chrono::system_clock::now();
        auto delta_time = chrono::duration_cast<std::chrono::milliseconds>(frame_start_time - frame_end_time);

        if (network) network->Network_Update();
        if (client) application_window->Render_Update(delta_time);

        update_function(delta_time, *this);

        this_thread::sleep_until(frame_start_time + 16ms);
        frame_end_time = chrono::system_clock::now();
    }
    Close_Application();
}

void Application::Close_Application() {
    application_state = ApplicationState::Closing;
}
