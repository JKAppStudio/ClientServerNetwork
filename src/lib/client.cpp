#ifndef __CLIENT_H__
#define __CLIENT_H__

/**
 * @file client.cpp
 * @author Evgeny Krivoshein
 * @brief Client implementation
 */

#include <client.hpp>
#include <iostream>

namespace cs_net{

// Constructor
Client::Client(std::shared_ptr<message_handler> service) 
    : _socket(_context)
    , _handler(std::make_shared<connection_handler>(_context, service))
{ }

// Distructor
Client::~Client(void) 
{
    _handler.reset();
    _thread.join();
}

// Connect
void Client::connect(std::string host, std::string port) 
{
    // Resolve host and port
    boost::asio::ip::tcp::resolver resolver(_context);
    auto endpoints = resolver.resolve(host, port);
    // Post Async connect to context
    boost::asio::async_connect(_handler->socket(), endpoints,
        [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
        {   
            if(!ec) _handler->start();
        });
    //Start ASIO context 
    _thread = std::thread([=](){_context.run();});
}

void Client::disconnect(void) 
{
    boost::asio::post(_context, [this](){_handler->disconnect();});
}


} // namespace cs_net
#endif // __CLIENT_H__