#include "../include/SocketConLib.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

SocketCon::SocketCon(Mode mode, const std::string& host, int port) 
    : mode(mode), host(host), port(port), sockfd(-1), clientfd(-1), connected(false) {
    // Constructor implementation
}

SocketCon::~SocketCon() {
    // Clean up if connected
    if (connected) {
        release();
    }
}

bool SocketCon::init() {
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options" << std::endl;
        close(sockfd);
        sockfd = -1;
        return false;
    }
    
    // Handle SERVER or CLIENT mode
    if (mode == Mode::SERVER) {
        // Bind the socket to the specified port
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);
        
        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to bind socket to port " << port << std::endl;
            close(sockfd);
            sockfd = -1;
            return false;
        }
        
        // Listen for incoming connections
        if (listen(sockfd, 5) < 0) {
            std::cerr << "Failed to listen on socket" << std::endl;
            close(sockfd);
            sockfd = -1;
            return false;
        }
        
        std::cout << "Server listening on port " << port << std::endl;
        
        // Accept the first incoming connection
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        std::cout << "Waiting for client connection..." << std::endl;
        
        clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (clientfd < 0) {
            std::cerr << "Failed to accept client connection" << std::endl;
            close(sockfd);
            sockfd = -1;
            return false;
        }
        
        std::cout << "Client connected from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
        
    } else if (mode == Mode::CLIENT) {
        // Connect to the server
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(host.c_str());
        server_addr.sin_port = htons(port);
        
        std::cout << "Connecting to server at " << host << ":" << port << std::endl;
        
        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to connect to server at " << host << ":" << port << std::endl;
            close(sockfd);
            sockfd = -1;
            return false;
        }
        
        std::cout << "Connected to server at " << host << ":" << port << std::endl;
        
        // Use sockfd as the client file descriptor in client mode
        clientfd = sockfd;
    }
    
    connected = true;
    return true;
}

void SocketCon::release() {
    if (!connected) {
        return;
    }
    
    // Close client socket if in server mode
    if (mode == Mode::SERVER && clientfd >= 0 && clientfd != sockfd) {
        close(clientfd);
        clientfd = -1;
    }
    
    // Close server socket
    if (sockfd >= 0) {
        close(sockfd);
        sockfd = -1;
    }
    
    connected = false;
    std::cout << "Socket connection closed" << std::endl;
}

bool SocketCon::send(const std::string& message) {
    if (!connected || clientfd < 0) {
        std::cerr << "Socket not connected" << std::endl;
        return false;
    }
    
    ssize_t bytes_sent = write(clientfd, message.c_str(), message.length());
    if (bytes_sent < 0) {
        std::cerr << "Failed to send message" << std::endl;
        return false;
    }
    
    return true;
}

bool SocketCon::receive(std::string& message) {
    if (!connected || clientfd < 0) {
        std::cerr << "Socket not connected" << std::endl;
        return false;
    }
    
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    
    ssize_t bytes_received = read(clientfd, buffer, BUFFER_SIZE - 1);
    if (bytes_received < 0) {
        std::cerr << "Failed to receive message" << std::endl;
        return false;
    } else if (bytes_received == 0) {
        // Connection closed by peer
        std::cout << "Connection closed by peer" << std::endl;
        connected = false;
        return false;
    }
    
    // Ensure null termination
    buffer[bytes_received] = '\0';
    
    // Update the output parameter
    message = std::string(buffer);
    
    return true;
}

bool SocketCon::isConnected() const {
    return connected;
}