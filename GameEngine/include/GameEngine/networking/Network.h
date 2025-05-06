#pragma once
#include <functional>
#include <map>
#include <string>
#include <steam/isteamnetworkingsockets.h>

#include "Rpc_Manager.h"

struct Rpc_Message {
    std::vector<char> rpc_call;

    Rpc_Message(char* rpc_data, size_t length) {
        rpc_call = std::vector<char>(rpc_data, rpc_data + length);
    }

    // Tells msgpack how to serialize Rpc_Message
    MSGPACK_DEFINE(rpc_call)
};

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
    struct Network_Client {
        int id;
    };

    std::function<void()> close_network_function;
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
    std::unique_ptr<RPC_Manager> rpc_manager;
    /**
     * Actually calls the rpc on the server or client.
     * Should only be called internally when actually applying the effects of the function related to the rpc.
     * @param data The name and parameters of the function cal
     * @param length The size of the data
     */
    void invoke_rpc(char* data, size_t length);

public:
    void On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* new_status);
    Network(bool server, std::function<void()> close_network_function);
    ~Network();
    void Network_Update();
    Network_State Get_Network_State();
    int Get_Num_Connected_Clients() const;
    bool Is_Server() const;
    std::string Get_Network_State_Str() const;
    void Send_Message_To_Client(HSteamNetConnection connection, const Rpc_Message& rpc_message);
    void Send_Message_To_Clients(const Rpc_Message& rpc_message);
    void Send_Message_To_Server(const Rpc_Message& rpc_message);

    /**
     * Calls the function on the server.
     * If the function returns a valid result it calls the function on every client.
     * This method can be called on the server or on any client.
     * @param function_name The name of the function being called
     * @param args The arguments for the function call
     */
    template <typename... Args>
    void call_rpc(std::string const& function_name, Args... args) {
        // TODO: Figure out how to let Rpc_Manager handle packing
        // clmdep_msgpack::v1::sbuffer* buffer = rpc_manager->pack_rpc(function_name, std::forward<Args>(args)...);
        auto call_obj = make_tuple(static_cast<uint8_t>(0), 1, function_name, std::make_tuple(args...));

        auto buffer = new clmdep_msgpack::v1::sbuffer;
        clmdep_msgpack::v1::pack(*buffer, call_obj);

        if (server) {
            invoke_rpc(buffer->data(), buffer->size());
        } else {
            // Send the rpc call to the server
            auto rpc_call_data = Rpc_Message(buffer->data(), buffer->size());
            Send_Message_To_Server(rpc_call_data);
        }
        delete buffer;
    }

    /**
     * Sets up the RPC call on the local machine.
     * Maps the function name to the given function.
     * Note that the RPC call must be bound on both the server and client to work properly.
     * @param function_name The name of the function being bound
     * @param function The logic to run when the function is called
     */
    template <typename Function>
    void bind_rpc(std::string const& function_name, Function function);
};

void Debug_Output(ESteamNetworkingSocketsDebugOutputType error_type, const char* pszMsg);
