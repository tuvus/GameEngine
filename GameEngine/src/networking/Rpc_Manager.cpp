#include "networking/Rpc_Manager.h"

#include <iostream>
#include "rpc/client.h"

using namespace std;

RPC_Manager::RPC_Manager() : dispatcher(std::make_shared<rpc::detail::dispatcher>()) {
    cout << "Testing the dispatcher" << endl;
    dispatcher->bind("testfunc", []() {
        cout << "The function worked!" << endl;
    });
    call_rpc("testfunc");
    cout << "Ending the dispatcher" << endl;
}

clmdep_msgpack::v1::object unpack(char* data, size_t length) {
    clmdep_msgpack::v1::object_handle result;

    // Pass the msgpack::object_handle
    unpack(result, data, length);
    // Get msgpack::object from msgpack::object_handle (shallow copy)
    return result.get();
}

template <typename... Args>
void RPC_Manager::call_rpc(std::string const& function_name, Args... args) {
    auto args_obj = make_tuple(args...);
    auto call_obj = make_tuple(static_cast<uint8_t>(0), 1, function_name, args_obj);

    auto buffer = new clmdep_msgpack::v1::sbuffer;
    clmdep_msgpack::v1::pack(*buffer, call_obj);
    dispatcher->dispatch(unpack(buffer->data(), buffer->size()));
    delete buffer;
}

