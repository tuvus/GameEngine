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
    template <typename Function>
    void bind_rpc(std::string const &function_name, Function function);
};
