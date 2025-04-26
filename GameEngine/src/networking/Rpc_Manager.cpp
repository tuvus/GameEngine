#include "networking/Rpc_Manager.h"

#include <iostream>
#include "rpc/client.h"

using namespace std;

RPC_Manager::RPC_Manager() : dispatcher(std::make_shared<rpc::detail::dispatcher>()) {
}

clmdep_msgpack::v1::object unpack(char* data, size_t length) {
    clmdep_msgpack::v1::object_handle result;
    // Pass the msgpack::object_handle
    unpack(result, data, length);
    // Get msgpack::object from msgpack::object_handle (shallow copy)
    return result.get();
}

template <typename... Args>
void RPC_Manager::call_rpc(std::string const& function_name, Args... args) const {
    auto call_obj = make_tuple(static_cast<uint8_t>(0), 1, function_name, make_tuple(args...));

    auto buffer = new clmdep_msgpack::v1::sbuffer;
    clmdep_msgpack::v1::pack(*buffer, call_obj);
    dispatcher->dispatch(unpack(buffer->data(), buffer->size()));
    delete buffer;
}

RPC_Manager::Rpc_Validator_Result RPC_Manager::call_data_rpc(char* data, size_t length) const {
    auto response = dispatcher->dispatch(unpack(data, length));
    auto handle = clmdep_msgpack::v1::object_handle();
    response.capture_result(handle);
    return handle.as<Rpc_Validator_Result>();
}

template <typename Function>
void RPC_Manager::bind_rpc(string const& function_name, Function function) {
    dispatcher->bind(function_name, function);
}

template <typename... Args>
tuple<char*, size_t>* RPC_Manager::pack_rpc(std::string const& function_name, Args... args) {
    auto call_obj = make_tuple(static_cast<uint8_t>(0), 1, function_name, make_tuple(args...));

    auto buffer = new clmdep_msgpack::v1::sbuffer;
    clmdep_msgpack::v1::pack(*buffer, call_obj);
    auto packed = new tuple(buffer->data(), buffer->size());
    delete buffer;

    return packed;
}
