#include "include/GyroLib.h"
#include "include/SocketConLib.h"
#include <iostream>
#include <sstream>
#include <string>
#include <csignal>
#include <vector>

// Global flag for signal handling
volatile sig_atomic_t running = 1;

// Signal handler for graceful termination
void signalHandler(int signum) {
    std::cout << "Signal " << signum << " received. Terminating..." << std::endl;
    running = 0;
}

// Process commands received from the server
std::string processCommand(const std::string& command, Gyro& gyro) {
    std::stringstream response;
    
    if (command == "temp:") {
        double temp = gyro.getTemp();
        response << "temp " << temp << ":";
    }
    else if (command == "gyro:") {
        double x = gyro.getGyroX();
        double y = gyro.getGyroY();
        double z = gyro.getGyroZ();
        response << "gyro " << x << " " << y << " " << z << ":";
    }
    else if (command == "acc:") {
        double x = gyro.getAccX();
        double y = gyro.getAccY();
        double z = gyro.getAccZ();
        response << "acc " << x << " " << y << " " << z << ":";
    }
    else if (command == "close:") {
        // Handle close command
        response << "close ok:";
        running = 0;
    }
    else {
        // Unknown command
        response << "error: unknown command:";
    }
    
    return response.str();
}

int main() {
    // Set up signal handling
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Create and initialize the gyro sensor
    Gyro gyro;
    gyro.init();
    
    // Create socket server on port 7003
    SocketCon server(SocketCon::Mode::SERVER, "", 7003);
    
    std::cout << "GyroSensor Node starting..." << std::endl;
    
    // Initialize the socket server
    if (!server.init()) {
        std::cerr << "Failed to initialize GyroSensor Node socket server" << std::endl;
        return 1;
    }
    
    std::cout << "GyroSensor Node started. Listening on port 7003..." << std::endl;
    
    // Main processing loop
    while (running) {
        // Wait for a command from the server
        std::string command;
        if (server.receive(command)) {
            std::cout << "Received command: " << command << std::endl;
            
            // Process the command and send the response
            std::string response = processCommand(command, gyro);
            std::cout << "Sending response: " << response << std::endl;
            server.send(response);
            
            // Check if we received a close command
            if (command == "close:") {
                break;
            }
        } else {
            // If receive failed, the connection might be closed
            break;
        }
    }
    
    // Clean up resources
    server.release();
    
    std::cout << "GyroSensor Node terminated" << std::endl;
    
    return 0;
}