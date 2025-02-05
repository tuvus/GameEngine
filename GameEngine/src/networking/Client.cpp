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


void Client::Set_Name(const std::string& name) {
    this->name = name;
    if (!Is_Local_Client()) {
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
