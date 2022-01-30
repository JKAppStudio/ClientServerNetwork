#include <iostream>
#include <boost/asio.hpp>
#include <message_handler.hpp>
#include <server.hpp>
#include <client.hpp>


using namespace cs_net;

class message_printer : public message_handler
{
public:
    void dispatch_message(const owned_message& owned_msg){
        std::cout << owned_msg.message;
    }

    void prepare_response(void){

    }
};

int main(int argc, char* argv[]){


    Server server(std::make_shared<message_printer>());
    server.start_server(8888);

    Client client(std::make_shared<message_printer>());
    client.connect("localhost", "8888");
    
    sleep(3);
    client.disconnect();
    server.stop_server();
    
    return 0;
}