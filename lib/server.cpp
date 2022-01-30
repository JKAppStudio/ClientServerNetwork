/**
 * @file server.cpp
 * @author Evgeny Krivoshein (you@domain.com)
 * @brief server implementation
 * 
 */

#include <server.hpp>

namespace cs_net{

// Constructor
Server::Server(std::shared_ptr<message_handler> service, int thread_count)
    : _thread_count(thread_count)
    , _acceptor(_context)
    , _message_handler(service)
{}
// Distructor
Server::~Server()
{
    stop_server();
}

// Statr server
void Server::start_server(uint16_t port)
{
    auto handler = std::make_shared<connection_handler>(_context, _message_handler);

    // Setup the acceptor to listen on the tcp port 
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();

    // Start accepting asynchronous
    _acceptor.async_accept(handler->socket()
                        , [=](auto ec)
                            { 
                                handle_new_connection(handler, ec);
                            }
    );

    // starts thread pool to handle asio events
    for(int i = 0; i <_thread_count; i++)
        _thread_pool.emplace_back([=]{_context.run();});

}

void Server::stop_server(void)
{
    // Request context to stop.
    _context.stop();
    // Tidying up dedicated threads
    for(auto& thread : _thread_pool)
        if (thread.joinable()) thread.join();
}

bool Server::accept(shared_handler_t handler)
{
    return true;
}

void Server::handle_new_connection(shared_handler_t handler, boost::system::error_code const& error)
{
    if(error) return;
    // if accepted, start handler and add it to connection pool
    // disconnect otherwise.
    if (accept(handler)){
        handler->start();
        std::cout << "New connection from: " << handler->remote_address() << std::endl;
    } else {
        handler->disconnect();
    }
    // start new async accept
    auto new_handler = std::make_shared<connection_handler>(_context, _message_handler);
    _acceptor.async_accept(new_handler->socket()
                        , [=](auto ec)
                            { 
                                handle_new_connection(new_handler, ec); 
                            }
    );
}

} // namespace cs_net