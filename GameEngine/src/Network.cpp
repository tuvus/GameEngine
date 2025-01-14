#include "Network.h"

#include <iostream>
#include <steam/steamnetworkingsockets.h>

using namespace std;

Network::Network(bool server) : server(server) {
    SteamNetworkingIPAddr addrServer;
    addrServer.Clear();
    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg)) {
        std::cerr << "GameNetworkingSockets initialization failed " << err_msg << std::endl;
        exit(-1);
    }

    connection_api = SteamNetworkingSockets();
    char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
    addrServer.ToString(szAddr, sizeof(szAddr), true);
    SteamNetworkingConfigValue_t config_options;

    // Interestingly we cannot change a method associated with a class cannot be cast to void*
    // therefore I used an anonymous function to serve as an adapter.
    auto connections_changed = [this](SteamNetConnectionStatusChangedCallback_t* info) -> void {
        On_Connection_Status_Changed(info);
    };
    config_options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, &connections_changed);

    if (server) {
        listen_socket = connection_api->CreateListenSocketIP(addrServer, 1, &config_options);
        if (listen_socket == k_HSteamListenSocket_Invalid) cerr << "Failed to setup listener on port " << 0 << endl;
        poll_group = connection_api->CreatePollGroup();
        if (poll_group == k_HSteamNetPollGroup_Invalid) cerr << "Failed to setup listener on port " << 0 << endl;
    } else {
        remote_host_connection = connection_api->ConnectByIPAddress(addrServer, 1, &config_options);
        if (remote_host_connection == k_HSteamNetConnection_Invalid) cerr << "Failed to connect to the host" << endl;
    }
}

void On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* pInfo) {}

Network::~Network() {}
