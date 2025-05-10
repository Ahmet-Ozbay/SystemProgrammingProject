/**
 * ClientNode.cpp - Client application for Raspberry Pi remote control system
 * 
 * This program runs on the PC and communicates with the ServerNode on the
 * Raspberry Pi through Ethernet (TCP/IP). It provides a text-based menu
 * interface for controlling and monitoring the remote system.
 */

 #include <iostream>
 #include <string>
 #include <sstream>
 #include <thread>
 #include <chrono>
 #include <cstdlib>
 #include <cctype>
 #include "../include/SocketConLib.h"
 
 // Function prototypes
 void displayMenu();
 void getSensorStatus(SocketCon& socket);
 void getSensorType(SocketCon& socket);
 void controlRelay(SocketCon& socket);
 void getRelayState(SocketCon& socket);
 void automaticControl(SocketCon& socket);
 void getGyroData(SocketCon& socket);
 void getAccData(SocketCon& socket);
 void getTemperature(SocketCon& socket);
 void getKeypadData(SocketCon& socket);
 void clearScreen();
 
 int main(int argc, char* argv[]) {
     std::string serverIP = "127.0.0.1"; // Default to localhost
     int serverPort = 7001;  // Default port
 
     // Check if IP address is provided as argument
     if (argc > 1) {
         serverIP = argv[1];
     }
     
     // Check if port is provided as argument
     if (argc > 2) {
         serverPort = std::stoi(argv[2]);
     }
     
     std::cout << "Raspberry Pi Remote Control System - Client" << std::endl;
     std::cout << "Connecting to server at " << serverIP << ":" << serverPort << std::endl;
     
     // Create socket connection
     SocketCon socket;
     socket.init(serverIP, serverPort, false); // false means client mode
     
     std::string response;
     bool running = true;
     
     while (running) {
         displayMenu();
         
         std::string choice;
         std::cout << "Enter your choice: ";
         std::getline(std::cin, choice);
         
         if (choice.empty()) {
             continue;
         }
         
         clearScreen();
         
         switch (choice[0]) {
             case '1':
                 getSensorStatus(socket);
                 break;
             case '2':
                 getSensorType(socket);
                 break;
             case '3':
                 controlRelay(socket);
                 break;
             case '4':
                 getRelayState(socket);
                 break;
             case '5':
                 automaticControl(socket);
                 break;
             case '6':
                 getGyroData(socket);
                 break;
             case '7':
                 getAccData(socket);
                 break;
             case '8':
                 getTemperature(socket);
                 break;
             case '9':
                 getKeypadData(socket);
                 break;
             case '0':
             case 'q':
             case 'Q':
                 std::cout << "Closing connection..." << std::endl;
                 socket.send("close:");
                 socket.receive(response);
                 std::cout << "Server response: " << response << std::endl;
                 running = false;
                 break;
             default:
                 std::cout << "Invalid choice. Please try again." << std::endl;
                 break;
         }
         
         if (running) {
             std::cout << "\nPress Enter to continue...";
             std::cin.get();
             clearScreen();
         }
     }
     
     socket.release();
     return 0;
 }
 
 void displayMenu() {
     std::cout << "\n=============== MENU ===============" << std::endl;
     std::cout << "1. Get Sensor Status" << std::endl;
     std::cout << "2. Get Sensor Type" << std::endl;
     std::cout << "3. Control Relay" << std::endl;
     std::cout << "4. Get Relay State" << std::endl;
     std::cout << "5. Automatic Control" << std::endl;
     std::cout << "6. Get Gyro Data" << std::endl;
     std::cout << "7. Get Acceleration Data" << std::endl;
     std::cout << "8. Get Temperature" << std::endl;
     std::cout << "9. Get Keypad Data" << std::endl;
     std::cout << "0. Exit" << std::endl;
     std::cout << "===================================" << std::endl;
 }
 
 void getSensorStatus(SocketCon& socket) {
     std::string response;
     
     std::cout << "Requesting sensor status..." << std::endl;
     socket.send("sensorState:");
     socket.receive(response);
     
     // Parse the response
     if (response.find("sensorState") != std::string::npos) {
         size_t pos = response.find(" ");
         if (pos != std::string::npos) {
             std::string state = response.substr(pos + 1);
             std::cout << "Sensor State: " << (state == "1" ? "ON" : "OFF") << std::endl;
         }
     } else {
         std::cout << "Unexpected response: " << response << std::endl;
     }
 }
 
 void getSensorType(SocketCon& socket) {
     std::string response;
     
     std::cout << "Requesting sensor type..." << std::endl;
     socket.send("sensorType:");
     socket.receive(response);
     
     // Parse the response
     if (response.find("sensorType") != std::string::npos) {
         size_t pos = response.find(" ");
         if (pos != std::string::npos) {
             std::string type = response.substr(pos + 1);
             std::cout << "Sensor Type: " << type << std::endl;
         }
     } else {
         std::cout << "Unexpected response: " << response << std::endl;
     }
 }
 
 void controlRelay(SocketCon& socket) {
     std::string response;
     
     // First get current state
     socket.send("relayState:");
     socket.receive(response);
     
     bool currentState = false;
     if (response.find("relay") != std::string::npos) {
         size_t pos = response.find(" ");
         if (pos != std::string::npos) {
             std::string state = response.substr(pos + 1);
             currentState = (state == "1");
         }
     }
     
     std::cout << "Driver Status: " << (currentState ? "ON" : "OFF") << std::endl;
     std::cout << "Press " << (currentState ? "0 to switch it OFF" : "1 to switch it ON") << ": ";
     
     std::string choice;
     std::getline(std::cin, choice);
     
     if (!choice.empty() && (choice[0] == '0' || choice[0] == '1')) {
         std::string command = "relay " + std::string(1, choice[0]) + ":";
         socket.send(command);
         socket.receive(response);
         
         if (response.find("relay ok") != std::string::npos) {
             std::cout << "Relay state changed successfully." << std::endl;
         } else {
             std::cout << "Failed to change relay state: " << response << std::endl;
         }
     } else {
         std::cout << "Invalid input. No changes made." << std::endl;
     }
 }
 
 void getRelayState(SocketCon& socket) {
     std::string response;
     
     std::cout << "Requesting relay state..." << std::endl;
     socket.send("relayState:");
     socket.receive(response);
     
     // Parse the response
     if (response.find("relay") != std::string::npos) {
         size_t pos = response.find(" ");
         if (pos != std::string::npos) {
             std::string state = response.substr(pos + 1);
             std::cout << "Driver Status: " << (state == "1" ? "ON" : "OFF") << std::endl;
         }
     } else {
         std::cout << "Unexpected response: " << response << std::endl;
     }
 }
 
 void automaticControl(SocketCon& socket) {
     std::string response;
     
     // Get sensor type
     socket.send("sensorType:");
     socket.receive(response);
     
     std::string sensorType = "UNKNOWN";
     if (response.find("sensorType") != std::string::npos) {
         size_t pos = response.find(" ");
         if (pos != std::string::npos) {
             sensorType = response.substr(pos + 1);
         }
     }
     
     std::cout << "Sensor List:" << std::endl;
     std::cout << "Type: " << sensorType << std::endl;
     std::cout << "In which state of the sensor should the driver be ON? (1-ON/0-OFF)" << std::endl;
     std::cout << "> ";
     
     std::string desiredState;
     std::getline(std::cin, desiredState);
     
     if (desiredState != "0" && desiredState != "1") {
         std::cout << "Invalid input. Automatic control canceled." << std::endl;
         return;
     }
     
     std::cout << "Right now the control is on. If the sensor is " 
               << (desiredState == "1" ? "ON" : "OFF") 
               << ", the driver will be ON. Press \"e\" to exit..." << std::endl;
     
     bool running = true;
     std::thread inputThread([&running]() {
         char input;
         while (running) {
             input = std::cin.get();
             if (input == 'e' || input == 'E') {
                 running = false;
             }
         }
     });
     
     // Main control loop
     while (running) {
         // Get sensor state
         socket.send("sensorState:");
         socket.receive(response);
         
         std::string sensorState = "";
         if (response.find("sensorState") != std::string::npos) {
             size_t pos = response.find(" ");
             if (pos != std::string::npos) {
                 sensorState = response.substr(pos + 1);
             }
         }
         
         // If sensor state matches desired state, turn relay ON; otherwise, turn it OFF
         if (sensorState == desiredState) {
             socket.send("relay 1:");
             socket.receive(response); // Consume response
         } else {
             socket.send("relay 0:");
             socket.receive(response); // Consume response
         }
         
         // Wait a bit before checking again
         std::this_thread::sleep_for(std::chrono::milliseconds(500));
     }
     
     // Clean up
     if (inputThread.joinable()) {
         inputThread.join();
     }
     
     // Force back to manual mode
     std::cout << "Automatic control has been stopped." << std::endl;
 }
 
 void getGyroData(SocketCon& socket) {
     std::string response;
     
     std::cout << "Requesting gyro data..." << std::endl;
     socket.send("gyro:");
     socket.receive(response);
     
     // Parse the response
     if (response.find("gyro") != std::string::npos) {
         std::istringstream iss(response.substr(5)); // Skip "gyro "
         float x, y, z;
         if (iss >> x >> y >> z) {
             std::cout << "Gyro: x: " << x << " y: " << y << " z: " << z << " [deg/sec]" << std::endl;
         }
     } else {
         std::cout << "Unexpected response: " << response << std::endl;
     }
 }
 
 void getAccData(SocketCon& socket) {
     std::string response;
     
     std::cout << "Requesting acceleration data..." << std::endl;
     socket.send("acc:");
     socket.receive(response);
     
     // Parse the response
     if (response.find("acc") != std::string::npos) {
         std::istringstream iss(response.substr(4)); // Skip "acc "
         float x, y, z;
         if (iss >> x >> y >> z) {
             std::cout << "Acceleration: x: " << x << " y: " << y << " z: " << z << " [m/s²]" << std::endl;
         }
     } else {
         std::cout << "Unexpected response: " << response << std::endl;
     }
 }
 
 void getTemperature(SocketCon& socket) {
     std::string response;
     
     std::cout << "Requesting temperature data..." << std::endl;
     socket.send("temp:");
     socket.receive(response);
     
     // Parse the response
     if (response.find("temp") != std::string::npos) {
         std::istringstream iss(response.substr(5)); // Skip "temp "
         float temp;
         if (iss >> temp) {
             std::cout << "Temperature: " << temp << " C" << std::endl;
         }
     } else {
         std::cout << "Unexpected response: " << response << std::endl;
     }
 }
 
 void getKeypadData(SocketCon& socket) {
     std::string response;
     
     std::cout << "Requesting keypad data..." << std::endl;
     socket.send("key:");
     socket.receive(response);
     
     // Parse the response
     if (response.find("key") != std::string::npos) {
         size_t pos = response.find(" ");
         if (pos != std::string::npos) {
             std::string key = response.substr(pos + 1);
             std::cout << "KEY: " << key << std::endl;
         }
     } else {
         std::cout << "Unexpected response: " << response << std::endl;
     }
 }
 
 void clearScreen() {
 #ifdef _WIN32
     std::system("cls");
 #else
     std::system("clear");
 #endif
 }