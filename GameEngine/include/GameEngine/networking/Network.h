#pragma once
#include <functional>
#include <map>
#include <string>
#include <steam/isteamnetworkingsockets.h>

class Client;

class Network {
public:
    enum Network_State {
        Setting_Up,
        Server_Running,
        Client_Connecting,
        Client_Connected,
        Closing,
        Closed,
    };

private:
    std::function<void()> close_network_function;
    bool server;
    Network_State state;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    HSteamNetConnection remote_host_connection;
    ISteamNetworkingSockets* connection_api;
    std::map<HSteamNetConnection, Client*> connection_to_clients;

    void Poll_Incoming_Messages();
    void Process_Message(std::string& message, Client& client);
    static void On_Connect_Changed_Adapter(SteamNetConnectionStatusChangedCallback_t* new_status);
    static Network* network_instance;

public:
    void On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* new_status);
    Network(bool server, std::function<void()> close_network_function);
    virtual ~Network();
    void Network_Update();
    Network_State Get_Network_State();
    int Get_Num_Connected_Clients() const;
    bool Is_Server() const;
    std::string Get_Network_State_Str() const;
    void Send_Message_To_Server(std::string message);
    void Send_Message_To_Client(HSteamNetConnection client, std::string message);
    void Send_Message_To_All_Clients(std::string message);
};

void Debug_Output(ESteamNetworkingSocketsDebugOutputType error_type, const char* pszMsg);
