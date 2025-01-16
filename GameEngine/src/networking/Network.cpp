#include "networking/Network.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

using namespace std;
const uint16 DEFAULT_SERVER_PORT = 27020;

Network::Network(bool server) : server(server) {
    network_instance = this;
    state = Setting_Up;

    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg)) {
        std::cerr << "GameNetworkingSockets initialization failed " << err_msg << std::endl;
        exit(-1);
    }
    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Debug, Debug_Output);

    connection_api = SteamNetworkingSockets();
    SteamNetworkingIPAddr addrServer;
    addrServer.Clear();
    addrServer.SetIPv6LocalHost(DEFAULT_SERVER_PORT);
    SteamNetworkingConfigValue_t config_options;

    config_options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)On_Connect_Changed_Adapter);

    if (server) {
        listen_socket = connection_api->CreateListenSocketIP(addrServer, 1, &config_options);
        if (listen_socket == k_HSteamListenSocket_Invalid) cerr << "Failed to setup socket listener on port " <<
            addrServer.m_port << endl;
        poll_group = connection_api->CreatePollGroup();
        if (poll_group == k_HSteamNetPollGroup_Invalid) cerr << "Failed to setup poll group listener on port " <<
            addrServer.m_port << endl;
        cout << "Starting server, listening on port: " << addrServer.m_port << endl;
        state = Running;
    } else {
        cout << "Starting client" << endl;
        remote_host_connection = connection_api->ConnectByIPAddress(addrServer, 1, &config_options);
        if (remote_host_connection == k_HSteamNetConnection_Invalid)
            cerr << "Failed to create connection to the host" << endl;
        state = Connecting;
    }
}

Network::~Network() {
    if (server) {
        for (auto client : connection_to_clients) {
            connection_api->CloseConnection(client.first, 0, "Shutting down server", false);
        }
        connection_to_clients.clear();
        connection_api->CloseListenSocket(listen_socket);
        listen_socket = k_HSteamListenSocket_Invalid;
        connection_api->DestroyPollGroup(poll_group);
        poll_group = k_HSteamNetPollGroup_Invalid;
    }
    GameNetworkingSockets_Kill();

    state = Closed;
}

void Network::Network_Update() {
    Poll_Incoming_Messages();
    connection_api->RunCallbacks();
}

/**
 * Handles receiving messages sent from the clients.
 */
void Network::Poll_Incoming_Messages() {
    while (true) {
        ISteamNetworkingMessage* incoming_message = nullptr;
        int numMessages = -2;

        if (server) numMessages = connection_api->ReceiveMessagesOnPollGroup(poll_group, &incoming_message, 1);
        else numMessages = connection_api->ReceiveMessagesOnConnection(remote_host_connection, &incoming_message, 1);

        if (numMessages == 0) break;
        if (numMessages < 0)
            cerr << "Error checking messages on server: " << server << ". Number of messages is: " << numMessages <<
                endl;
        assert(numMessages == 1 && incoming_message);
        // It's easier to parse the string as a c-string instead of a c++ string at first
        std::string cstring_message;
        cstring_message.assign((const char*)incoming_message->m_pData, incoming_message->m_cbSize);
        const char* message = cstring_message.c_str();

        if (server) {
            auto itClient = connection_to_clients.find(incoming_message->m_conn);
            assert(itClient != connection_to_clients.end());

            cout << "Message from " << itClient->second.id << ": " << message << endl;
        } else {
            cout << cstring_message << endl;
        }
        incoming_message->Release();
    }
}

/**
 * This static method is needed to convert On_Connection_Status_Changed to a (void*) anonymous function instead of a method.
 * We go around this by using a static instance to point to the correct method that we wanted to call.
 */
void Network::On_Connect_Changed_Adapter(SteamNetConnectionStatusChangedCallback_t* new_status) {
    network_instance->On_Connection_Status_Changed(new_status);
}

void Network::On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* new_status) {
    assert(new_status->m_hConn == remote_host_connection || remote_host_connection == k_HSteamNetConnection_Invalid);
    switch (new_status->m_info.m_eState) {
        case k_ESteamNetworkingConnectionState_None:
            break;
        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally: {
            if (server) {
                // Something happened to our client
                // Close the connection to them specifically
                if (new_status->m_eOldState != k_ESteamNetworkingConnectionState_Connected) {
                    assert(new_status->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
                    connection_api->CloseConnection(new_status->m_hConn, new_status->m_info.m_eState, nullptr, false);
                    break;
                }
                auto client = connection_to_clients.find(new_status->m_hConn);
                assert(client != connection_to_clients.end());
                const char* debug_message;
                if (new_status->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer) {
                    debug_message = "Connection closed by client request with id: " + client->second.id;
                } else {
                    debug_message = "Internal problem on the server with id: " + client->second.id;
                }
                cout << debug_message << endl;
                connection_api->CloseConnection(new_status->m_hConn, new_status->m_info.m_eState, debug_message, false);
            } else {
                connection_api->CloseConnection(new_status->m_hConn, new_status->m_info.m_eState, nullptr, false);
                state = Closing;
            }
        }
        case k_ESteamNetworkingConnectionState_Connecting: {
            if (!server) break;
            auto client = connection_to_clients.find(new_status->m_hConn);
            if (client != connection_to_clients.end()) {
                cerr << "Trying to conect a client that has already been connected!" << endl;
            }
            cout << "Connecting client " << new_status->m_info.m_szConnectionDescription << endl;
            if (connection_api->AcceptConnection(new_status->m_hConn) != k_EResultOK) {
                connection_api->CloseConnection(new_status->m_hConn, k_ESteamNetworkingConnectionState_ClosedByPeer,
                    "Likely disconnected before connected", false);
                cout << "Couldn't connect client, they might have disconnected already" << endl;
                break;
            }
            if (!connection_api->SetConnectionPollGroup(new_status->m_hConn, poll_group)) {
                connection_api->CloseConnection(new_status->m_hConn, k_ESteamNetworkingConnectionState_ClosedByPeer,
                    "Couldn't add client to a poll group", false);
            }

            int new_client_id = rand();
            cout << "Connecting client with it: " << new_client_id << endl;
            connection_to_clients[new_status->m_hConn] = Network_Client{new_client_id};
            connection_api->SetConnectionName(new_status->m_hConn, to_string(new_client_id).c_str());
        }
        case k_ESteamNetworkingConnectionState_Connected: {
            if (server) break;
            cout << "Client connected" << endl;
            state = Connected;
        }
        default:
            break;
    }
}

Network::Network_State Network::Get_Network_State() {
    return state;
}

int Network::Get_Num_Connected_Clients() const {
    return connection_to_clients.size();
}

void Debug_Output(ESteamNetworkingSocketsDebugOutputType error_type, const char* pszMsg) {
    if (error_type == k_ESteamNetworkingSocketsDebugOutputType_Bug) {
        cerr << pszMsg << endl;
    } else {
        cout << pszMsg << endl;
    }
}

Network* Network::network_instance = nullptr;
