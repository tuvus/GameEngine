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
    application_window(application_factory.Create_Window(*this)) {}

void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) {
    // Temporary here
}

void Application::Start_Application() {
    cout << "Starting application: " + Get_Name() << endl;
    SteamNetworkingIPAddr addrServer;
    addrServer.Clear();
    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg)) {
        cerr << "GameNetworkingSockets initialization failed " << err_msg << endl;
        exit(-1);
    }

    connection_api = SteamNetworkingSockets();
    char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
    addrServer.ToString(szAddr, sizeof(szAddr), true);
    SteamNetworkingConfigValue_t config_options;
    config_options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)OnConnectionStatusChanged);
    if (client) {
        remote_host_connection = connection_api->ConnectByIPAddress(addrServer, 1, &config_options);
        if (remote_host_connection == k_HSteamNetConnection_Invalid) cerr << "Failed to connect to the host" << endl;
    } else {
        listen_socket = connection_api->CreateListenSocketIP(addrServer, 1, &config_options);
        if (listen_socket == k_HSteamListenSocket_Invalid) cerr << "Failed to setup listener on port " << 0 << endl;
        poll_group = connection_api->CreatePollGroup();
        if (poll_group == k_HSteamNetPollGroup_Invalid) cerr << "Failed to setup listener on port " << 0 << endl;
    }
    application_window->Start_Application_Window();
    Application_Loop();
}


string Application::Get_Name() {
    return application_name;
}


void Application::Application_Loop() {
    application_state = ApplicationState::Running;
    chrono::time_point<chrono::system_clock> frame_end_time = chrono::system_clock::now();
    while (application_state == ApplicationState::Running) {
        chrono::time_point<chrono::system_clock> frame_start_time = chrono::system_clock::now();
        application_window->Render_Update(
            chrono::duration_cast<std::chrono::milliseconds>(frame_start_time - frame_end_time));
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
