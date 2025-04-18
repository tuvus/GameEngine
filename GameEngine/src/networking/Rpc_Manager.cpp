#include "networking/Rpc_Manager.h"

#include <iostream>
#include "asio/"

using namespace std;

RPC_Manager::RPC_Manager() : dispatcher(std::make_shared<rpc::detail::dispatcher>()) {
    cout << "Testing the dispatcher" << endl;
    dispatcher->bind("testfunc", []() {
        cout << "The function worked!" << endl;
    });
    call_rpc("testfunc");
}

template <typename... Args>
void RPC_Manager::call_rpc(std::string const& function_name, Args... args) {
    auto args_obj = make_tuple(args...);
    auto call_obj = make_tuple(function_name, args_obj);

    auto buffer = new RPCLIB_MSGPACK::sbuffer;
    RPCLIB_MSGPACK::pack(*buffer, call_obj);

    RPCLIB_MSGPACK::unpacker pac = RPCLIB_MSGPACK::unpacker();
    pac.reserve_buffer(buffer->size());
    RPCLIB_ASIO::buffer(pac.buffer(), buffer->size()),

    // memcpy(pac.buffer(), buffer->data(), buffer->size());
    delete buffer;
    RPCLIB_MSGPACK::unpacked result;
    while(pac.next(result)) {
        auto message = result.get();
        dispatcher->dispatch(message);

    }
}
