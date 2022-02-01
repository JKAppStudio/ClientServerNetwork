/**
 * @file client.hpp
 * @author Evgeny Krivoshein
 * @brief Client interface
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

    /**
     * @brief Establish connection with remote host.
     * 
     * @param host remote host ip address
     * @param port port to conect to.
     */
    void connect(std::string host, std::string port);

    /**
     * @brief Disconnect from remote host by closing socket.
     * 
     */
    void disconnect(void);

    /**
     * @brief Send message
     * 
     * @param msg message to send
     */
    void send(const message_type& msg);

private:
    /// ASIO context
    boost::asio::io_context _context;
    /// Soket
    boost::asio::ip::tcp::socket _socket;
    /// Connection handler 
    shared_handler_t _handler;
    /// Thread in wich _context will be running
    std::thread _thread;
};

} // namespace cs_net

#endif // __CS_NET_CLIENT_HPP__