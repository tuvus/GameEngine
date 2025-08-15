#include "rpc/client.h"
#include <iostream>

#include "networking/rpc_manager.h"

using namespace std;
RPC_Manager::RPC_Manager() : dispatcher(std::make_shared<rpc::detail::dispatcher>()) {

    // bind_rpc("test", [](int asdf){cout << "THE TEST WORKED!" << asdf << endl;});
    // bind_rpc("test", [this](){cout << "THE TEST WORKED!" << endl;});
    // call_rpc("test");
}

clmdep_msgpack::v1::object rpc_unpack(char* data, size_t length) {
    clmdep_msgpack::v1::object_handle result;
    // Pass the msgpack::object_handle
    unpack(result, data, length);
    // Get msgpack::object from msgpack::object_handle (shallow copy)
    return result.get();
}

template <typename... Elements>
void RPC_Manager::call_rpc(std::string const& function_name, tuple<Elements...> args) const {
    auto call_obj = make_tuple(static_cast<uint8_t>(0), 1, function_name, args);

    auto buffer = new clmdep_msgpack::v1::sbuffer;
    clmdep_msgpack::v1::pack(*buffer, call_obj);
    dispatcher->dispatch(rpc_unpack(buffer->data(), buffer->size()));
    delete buffer;
}

RPC_Manager::Rpc_Validator_Result RPC_Manager::call_data_rpc(char* data, size_t length) const {
    auto response = dispatcher->dispatch(rpc_unpack(data, length));
    auto result = response.get_result()->get();
    if (result.type != clmdep_msgpack::type::POSITIVE_INTEGER)
        return INVALID;
    return static_cast<Rpc_Validator_Result>(result.as<int>());
}

template <typename Function>
void RPC_Manager::bind_rpc(string const& function_name, Function function) {
    dispatcher->bind(function_name, function);
}

template <typename... Args>
clmdep_msgpack::v1::sbuffer* RPC_Manager::pack_rpc(std::string const& function_name, Args... args) {
    auto call_obj = make_tuple(static_cast<uint8_t>(0), 1, function_name, make_tuple(args...));

    auto buffer = new clmdep_msgpack::v1::sbuffer;
    clmdep_msgpack::v1::pack(*buffer, call_obj);
    return buffer;
}
