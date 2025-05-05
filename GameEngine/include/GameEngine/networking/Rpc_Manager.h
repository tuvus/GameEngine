#pragma once
#include <memory>
#include <rpc/dispatcher.h>

class RPC_Manager {
public:
    enum Rpc_Validator_Result {
        VALID,
        INVALID,
    };

public:
    RPC_Manager();
    template<typename... Elements>
    void call_rpc(std::string const& function_name, std::tuple<Elements...> args) const;
    Rpc_Validator_Result call_data_rpc(char* data, size_t length) const;
    template <typename Function>
    void bind_rpc(std::string const& function_name, Function function);
    // Making this public for now until I figure out how to pass a function to bind_rpc
    std::shared_ptr<rpc::detail::dispatcher> dispatcher;

    /**
     * Converts the given function name and arguments into the serialized representation of the data along with its length.
     */
    template <typename... Args>
    constexpr std::tuple<char*, size_t>* pack_rpc(std::string const& function_name, Args... args);
};
