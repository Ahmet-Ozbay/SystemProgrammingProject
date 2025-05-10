#include "../include/RelayLib.h"
#include <wiringPi.h>
#include <iostream>

Relay::Relay() : initialized(false), currentState(false) {
    // Constructor implementation
}

Relay::~Relay() {
    // Clean up if initialized
    if (initialized) {
        release();
    }
}

void Relay::init() {
    // Initialize wiringPi library if not already initialized
    if (wiringPiSetupGpio() == -1) {
        std::cerr << "Failed to initialize wiringPi" << std::endl;
        return;
    }
    
    // Configure the relay pin as output
    pinMode(RELAY_PIN, OUTPUT);
    
    // Initialize the relay to OFF state
    digitalWrite(RELAY_PIN, LOW);
    currentState = false;
    
    initialized = true;
    std::cout << "Relay initialized successfully" << std::endl;
}

void Relay::release() {
    if (!initialized) {
        return;
    }
    
    // Turn off the relay before releasing
    digitalWrite(RELAY_PIN, LOW);
    
    // Reset pin to input mode (safe state)
    pinMode(RELAY_PIN, INPUT);
    
    initialized = false;
    currentState = false;
    std::cout << "Relay resources released" << std::endl;
}

bool Relay::set(bool state) {
    if (!initialized) {
        std::cerr << "Relay not initialized" << std::endl;
        return false;
    }
    
    // Set the relay state
    digitalWrite(RELAY_PIN, state ? HIGH : LOW);
    currentState = state;
    
    std::cout << "Relay set to " << (state ? "ON" : "OFF") << std::endl;
    return true;
}

bool Relay::getState() const {
    return currentState;
}