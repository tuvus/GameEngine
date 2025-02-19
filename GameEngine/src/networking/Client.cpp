#include "networking/Client.h"

#include <zpp_bits.h>
using namespace std;
using namespace std::literals;
using namespace zpp::bits::literals;


Client::Client(Network* network, const long id, bool local_client) : network(network), id(id), name(to_string(id)),
    local_client(local_client) {}

void Client::Recieve_Message(std::string& message) {
    if (message.starts_with("set:")) name = message.substr(4);
}


void Client::Set_Name(const std::string& name, bool rpc) {
    if (Is_Local_Client()) {
        if (rpc) {
            // This must have come from the server
            this->name = name;
        } else {
            network->Send_Rpc_Call_To_Server<"SetName"_sha256_int>();
        }
    } else {
        network->Send_Message_To_All_Clients("set:" + name);
    }
}

void Client::Set_Name_RPC(const std::string& name) const {
    network->Send_Message_To_Server("set:" + name);
}


long Client::Get_Id() const {
    return id;
}

std::string Client::Get_Name() {
    return name;
}

bool Client::Is_Local_Client() const {
    return local_client;
}
