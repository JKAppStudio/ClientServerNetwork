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

    owned_message(std::weak_ptr<connection_handler> weak_handler, const message_type& msg)
    : owner(std::move(weak_handler))
    , message(msg)
    {}
};

/// Owned message queue
typedef std::deque<owned_message> owned_message_queue;

class message_handler
{
public:
    virtual void dispatch_message(const owned_message& owned_msg) {};
    virtual void on_connection(std::weak_ptr<connection_handler> handler) {};
};

} // namespace cs_net

#endif