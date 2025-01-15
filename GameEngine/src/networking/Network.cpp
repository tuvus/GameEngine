#include "networking/Network.h"

#include <cassert>
#include <iostream>
#include <steam/steamnetworkingsockets.h>

using namespace std;

const uint16 DEFAULT_SERVER_PORT = 27020;

Network::Network(bool server) : server(server) {
    state = Setting_Up;
    SteamNetworkingIPAddr addrServer;
    addrServer.Clear();
    addrServer.m_port = DEFAULT_SERVER_PORT;
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
        if (listen_socket == k_HSteamListenSocket_Invalid)
            cerr << "Failed to setup socket listener on port " << 0 << endl;
        poll_group = connection_api->CreatePollGroup();
        if (poll_group == k_HSteamNetPollGroup_Invalid)
            cerr << "Failed to setup poll group listener on port " << 0 << endl;
        cout << "Starting server, listening on port: " << addrServer.m_port << endl;
        state = Running;
    } else {
        remote_host_connection = connection_api->ConnectByIPAddress(addrServer, 1, &config_options);
        if (remote_host_connection == k_HSteamNetConnection_Invalid) cerr << "Failed to connect to the host" << endl;
        cout << "Starting client" << endl;
        state = Connecting;
    }
}

Network::~Network() {
    cout << "Closing Server" << endl;
    connection_to_clients.clear();
    connection_api->CloseListenSocket(listen_socket);
    listen_socket = k_HSteamListenSocket_Invalid;
    connection_api->DestroyPollGroup(poll_group);
    poll_group = k_HSteamNetPollGroup_Invalid;
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
        int numMsgs = connection_api->ReceiveMessagesOnPollGroup(poll_group, &incoming_message, 1);
        if (numMsgs == 0) break;
        if (numMsgs < 0) cerr << "Error checking messages. Number of messages is: " << numMsgs << endl;
        assert(numMsgs == 1 && incoming_message);
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

void Network::On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* new_status) {
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
            assert(client != connection_to_clients.end());
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
        }
        case k_ESteamNetworkingConnectionState_Connected: {
            if (server) break;
            state = Connected;
        }
    }
}

Network::Network_State Network::Get_Network_State() {
    return state;
}
