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
    std::ostringstream stream;
    stream << &_receive_packet;
    message_type packet = stream.str();
    // stream >> packet;
    owned_message msg(weak_handler, packet);
    _context.post([self, msg](){
        self->_message_handler->dispatch_message(msg);
        });
    read_message();
}

void connection_handler::send(const message_type& msg) 
{
    boost::asio::post(boost::asio::bind_executor(_write_strand,
        [self=shared_from_this(), msg]()
        {
            self->enqueue_message(msg);
        })
    );    
}

void connection_handler::enqueue_message(const message_type& msg) 
{
    bool ready_to_write = _send_queue.empty();
    _send_queue.push_back(msg);
    if(ready_to_write) start_send();    
}

void connection_handler::start_send(void) 
{
    _send_queue.front() += '\0';
    boost::asio::async_write(_socket
        , boost::asio::buffer(_send_queue.front())
        , boost::asio::bind_executor(_write_strand,
            [self = shared_from_this()](const boost::system::error_code& ec, size_t)
            {
                self->send_done(ec);
            })
    );
}

void connection_handler::send_done(const boost::system::error_code& ec) 
{
    if(ec) return; 
    _send_queue.pop_front();
    if(!_send_queue.empty()) start_send();
}

} //namespace cs_net