
/**
 * @file connection_handler.hpp
 * @brief Connection Handler class.
 * 
 * @author Evgeny Krivoshein
 */

#ifndef __CSNET_CONNECTION_HANDLER_HPP__
#define __CSNET_CONNECTION_HANDLER_HPP__

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <message_handler.hpp>
#include <iostream>
#include <deque>

namespace cs_net{


/**
 * @brief handles network connection.
 * 
 * @details Deals with all network (socket) heavy lifting.
 * 
 */
class connection_handler
    : public std::enable_shared_from_this<connection_handler>
{

public:
    /**
     * @brief Construct a new connection handler object
     * @param context ASIO io_context to work in
     * @param service 
     */
    connection_handler(boost::asio::io_context& context, std::shared_ptr<message_handler> service);

    /**
     * @brief Destroy the connection handler object
     */
    ~connection_handler(){};

    /**
     * @brief Return socket handler operates on
     * @return boost::asio::ip::tcp::socket& 
     */
    boost::asio::ip::tcp::socket& socket(){ return _socket; }

    /**
     * @brief Start handler 
     * 
     */
    void start(void);

    /**
     * @brief Close connection.
     * @details Close dedicated socket.
     * 
     */
    void disconnect(void);

    /**
     * @brief Return remote client IP address.
     * @details Remote address in "IP":"PORT" format.
     * @return std::string remote ip address
     */
    std::string remote_address(void) const;

    /**
     * @brief Send message 
     * 
     * @param msg message to send.
     */
    void send(const message_type& msg);

private:

    /**
     * @brief Reads message (async).
     * @details Reads data from socket until '\0' (null terminator).  
     */
    void read_message(void);

    /**
     * @brief Reconstruct message and push it to receive queue (async).
     * @details Creates new owned_message with this as owner and recreated message.
     * Push message in receive_queue with strand 
     * 
     * @param ec error code
     * @param bytes_read number of bytes was read from socket
     */
    void read_message_done(boost::system::error_code const& ec, size_t bytes_read);

    void enqueue_message(const message_type& msg);

    void start_send(void);

    void send_done(const boost::system::error_code& ec);
private:
    // ASIO context reference.
    boost::asio::io_context& _context;
    // TCP socket. 
    boost::asio::ip::tcp::socket _socket;
    
    // Read message placeholder
    boost::asio::streambuf _receive_packet;

    // Synchronization strand. Prevents simultaneous writes to socket.
    boost::asio::io_context::strand _write_strand;
    // Outgoing message queue.
    message_queue _send_queue;

    // Message handler
    std::shared_ptr<message_handler> _message_handler;
};

} // namespace cs_net

#endif // __CSNET_CONNECTION_HANDLER_HPP__