/**
 * @file client.hpp
 * @author Evgeny Krivoshein
 * @brief cs_net client interface
 */

#ifndef __CS_NET_CLIENT_HPP__
#define __CS_NET_CLIENT_HPP__

#include <boost/asio.hpp>
#include <memory>
#include <message_handler.hpp>
#include <connection_handler.hpp>

namespace cs_net{

/**
 * @brief Client side of client-server network.
 * 
 */
class Client
{
public: 
    /**
     * @brief Construct a new Client object
     * 
     * @param service commenication protocol
     */
    Client(std::shared_ptr<message_handler> service);
    
    /**
     * @brief Destroy the Client object
     */
    ~Client(void);

    void connect(std::string host, std::string port);
    void disconnect(void);
    void send(const message_type& msg);

private:
    boost::asio::io_context _context;
    boost::asio::ip::tcp::socket _socket;

    std::shared_ptr<connection_handler> _handler;
    std::thread _thread;
};

} // namespace cs_net

#endif // __CS_NET_CLIENT_HPP__