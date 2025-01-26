#pragma once
#include <string>

#include "Network.h"

class Client {
private:
    /* Network is only not null on the local client */
    Network* network;
    long id;
    std::string name;

public:
    Client(long id);
    Client(Network* network, long id);
    ~Client() = default;
    void Recieve_Message(std::string& message);
    long Get_Id() const;
    void Set_Name(const std::string& name);
    void Set_Name_RPC(const std::string& name);
    std::string Get_Name();
    bool Is_Local_Client() const;
};
