#pragma once
#include <map>
#include <steam/isteamnetworkingsockets.h>

class Network {
public:
    enum Network_State {
        Setting_Up,
        Running,
        Connecting,
        Connected,
        Closing,
        Closed,
    };

private:
    struct Network_Client {
        int id;
    };

    bool server;
    Network_State state;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    HSteamNetConnection remote_host_connection;
    ISteamNetworkingSockets* connection_api;
    std::map<HSteamNetConnection, Network_Client> connection_to_clients;

    void Poll_Incoming_Messages();
    static void On_Connect_Changed_Adapter(SteamNetConnectionStatusChangedCallback_t* new_status);
    static Network* network_instance;

public:
    void On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* new_status);
    Network(bool server);
    ~Network();
    void Network_Update();
    Network_State Get_Network_State();
    int Get_Num_Connected_Clients() const;
};

void Debug_Output(ESteamNetworkingSocketsDebugOutputType error_type, const char* pszMsg);
