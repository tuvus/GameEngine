#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <steam/isteamnetworkingsockets.h>
#include <string>
#include <utility>

#include "rpc_manager.h"

#include <queue>

struct Rpc_Message {
    // Information about msgpack here: https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_packer
    long rpc_id;
    bool order_sensitive;
    std::vector<char> rpc_call;

    Rpc_Message() = default;

    Rpc_Message(bool order_sensitive, char* rpc_data, size_t length)
        : rpc_id(-1), order_sensitive(order_sensitive) {
        rpc_call = std::vector<char>(rpc_data, rpc_data + length);
    }

    explicit Rpc_Message(bool order_sensitive, std::vector<char> rpc_call)
        : rpc_id(-1), order_sensitive(order_sensitive), rpc_call(std::move(rpc_call)) {}

    Rpc_Message(const Rpc_Message& copy, long msg_id)
        : rpc_id(msg_id), order_sensitive(copy.order_sensitive) {
        rpc_call = std::vector<char>();
        // We need to do a deep copy and copy the data as well so that the old one can be deleted
        std::copy(copy.rpc_call.begin(), copy.rpc_call.end(), back_inserter(rpc_call));
    }

    // Tells msgpack how to serialize Rpc_Message
    MSGPACK_DEFINE(rpc_id, order_sensitive, rpc_call);
};

typedef unsigned int uint32;
typedef uint32 Client_ID;

class Network_Events_Receiver {
  public:
    // Called on the client when the client is connected
    virtual void On_Connected() = 0;
    // Called on the client when the client is disconnected (not on the host)
    virtual void On_Disconnected() = 0;
    // Called on the server when the server is started
    virtual void On_Server_Start() = 0;
    // Called on the server when the server is stopped
    virtual void On_Server_Stop() = 0;
    // Called on the server when a client is connected
    virtual void On_Client_Connected(Client_ID) = 0;
    // Called on the server when a client is disconnected
    virtual void On_Client_Disconnected(Client_ID) = 0;
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
    class Rpc_Comparator {
      public:
        bool operator()(Rpc_Message* a, Rpc_Message* b) { return a->rpc_id <= b->rpc_id; }
    };

    class Connection_State {
      public:
        long next_rpc_id = 0;
        long next_rpc_id_to_receive = 0;
        std::priority_queue<Rpc_Message*, std::vector<Rpc_Message*>, Rpc_Comparator> rpc_heap;
    };

    std::function<void()> close_network_function;
    bool server;
    Network_State state;
    HSteamListenSocket listen_socket;
    HSteamNetPollGroup poll_group;
    Client_ID remote_host_connection;
    ISteamNetworkingSockets* connection_api;
    // Connected clients and the next rpc id to send to them
    std::unordered_map<Client_ID, Connection_State*> connected_clients;
    // The client may connect to the server after it receives an rpc call from the server
    // This results in the connected_clients not being initialized correctly
    // In this case store the messages in this list and execute them once connected
    std::vector<std::tuple<Client_ID, Rpc_Message*>> pre_connected_messages;
    std::vector<Client_ID> newly_connected_clients;
    pthread_mutex_t newly_connected_client_mutex;

    void Poll_Incoming_Messages();
    static void On_Connect_Changed_Adapter(SteamNetConnectionStatusChangedCallback_t* new_status);
    static Network* network_instance;
    std::unique_ptr<RPC_Manager> rpc_manager;

    void Receive_Message(Client_ID from, char* data, size_t size);
    void Receive_Message(Client_ID from, Rpc_Message* rpc);

    /**
     * Actually calls the rpc on the server or client.
     * Should only be called internally when actually applying the effects of the function related
     * to the rpc.
     * @param data The name and parameters of the function cal
     * @param size The size of the data
     */
    void invoke_rpc(bool order_sensitive, char* data, size_t size);

  public:
    void On_Connection_Status_Changed(SteamNetConnectionStatusChangedCallback_t* new_status);
    Network(bool server, std::function<void()> close_network_function);
    ~Network();
    void Start_Network();
    void Network_Update();
    Network_State Get_Network_State() const;
    int Get_Num_Connected_Clients() const;
    bool Is_Server() const;
    std::string Get_Network_State_Str() const;
    void Send_Message_To_Client(HSteamNetConnection connection, const Rpc_Message& rpc_message);
    void Send_Message_To_Clients(const Rpc_Message& rpc_message);
    void Send_Message_To_Server(const Rpc_Message& rpc_message);

    // Holds aset of subscribers to the networking events that can occur
    std::unique_ptr<std::unordered_set<Network_Events_Receiver*>> connection_events;

    /**
     * Calls the function on the server.
     * If the function returns a valid result it calls the function on every client.
     * This method can be called on the server or on any client.
     * @param order_sensitive If true the rpc has to be called after all previous rpcs are called
     * @param function_name The name of the function being called
     * @param args The arguments for the function call
     */
    template <typename... Args>
    void call_rpc(bool order_sensitive, std::string const& function_name, Args... args) {
        // TODO: Figure out how to let Rpc_Manager handle packing
        // clmdep_msgpack::v1::sbuffer* buffer = rpc_manager->pack_rpc(function_name,
        // std::forward<Args>(args)...);
        auto call_obj =
            make_tuple(static_cast<uint8_t>(0), 1, function_name, std::make_tuple(args...));

        auto buffer = new clmdep_msgpack::v1::sbuffer;
        clmdep_msgpack::v1::pack(*buffer, call_obj);

        if (server) {
            invoke_rpc(order_sensitive, buffer->data(), buffer->size());
        } else {
            // Send the rpc call to the server
            auto rpc_call_data = Rpc_Message(order_sensitive, buffer->data(), buffer->size());
            Send_Message_To_Server(rpc_call_data);
        }
        delete buffer;
    }

    /**
     * Sends the rpc from the server to be called on the client specified.
     * @param client_id The id of the client to send to
     * @param function_name The name of the function being called
     * @param args The arguments for the function call
     */
    template <typename... Args>
    void call_rpc_on_client(Client_ID client_id, bool order_sensitive,
                            std::string const& function_name, Args... args) {
        if (!Is_Server()) {
            std::cerr << "Trying to call an rpc on another client while not on the server!"
                      << std::endl;
            return;
        }

        // TODO: Figure out how to let Rpc_Manager handle packing
        // clmdep_msgpack::v1::sbuffer* buffer = rpc_manager->pack_rpc(function_name,
        // std::forward<Args>(args)...);
        auto call_obj =
            make_tuple(static_cast<uint8_t>(0), 1, function_name, std::make_tuple(args...));

        auto buffer = new clmdep_msgpack::v1::sbuffer;
        clmdep_msgpack::v1::pack(*buffer, call_obj);

        auto rpc_call_data = Rpc_Message(order_sensitive, buffer->data(), buffer->size());
        Send_Message_To_Client(client_id, rpc_call_data);

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
    void bind_rpc(std::string const& function_name, Function function) {
        // Directly calling the dispatcher for now
        rpc_manager->dispatcher->bind(function_name, function);
    }

    /**
     * Sets up the RPC call only on the server.
     * The function will only be called on the server and will not call on any non-host clients.
     * Does not do anything if not currently on the server.
     * @param function_name The name of the function being bound
     * @param function The logic to run when the function is called
     */
    template <typename Function>
    void bind_server_rpc(std::string const& function_name, Function function) {
        if (Is_Server()) {
            // Directly calling the dispatcher for now
            rpc_manager->dispatcher->bind(function_name, function);
        }
    }
};

void Debug_Output(ESteamNetworkingSocketsDebugOutputType error_type, const char* pszMsg);
