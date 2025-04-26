#include "networking/Network.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

#include "networking/Rpc_Manager.h"

using namespace std;
const uint16 DEFAULT_SERVER_PORT = 27020;

Network::Network(bool server, std::function<void()> close_network_function) : close_network_function(
    close_network_function), server(server) {
    network_instance = this;
    state = Setting_Up;

    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg)) {
        std::cerr << "GameNetworkingSockets initialization failed " << err_msg << std::endl;
        exit(-1);
    }
    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Debug, Debug_Output);

    connection_api = SteamNetworkingSockets();
    SteamNetworkingIPAddr addr_server;
    addr_server.Clear();
    addr_server.SetIPv6LocalHost(DEFAULT_SERVER_PORT);
    SteamNetworkingConfigValue_t config_options;

    config_options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)On_Connect_Changed_Adapter);

    rpc_manager = make_unique<RPC_Manager>();
    if (server) {
        listen_socket = connection_api->CreateListenSocketIP(addr_server, 1, &config_options);
        if (listen_socket == k_HSteamListenSocket_Invalid) cerr << "Failed to setup socket listener on port " <<
            addr_server.m_port << endl;
        poll_group = connection_api->CreatePollGroup();
        if (poll_group == k_HSteamNetPollGroup_Invalid) cerr << "Failed to setup poll group listener on port " <<
            addr_server.m_port << endl;
        cout << "Starting server, listening on port: " << addr_server.m_port << endl;
        state = Server_Running;
    } else {
        cout << "Starting client" << endl;
        remote_host_connection = connection_api->ConnectByIPAddress(addr_server, 1, &config_options);
        if (remote_host_connection == k_HSteamNetConnection_Invalid)
            cerr << "Failed to create connection to the host" << endl;
        state = Client_Connecting;
    }
}

Network::~Network() {
    state = Closed;
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
}

void Network::Network_Update() {
    Poll_Incoming_Messages();
    connection_api->RunCallbacks();
    if (state == Closing) {
        close_network_function();
    }
}

/**
 * Handles receiving messages sent from the clients.
 */
void Network::Poll_Incoming_Messages() {
    if (state == Closing || state == Closed) return;
    while (true) {
        ISteamNetworkingMessage* incoming_message = nullptr;
        int num_messages = -2;

        if (server) num_messages = connection_api->ReceiveMessagesOnPollGroup(poll_group, &incoming_message, 1);
        else num_messages = connection_api->ReceiveMessagesOnConnection(remote_host_connection, &incoming_message, 1);

        if (num_messages == 0) break;
        if (num_messages < 0)
            cerr << "Error checking messages on server: " << server << ". Number of messages is: " << num_messages <<
                endl;
        assert(num_messages == 1 && incoming_message);
        // It's easier to parse the string as a c-string instead of a c++ string at first
        std::string cstring_message;
        cstring_message.assign((const char*)incoming_message->m_pData, incoming_message->m_cbSize);
        const char* message = cstring_message.c_str();

        if (server) {
            auto client = connection_to_clients.find(incoming_message->m_conn);
            assert(client != connection_to_clients.end());

            cout << "Message from " << client->second.id << ": " << message << endl;
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
                string debug_message;
                if (new_status->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer) {
                    debug_message = "Connection closed by client request with id: " + to_string(client->second.id);
                } else {
                    debug_message = "Internal problem on the server with id: " + client->second.id;
                }
                cout << debug_message << endl;
                connection_api->CloseConnection(new_status->m_hConn, new_status->m_info.m_eState, debug_message.c_str(),
                    false);
                connection_to_clients.erase(client);
            } else {
                if (new_status->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer)
                    cout << "Leaving server due to server request" << endl;
                else cout << "Leaving server" << endl;
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
            state = Client_Connected;
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

bool Network::Is_Server() const {
    return server;
}

std::string Network::Get_Network_State_Str() const {
    switch (state) {
        case Setting_Up:
            if (server) return "Starting Server";
            return "Starting Client";
        case Server_Running:
            return "Server Running";
        case Client_Connecting:
            return "Connecting to server";
        case Client_Connected:
            return "Connected to server";
        case Closing:
            if (server) return "Shutting down server";
            return "Closing client";
        case Closed:
            return "Closed";
    }
    return "Error";
}

void Network::invoke_rpc(char* data, size_t length) {
    if (server) {
        auto result = rpc_manager->call_data_rpc(data, length);
        if (result == RPC_Manager::INVALID) {
            cout << "Dropping invalid rpc call!";
            return;
        }
        // Send rpc call to all clients

    } else {
        rpc_manager->call_data_rpc(data, length);
    }
}


template <typename... Args>
void Network::call_rpc(std::string const& function_name, Args... args) {
    // auto serialized_rpc = rpc_manager->pack_rpc(function_name, args);
    auto call_obj = make_tuple(static_cast<uint8_t>(0), 1, function_name, make_tuple(args...));

    auto buffer = new clmdep_msgpack::v1::sbuffer;
    clmdep_msgpack::v1::pack(*buffer, call_obj);

    if (server) {
        invoke_rpc(buffer->data(), buffer->size());
    } else {
        // Send the rpc call to the server
    }
    delete buffer;
}

template <typename Function>
void Network::bind_rpc(std::string const& function_name, Function function) {
    rpc_manager->bind_rpc<Function>(function_name, function);
}


void Debug_Output(ESteamNetworkingSocketsDebugOutputType error_type, const char* pszMsg) {
    if (error_type == k_ESteamNetworkingSocketsDebugOutputType_Bug) {
        cerr << pszMsg << endl;
    } else {
        cout << pszMsg << endl;
    }
}

Network* Network::network_instance = nullptr;
