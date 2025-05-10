#include "include/SocketConLib.h"
#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Global flag for signal handling
volatile sig_atomic_t running = 1;

// Signal handler for graceful termination
void signalHandler(int signum) {
    std::cout << "Signal " << signum << " received. Terminating..." << std::endl;
    running = 0;
}

int main() {
    // Set up signal handling
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    std::cout << "Server Node starting..." << std::endl;
    
    // Connect to GyroSensor Node (localhost:7003)
    SocketCon gyroClient(SocketCon::Mode::CLIENT, "127.0.0.1", 7003);
    if (!gyroClient.init()) {
        std::cerr << "Failed to connect to GyroSensor Node. Make sure it's running." << std::endl;
        return 1;
    }
    std::cout << "Connected to GyroSensor Node" << std::endl;
    
    // Connect to DigitalIO Node (localhost:7002)
    SocketCon digitalIOClient(SocketCon::Mode::CLIENT, "127.0.0.1", 7002);
    if (!digitalIOClient.init()) {
        std::cerr << "Failed to connect to DigitalIO Node. Make sure it's running." << std::endl;
        gyroClient.release();
        return 1;
    }
    std::cout << "Connected to DigitalIO Node" << std::endl;
    
    // Create server socket on port 7001
    SocketCon server(SocketCon::Mode::SERVER, "", 7001);
    if (!server.init()) {
        std::cerr << "Failed to initialize Server Node socket server" << std::endl;
        gyroClient.release();
        digitalIOClient.release();
        return 1;
    }
    
    std::cout << "Server Node started. Listening on port 7001..." << std::endl;
    
    // Get the local IP address to display to the user
    char hostname[128];
    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);
    
    gethostname(hostname, sizeof(hostname));
    std::cout << "Hostname: " << hostname << std::endl;
    
    std::cout << "The Client Node should connect to this server at <IP_ADDRESS>:7001" << std::endl;
    std::cout << "Replace <IP_ADDRESS> with the IP address of this Raspberry Pi" << std::endl;
    
    // Main processing loop
    while (running) {
        // Wait for a command from the client
        std::string command;
        if (server.receive(command)) {
            std::cout << "Received command from client: " << command << std::endl;
            
            // Determine which node should receive the command
            if (command.substr(0, 5) == "gyro:" || 
                command.substr(0, 5) == "temp:" || 
                command.substr(0, 4) == "acc:") {
                // Forward to GyroSensor Node
                gyroClient.send(command);
                
                // Get response from GyroSensor Node
                std::string response;
                if (gyroClient.receive(response)) {
                    // Forward response to client
                    std::cout << "GyroSensor response: " << response << std::endl;
                    server.send(response);
                } else {
                    server.send("error: GyroSensor Node disconnected:");
                }
            }
            else if (command.substr(0, 12) == "sensorState:" || 
                     command.substr(0, 11) == "sensorType:" || 
                     command.substr(0, 6) == "relay " || 
                     command.substr(0, 11) == "relayState:" || 
                     command.substr(0, 4) == "key:") {
                // Forward to DigitalIO Node
                digitalIOClient.send(command);
                
                // Get response from DigitalIO Node
                std::string response;
                if (digitalIOClient.receive(response)) {
                    // Forward response to client
                    std::cout << "DigitalIO response: " << response << std::endl;
                    server.send(response);
                } else {
                    server.send("error: DigitalIO Node disconnected:");
                }
            }
            else if (command == "close:") {
                // Forward close command to both nodes
                gyroClient.send(command);
                digitalIOClient.send(command);
                
                // Get responses
                std::string gyroResponse, digitalIOResponse;
                gyroClient.receive(gyroResponse);
                digitalIOClient.receive(digitalIOResponse);
                
                // Send response to client
                server.send("close ok:");
                
                // Exit the loop
                running = 0;
            }
            else {
                // Unknown command
                server.send("error: unknown command:");
            }
        } else {
            // If receive failed, the connection might be closed
            std::cout << "Client disconnected" << std::endl;
            break;
        }
    }
    
    // Clean up resources
    server.release();
    gyroClient.release();
    digitalIOClient.release();
    
    std::cout << "Server Node terminated" << std::endl;
    
    return 0;
}