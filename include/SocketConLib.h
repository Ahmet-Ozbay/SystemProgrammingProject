#ifndef SOCKET_CON_LIB_H
#define SOCKET_CON_LIB_H

#include <string>

/**
 * @brief Class for socket communication
 * 
 * This class provides methods to initialize and manage socket connections
 * for both server and client modes
 */
class SocketCon {
public:
    /**
     * @brief Socket connection modes
     */
    enum class Mode {
        SERVER,    ///< Server mode: listens for incoming connections
        CLIENT     ///< Client mode: connects to a server
    };
    
    /**
     * @brief Constructor for the SocketCon class
     * 
     * @param mode Socket connection mode (SERVER or CLIENT)
     * @param host Host address (only relevant in CLIENT mode, ignored in SERVER mode)
     * @param port Port number to listen on (SERVER) or connect to (CLIENT)
     */
    SocketCon(Mode mode, const std::string& host, int port);
    
    /**
     * @brief Destructor for the SocketCon class
     */
    ~SocketCon();
    
    /**
     * @brief Initialize the socket connection
     * 
     * In SERVER mode, it creates a socket, binds it to the specified port, and listens for connections.
     * In CLIENT mode, it creates a socket and connects to the specified host and port.
     * 
     * @return bool True if initialization was successful, false otherwise
     */
    bool init();
    
    /**
     * @brief Close the socket connection and release resources
     * 
     * @return void
     */
    void release();
    
    /**
     * @brief Send a message through the socket
     * 
     * @param message The message to send
     * @return bool True if the message was sent successfully, false otherwise
     */
    bool send(const std::string& message);
    
    /**
     * @brief Receive a message from the socket
     * 
     * @param message Reference to store the received message
     * @return bool True if a message was received successfully, false otherwise
     */
    bool receive(std::string& message);
    
    /**
     * @brief Check if the socket is connected
     * 
     * @return bool True if the socket is connected, false otherwise
     */
    bool isConnected() const;
    
private:
    // Socket file descriptor
    int sockfd;
    
    // Client socket file descriptor (only used in SERVER mode)
    int clientfd;
    
    // Socket connection mode
    Mode mode;
    
    // Host address
    std::string host;
    
    // Port number
    int port;
    
    // Connection status
    bool connected;
    
    // Buffer size for receiving messages
    static const int BUFFER_SIZE = 1024;
};

#endif // SOCKET_CON_LIB_H