
/** 
 * @file server.hpp
 * @brief Server interface.
 * 
 * @author Evgeny Krivoshein
 */

#ifndef __CSNET_SERVER_HPP__
#define __CSNET_SERVER_HPP__

#include <connection_handler.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>


namespace cs_net{

/**
 * @brief Listening port, accepts client connections, instantiates connection handlers.
 * 
 * @details Server listening port for incoming connections. 
 * Once connection encountered, accepts/declines client connection.
 * Instantiates connection handler for each client.
 * 
 * @param Service communication protocol based on message_handler class
 */
class Server
{

public:

    // Do not allow to copy Server.
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    /**
     * @brief Construct a new csNet server object.
     * @param thread_count number of threads, available for server.
     */
    Server(std::shared_ptr<message_handler> service, int thread_count = 1);

    /**
     * @brief Destroy the Server object
     */
    ~Server();

    /**
     * @brief Starts server.
     * @param port port number to start server on.
     */
    void start_server(uint16_t port);

    /**
     * @brief Stop server.
     * @details Stop server by stoping ASIO io_context, closing opened connections and joining dedicated threads, 
     */
    void stop_server(void);

    /**
     * @brief Permit/decline connection.
     * @details Virtual method for connection filtering. By default permits all connections.
     * @param handler connection handler
     */
    virtual bool accept(shared_handler_t handler);

private:
    /**
     * @brief Start connection handler for new connection.
     * @details If connection accepted, push connection in pool and starts handler.
     * If not, dissconnects.
     * @param handler connection handler object  
     * @param error error code
     */
    void handle_new_connection(shared_handler_t handler, boost::system::error_code const& error);

    // Number of threads, available for server to run asio io_context.
    int _thread_count;
    // Server thread pool
    std::vector<std::thread> _thread_pool;

    // ASIO context
    boost::asio::io_context _context;
    // ASIO acceptor
    boost::asio::ip::tcp::acceptor _acceptor;

    // Commenication Service
    std::shared_ptr<message_handler> _message_handler;    
};
} // namespace cs_net

#endif
