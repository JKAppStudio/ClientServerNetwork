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
    _message_handler->on_connect(shared_from_this());
    read_message();

}

void connection_handler::disconnect(void){
    _message_handler->on_disconnect(shared_from_this());
    _socket.close();
}

// Remote address
std::string connection_handler::remote_address(void) const{
    return _socket.remote_endpoint().address().to_string() + ":" + std::to_string(_socket.remote_endpoint().port());
}

// Read message 
void connection_handler::read_message(void)
{
    // Read asynchronously till '\0' charecter. Then call read_message_done.
    boost::asio::async_read_until(_socket,
                            _receive_packet,
                            '\0',
                            [self=this->shared_from_this()] (auto ec, size_t bytes_read)
                            {
                                self->read_message_done(ec, bytes_read);
                            } );
}

// Read message done
void connection_handler::read_message_done(boost::system::error_code const& ec, size_t bytes_read)
{
    // Resign if error acquired
    if (ec) return;

    // Creat weak_ptr for this
    auto self = this->shared_from_this();
    std::weak_ptr<connection_handler> weak_handler = self;

    // Get message from streambuffer
    std::ostringstream stream;
    stream << &_receive_packet;
    message_type packet = stream.str();

    // Construct owned message from weak_ptr and incoming message
    owned_message msg(weak_handler, packet);

    // Call message dispatcher (async)
    boost::asio::post(_context,
        [self, msg](){
            self->_message_handler->on_message_receive(msg);
        });
    read_message();
}

// Send
void connection_handler::send(const message_type& msg) 
{
    // Post enqueue message to write strand
    boost::asio::post(boost::asio::bind_executor(_write_strand,
        [self=shared_from_this(), msg]()
        {
            self->enqueue_message(msg);
        })
    );    
}

// Enqueue message
void connection_handler::enqueue_message(const message_type& msg) 
{
    // Check if already writing
    bool write_in_progress = !_send_queue.empty();
    // Push message in send queue
    _send_queue.push_back(msg);
    // Start send if not already writing
    if(!write_in_progress) start_send();    
}

// Start send
void connection_handler::start_send(void) 
{
    // Add '\0' to message
    _send_queue.front() += '\0';
    // Write message asynchronously. Call send_done after.
    boost::asio::async_write(_socket
        , boost::asio::buffer(_send_queue.front())
        , boost::asio::bind_executor(_write_strand,
            [self = shared_from_this()](const boost::system::error_code& ec, size_t)
            {
                self->send_done(ec);
            })
    );
}

//Send done
void connection_handler::send_done(const boost::system::error_code& ec) 
{
    // Resign if error acquired
    if(ec) return; 
    // Pop sent message
    _send_queue.pop_front();
    // Start send if there is message to send
    if(!_send_queue.empty()) start_send();
}

} //namespace cs_net