#pragma once
#include <map>
#include <steam/isteamnetworkingsockets.h>

class Network {
private:
    struct Network_Client {
        int id;
    };

    bool server;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    HSteamNetConnection remote_host_connection;
    ISteamNetworkingSockets* connection_api;
    std::map<HSteamNetConnection, Network_Client> connection_to_clients;

    void Poll_Incoming_Messages();
    void Poll_Connection_Status_Changed();
    void On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* pInfo);

public:
    Network(bool server);
    ~Network();
    void Network_Update();
};
