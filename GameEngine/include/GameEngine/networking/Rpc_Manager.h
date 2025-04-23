#pragma once
#include <memory>
#include <rpc/dispatcher.h>

class RPC_Manager {
private:
    std::shared_ptr<rpc::detail::dispatcher> dispatcher;

public:
    RPC_Manager();
    template <typename... Args>
    void call_rpc(std::string const &function_name, Args... args);
    void RPC_Manager::call_rpc(char* data, size_t length) const;
    template <typename Function>
    void bind_rpc(std::string const &function_name, Function function);
};
