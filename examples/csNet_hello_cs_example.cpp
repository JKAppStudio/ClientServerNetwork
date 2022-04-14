#include <iostream>
// #include <boost/asio.hpp>
#include <message_handler.hpp>
#include <server.hpp>
#include <client.hpp>


using namespace cs_net;

class message_printer : public message_handler
{
public:
    void on_message_receive(const owned_message& owned_msg)
    {
        std::cout << owned_msg.message << std::endl;
    }

    void on_connect(std::weak_ptr<connection_handler> weak_handler)
    {
        if (std::shared_ptr<connection_handler> handler = weak_handler.lock()){
            handler->send("Hello from SERVER!");
        }
    }
};

class client_printer : public message_printer
{
    void on_connect(std::weak_ptr<connection_handler> weak_handler)
    {
        if (std::shared_ptr<connection_handler> handler = weak_handler.lock()){
            handler->send("Hello from CLIENT!");
        }
    }
};

int main(int argc, char* argv[]){


    Server server(std::make_shared<message_printer>());
    server.start_server(8888);

    Client client(std::make_shared<client_printer>());
    client.connect("localhost", "8888");
    
    sleep(1);
    client.disconnect();
    server.stop_server();
    
    return 0;
}