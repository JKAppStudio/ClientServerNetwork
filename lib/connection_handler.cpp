/**
 * @file connection_handler.cpp
 * @author Evgeny Krivoshein
 * @brief Connection handler implementation
 */

#include <connection_handler.hpp>

namespace cs_net{

// Constructor
connection_handler::connection_handler(boost::asio::io_context& context, std::shared_ptr<message_handler> service)
    : _context(context)
    , _socket(context)
    , _write_strand(context) 
    , _message_handler(service)
{}

// Start
void connection_handler::start(void){
    read_message();
}

void connection_handler::disconnect(void){
    _socket.close();
}

// Remote address
std::string connection_handler::remote_address(void) const{
    return _socket.remote_endpoint().address().to_string() + ":" + std::to_string(_socket.remote_endpoint().port());
}

void connection_handler::read_message(void)
{
    boost::asio::async_read_until(_socket,
                            _receive_packet,
                            '\0',
                            [self=this->shared_from_this()] (auto ec, size_t bytes_read)
                            {
                                self->read_message_done(ec, bytes_read);
                            } );
}

void connection_handler::read_message_done(boost::system::error_code const& ec, size_t bytes_read)
{
    if (ec) return;
    auto self = this->shared_from_this();
    std::weak_ptr<connection_handler> weak_handler = self;
    std::istream stream(&_receive_packet);
    message_type packet;
    stream >> packet;
    owned_message msg(weak_handler, packet);
    _context.post([self, msg](){
        self->_message_handler->dispatch_message(msg);
        });
    read_message();
}

} //namespace cs_net