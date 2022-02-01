/**
 * @file message_handler.hpp
 * @author Evgeny Krivoshein
 * @brief Virtual class for communication protocol.
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CS_NET_MESSAGE_HANDLER_HPP__
#define __CS_NET_MESSAGE_HANDLER_HPP__

#include <memory>
#include <iostream>
#include <deque>
#include <string>

namespace cs_net{

// Forward declaration
class connection_handler;


// Convinience types declaration
/// Shared pointer to connection handler
typedef std::shared_ptr<connection_handler> shared_handler_t;
/// Weak pointer to connection handler
typedef std::weak_ptr<connection_handler> weak_handler_t;

/// Message type
typedef std::string message_type;
/// Message queue
typedef std::deque<message_type> message_queue;

/**
 * @brief Holds message and message sender.
 * 
 * @details Stores weak pointer to sender connection_handler and message.
 * 
 */
struct owned_message{
    std::weak_ptr<connection_handler> owner;
    message_type message;

    owned_message(weak_handler_t weak_handler, const message_type& msg)
    : owner(std::move(weak_handler))
    , message(msg)
    {}
};

/// Owned message queue
typedef std::deque<owned_message> owned_message_queue;

/**
 * @brief Delegate class for handling communication protocol.
 * 
 * @details Protocol blueprint. 
 * Conteins callback functions set for connection handler call in appropriate time.
 *  
 */
class message_handler
{
public:
    /**
     * @brief On message receve callback function.
     * @details Called when connection handler receives new message.
     * 
     * @param owned_msg received message
     */
    virtual void on_message_receive(const owned_message& owned_msg) {};

    /**
     * @brief On connect callback function.
     * @details Called when connection established. 
     * 
     * @param weak_handler weak pointer to connection handler.
     */
    virtual void on_connect(weak_handler_t weak_handler) {};

    /**
     * @brief On disconnect callback function.
     * @details Called in connection handler's disconnect method before closing socket.  
     * 
     * @param weak_handler weak pointer to connection handler.
     */
    virtual void on_disconnect(weak_handler_t weak_handler) {};

};

} // namespace cs_net

#endif