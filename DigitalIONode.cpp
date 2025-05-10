#include "include/KeypadLib.h"
#include "include/DigSensorLib.h"
#include "include/RelayLib.h"
#include "include/SocketConLib.h"
#include <iostream>
#include <sstream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>

// Global flag for signal handling
volatile sig_atomic_t running = 1;

// Signal handler for graceful termination
void signalHandler(int signum) {
    std::cout << "Signal " << signum << " received. Terminating..." << std::endl;
    running = 0;
}

// Function to continuously check for keypad input
void keypadMonitor(Keypad& keypad) {
    while (running) {
        char key = keypad.getKey();
        if (key == '#') {
            std::cout << "Key sequence entered: " << keypad.getKeyBuffer() << std::endl;
        }
        // Small delay to prevent high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// Process commands received from the server
std::string processCommand(const std::string& command, DigSensor& sensor, Relay& relay, Keypad& keypad) {
    std::stringstream response;
    
    if (command == "sensorState:") {
        bool state = sensor.read();
        response << "sensorState " << (state ? "1" : "0") << ":";
    }
    else if (command == "sensorType:") {
        std::string type = sensor.getType();
        response << "sensorType " << type << ":";
    }
    else if (command.substr(0, 6) == "relay ") {
        // Extract the relay state from the command
        if (command.length() >= 8) {
            char stateChar = command[6];
            bool state = (stateChar == '1');
            
            if (relay.set(state)) {
                response << "relay ok:";
            } else {
                response << "relay err:";
            }
        } else {
            response << "relay err:";
        }
    }
    else if (command == "relayState:") {
        bool state = relay.getState();
        response << "relay " << (state ? "1" : "0") << ":";
    }
    else if (command == "key:") {
        std::string keyBuffer = keypad.getKeyBuffer();
        response << "key " << keyBuffer << ":";
        // Clear the key buffer after sending
        keypad.clearKeyBuffer();
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
    
    // Create and initialize the components
    DigSensor sensor;
    Relay relay;
    Keypad keypad;
    
    sensor.init();
    relay.init();
    keypad.init();
    
    // Create socket server on port 7002
    SocketCon server(SocketCon::Mode::SERVER, "", 7002);
    
    std::cout << "DigitalIO Node starting..." << std::endl;
    
    // Initialize the socket server
    if (!server.init()) {
        std::cerr << "Failed to initialize DigitalIO Node socket server" << std::endl;
        // Clean up resources
        sensor.release();
        relay.release();
        keypad.release();
        return 1;
    }
    
    std::cout << "DigitalIO Node started. Listening on port 7002..." << std::endl;
    
    // Start keypad monitoring in a separate thread
    std::thread keypadThread(keypadMonitor, std::ref(keypad));
    
    // Main processing loop
    while (running) {
        // Wait for a command from the server
        std::string command;
        if (server.receive(command)) {
            std::cout << "Received command: " << command << std::endl;
            
            // Process the command and send the response
            std::string response = processCommand(command, sensor, relay, keypad);
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
    
    // Signal the keypad thread to stop and wait for it to finish
    running = 0;
    if (keypadThread.joinable()) {
        keypadThread.join();
    }
    
    // Clean up resources
    server.release();
    sensor.release();
    relay.release();
    keypad.release();
    
    std::cout << "DigitalIO Node terminated" << std::endl;
    
    return 0;
}