#include <memory>

#include "CardGame.h"
#include "Application.h"
#include "ApplicationFactory.h"

class CardGameFactory : public ApplicationFactory {
public:
    string Get_Name() override {
        return "NewCardGame";
    }

    unique_ptr<ApplicationWindow> Create_Window(Application& application) override {
        return make_unique<Card_Game>(application);
    }

    function<void(std::chrono::milliseconds, Application&)> Create_Update_Function() override {
        return [](std::chrono::milliseconds delta_time, Application& application) {};
    }
};

using namespace std::literals;
using namespace zpp::bits::literals;

int foo(int i, std::string s);
std::string bar(int i, int j);

using rpc = zpp::bits::rpc<
    zpp::bits::bind<foo, "foo"_sha256_int>,
    zpp::bits::bind<bar, "bar"_sha256_int>
>;

auto [data, in, out] = zpp::bits::data_in_out();

// Server and client together:
auto [client, server] = rpc::client_server(in, out);

// Or separately:
int main() {
    rpc::client client{in, out};
    rpc::server server{in, out};

    // Request from the client:
    client.request<"foo"_sha256_int>(1337, "hello"s).or_throw();

    // Serve the request from the server:
    server.serve().or_throw();

    // Read back the response
    client.response<"foo"_sha256_int>().or_throw(); // == foo(1337, "hello"s);
    unique_ptr<ApplicationFactory> factory = make_unique<CardGameFactory>();
    Create_Application(move(factory), true);
}
