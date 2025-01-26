#include "networking/Client.h"

using namespace std;
Client::Client(const long id) : network(nullptr), id(id), name(to_string(id)) {}

Client::Client(Network* network, const long id) : network(network), id(id), name(to_string(id)) {}

void Client::Recieve_Message(std::string& message) {
    if (message.starts_with("set:"))
        name = message.substr(4);
}


void Client::Set_Name(const std::string& name) {
    this->name = name;
}

void Client::Set_Name_RPC(const std::string& name) {
    network->Send_Message_To_Server("set:" + name);
}


long Client::Get_Id() const {
    return id;
}

std::string Client::Get_Name() {
    return name;
}

bool Client::Is_Local_Client() const {
    return network != nullptr;
}

