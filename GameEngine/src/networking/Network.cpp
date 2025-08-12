#include <cassert>
#include <functional>
#include <iostream>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>

#include "networking/Network.h"
#include "networking/Rpc_Manager.h"

using namespace std;
const uint16 DEFAULT_SERVER_PORT = 27020;

Network::Network(bool server, std::function<void()> close_network_function)
    : close_network_function(close_network_function), server(server) {
    network_instance = this;
    state = Setting_Up;
    connection_events = make_unique<std::unordered_set<Network_Events_Receiver*>>();
    rpc_manager = make_unique<RPC_Manager>();
}

Network::~Network() {
    state = Closed;
    if (server) {
        for (auto client : connection_to_clients) {
            connection_api->CloseConnection(client.first, 0, "Shutting down server", false);
        }
        for (const Network_Events_Receiver* receiver : *connection_events) {
            const_cast<Network_Events_Receiver*>(receiver)->On_Server_Stop();
        }
        connection_to_clients.clear();
        connection_api->CloseListenSocket(listen_socket);
        listen_socket = k_HSteamListenSocket_Invalid;
        connection_api->DestroyPollGroup(poll_group);
        poll_group = k_HSteamNetPollGroup_Invalid;
    }
    GameNetworkingSockets_Kill();
}

void Network::Start_Network() {
    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg)) {
        std::cerr << "GameNetworkingSockets initialization failed " << err_msg << std::endl;
        exit(-1);
    }
    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Debug,
                                                   Debug_Output);

    connection_api = SteamNetworkingSockets();
    SteamNetworkingIPAddr addr_server;
    addr_server.Clear();
    addr_server.SetIPv6LocalHost(DEFAULT_SERVER_PORT);
    SteamNetworkingConfigValue_t config_options;

    config_options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                          (void*) On_Connect_Changed_Adapter);

    if (server) {
        remote_host_connection = k_HSteamNetConnection_Invalid;
        listen_socket = connection_api->CreateListenSocketIP(addr_server, 1, &config_options);
        if (listen_socket == k_HSteamListenSocket_Invalid)
            cerr << "Failed to setup socket listener on port " << addr_server.m_port << endl;
        poll_group = connection_api->CreatePollGroup();
        if (poll_group == k_HSteamNetPollGroup_Invalid)
            cerr << "Failed to setup poll group listener on port " << addr_server.m_port << endl;
        cout << "Starting server, listening on port: " << addr_server.m_port << endl;
        state = Server_Running;
        for (const Network_Events_Receiver* receiver : *connection_events) {
            const_cast<Network_Events_Receiver*>(receiver)->On_Server_Start();
        }
    } else {
        cout << "Starting client" << endl;
        remote_host_connection =
            connection_api->ConnectByIPAddress(addr_server, 1, &config_options);
        if (remote_host_connection == k_HSteamNetConnection_Invalid)
            cerr << "Failed to create connection to the host" << endl;
        state = Client_Connecting;
    }
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
    if (state == Closing || state == Closed)
        return;
    while (true) {
        ISteamNetworkingMessage* incoming_message = nullptr;
        int num_messages = -2;

        if (server)
            num_messages =
                connection_api->ReceiveMessagesOnPollGroup(poll_group, &incoming_message, 1);
        else
            num_messages = connection_api->ReceiveMessagesOnConnection(remote_host_connection,
                                                                       &incoming_message, 1);

        if (num_messages == 0)
            break;
        if (num_messages < 0)
            cerr << "Error checking messages on server: " << server
                 << ". Number of messages is: " << num_messages << endl;
        assert(num_messages == 1 && incoming_message);
        // // It's easier to parse the string as a c-string instead of a c++ string at first
        // std::string cstring_message;
        // cstring_message.assign((const char*)incoming_message->m_pData,
        // incoming_message->m_cbSize); const char* message = cstring_message.c_str();

        if (server) {
            auto client = connection_to_clients.find(incoming_message->m_conn);
            assert(client != connection_to_clients.end());
        } else {
            // cout << cstring_message << endl;
        }
        Receive_Message(static_cast<char*>(incoming_message->m_pData), incoming_message->m_cbSize);

        incoming_message->Release();
    }
}

/**
 * This static method is needed to convert On_Connection_Status_Changed to a
 * (void*) anonymous function instead of a method. We go around this by using a
 * static instance to point to the correct method that we wanted to call.
 */
void Network::On_Connect_Changed_Adapter(SteamNetConnectionStatusChangedCallback_t* new_status) {
    network_instance->On_Connection_Status_Changed(new_status);
}

void Network::On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* new_status) {
    assert(new_status->m_hConn == remote_host_connection ||
           remote_host_connection == k_HSteamNetConnection_Invalid);
    switch (new_status->m_info.m_eState) {
        case k_ESteamNetworkingConnectionState_None:
            break;
        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally: {
            if (server) {
                // Something happened to our client
                // Close the connection to them specifically
                if (new_status->m_eOldState != k_ESteamNetworkingConnectionState_Connected) {
                    auto client = connection_to_clients.find(new_status->m_hConn);
                    assert(client != connection_to_clients.end());
                    assert(new_status->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
                    connection_api->CloseConnection(new_status->m_hConn,
                                                    new_status->m_info.m_eState, nullptr, false);
                    for (const Network_Events_Receiver* receiver : *connection_events) {
                        const_cast<Network_Events_Receiver*>(receiver)->On_Client_Disconnected(
                            client->first);
                    }
                    connection_to_clients.erase(client);
                    break;
                }

                auto client = connection_to_clients.find(new_status->m_hConn);
                assert(client != connection_to_clients.end());
                string debug_message;
                if (new_status->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer) {
                    debug_message = "Connection closed by client request with id: " +
                                    to_string(client->second.id);
                } else {
                    debug_message =
                        "Internal problem on the server with id: " + to_string(client->second.id);
                }
                cout << debug_message << endl;
                connection_api->CloseConnection(new_status->m_hConn, new_status->m_info.m_eState,
                                                debug_message.c_str(), false);
                for (const Network_Events_Receiver* receiver : *connection_events) {
                    const_cast<Network_Events_Receiver*>(receiver)->On_Client_Disconnected(
                        client->first);
                }
                connection_to_clients.erase(client);
            } else {
                if (new_status->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer)
                    cout << "Leaving server due to server request" << endl;
                else
                    cout << "Leaving server" << endl;
                connection_api->CloseConnection(new_status->m_hConn, new_status->m_info.m_eState,
                                                nullptr, false);
                vector<Network_Events_Receiver*> receivers;
                for (const Network_Events_Receiver* receiver : *connection_events) {
                    receivers.emplace_back(const_cast<Network_Events_Receiver*>(receiver));
                }
                for (Network_Events_Receiver* receiver : receivers) {
                    receiver->On_Disconnected();
                }
                state = Closing;
            }
        }
        case k_ESteamNetworkingConnectionState_Connecting: {
            if (!server)
                break;
            auto client = connection_to_clients.find(new_status->m_hConn);
            if (client != connection_to_clients.end()) {
                cerr << "Trying to connect a client that has already been connected!" << endl;
            }
            cout << "Connecting client " << new_status->m_info.m_szConnectionDescription << endl;
            if (connection_api->AcceptConnection(new_status->m_hConn) != k_EResultOK) {
                connection_api->CloseConnection(new_status->m_hConn,
                                                k_ESteamNetworkingConnectionState_ClosedByPeer,
                                                "Likely disconnected before connected", false);
                cout << "Couldn't connect client, they might have disconnected already" << endl;
                break;
            }
            if (!connection_api->SetConnectionPollGroup(new_status->m_hConn, poll_group)) {
                connection_api->CloseConnection(new_status->m_hConn,
                                                k_ESteamNetworkingConnectionState_ClosedByPeer,
                                                "Couldn't add client to a poll group", false);
            }

            int new_client_id = rand();
            cout << "Connecting client with it: " << new_client_id << endl;
            connection_to_clients[new_status->m_hConn] = Network_Client{new_client_id};
            connection_api->SetConnectionName(new_status->m_hConn,
                                              to_string(new_client_id).c_str());
            for (const Network_Events_Receiver* receiver : *connection_events) {
                const_cast<Network_Events_Receiver*>(receiver)->On_Client_Connected(new_client_id);
            }
        }
        case k_ESteamNetworkingConnectionState_Connected: {
            if (server)
                break;
            cout << "Client connected" << endl;
            state = Client_Connected;
            for (const Network_Events_Receiver* receiver : *connection_events) {
                const_cast<Network_Events_Receiver*>(receiver)->On_Connected();
            }
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
            if (server)
                return "Starting Server";
            return "Starting Client";
        case Server_Running:
            return "Server Running";
        case Client_Connecting:
            return "Connecting to server";
        case Client_Connected:
            return "Connected to server";
        case Closing:
            if (server)
                return "Shutting down server";
            return "Closing client";
        case Closed:
            return "Closed";
    }
    return "Error";
}

void Network::Send_Message_To_Client(const HSteamNetConnection connection,
                                     const Rpc_Message& rpc_message) {
    clmdep_msgpack::sbuffer message_data;
    clmdep_msgpack::packer packer(message_data);
    packer.pack(rpc_message);
    connection_api->SendMessageToConnection(connection, message_data.data(),
                                            static_cast<uint32>(message_data.size()),
                                            k_nSteamNetworkingSend_Reliable, nullptr);
}

void Network::Send_Message_To_Clients(const Rpc_Message& rpc_message) {
    for (auto client_connection : connection_to_clients) {
        Send_Message_To_Client(client_connection.first, rpc_message);
    }
}

void Network::Send_Message_To_Server(const Rpc_Message& rpc_message) {
    // Psych! We can actually reuse Send_Message_To_Client but pass the server connection instead!
    Send_Message_To_Client(remote_host_connection, rpc_message);
}

void Network::Receive_Message(char* data, size_t size) {
    clmdep_msgpack::object_handle result;
    clmdep_msgpack::unpack(result, data, size);
    auto rpc_message = result.get().as<Rpc_Message>();
    invoke_rpc(rpc_message.rpc_call.data(), rpc_message.rpc_call.size());
}

void Network::invoke_rpc(char* data, size_t size) {
    if (server) {
        auto result = rpc_manager->call_data_rpc(data, size);
        if (result == RPC_Manager::INVALID) {
            cerr << "Dropping invalid rpc call!" << endl;
            return;
        }
        auto rpc_call_data = Rpc_Message(data, size);
        Send_Message_To_Clients(rpc_call_data);
    } else {
        // The rpc call must be valid by this point
        if (rpc_manager->call_data_rpc(data, size) != RPC_Manager::VALID) {
            cerr << "Client received an invalid rpc call! This is probably due to a desync!"
                 << endl;
        }
    }
}

void Debug_Output(ESteamNetworkingSocketsDebugOutputType error_type, const char* pszMsg) {
    if (error_type == k_ESteamNetworkingSocketsDebugOutputType_Bug) {
        cerr << pszMsg << endl;
        cerr << pszMsg << endl;
    } else {
        cout << pszMsg << endl;
    }
}

Network* Network::network_instance = nullptr;
