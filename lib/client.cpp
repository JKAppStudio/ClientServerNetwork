#ifndef __CLIENT_H__
#define __CLIENT_H__

/**
 * @file client.cpp
 * @author Evgeny Krivoshein
 * @brief cs_net client implementation
 */

#include <client.hpp>
#include <iostream>

namespace cs_net{

Client::Client(std::shared_ptr<message_handler> service) 
    : _socket(_context)
    , _handler(std::make_shared<connection_handler>(_context, service))
{ }

Client::~Client(void) 
{
    _handler.reset();
    _thread.join();
}

void Client::connect(std::string host, std::string port) 
{
    boost::asio::ip::tcp::resolver resolver(_context);
    auto endpoints = resolver.resolve(host, port);
    boost::asio::async_connect(_handler->socket(), endpoints,
        [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
        {
            if(!ec) _handler->start();
        });
    _thread = std::thread([=](){_context.run();});
}

void Client::disconnect(void) 
{
    boost::asio::post(_context, [this](){_handler->disconnect();});
}


} // namespace cs_net
#endif // __CLIENT_H__