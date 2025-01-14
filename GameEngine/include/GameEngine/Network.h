#pragma once
#include <steam/isteamnetworkingsockets.h>

class Network {
private:
    bool server;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    HSteamNetConnection remote_host_connection;
    ISteamNetworkingSockets *connection_api;

    void On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* pInfo);
public:
    Network(bool server);
    ~Network();
};