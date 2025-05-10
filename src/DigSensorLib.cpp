#include "../include/DigSensorLib.h"
#include <wiringPi.h>
#include <iostream>

DigSensor::DigSensor() : initialized(false), sensorType("TEMPERATURE") {
    // Default sensor type is TEMPERATURE, can be changed if needed
}

DigSensor::~DigSensor() {
    // Clean up if initialized
    if (initialized) {
        release();
    }
}

void DigSensor::init() {
    // Initialize wiringPi library if not already initialized
    if (wiringPiSetupGpio() == -1) {
        std::cerr << "Failed to initialize wiringPi" << std::endl;
        return;
    }
    
    // Configure the sensor pin as input
    pinMode(SENSOR_PIN, INPUT);
    
    initialized = true;
    std::cout << "Digital sensor initialized successfully" << std::endl;
}

void DigSensor::release() {
    if (!initialized) {
        return;
    }
    
    // Reset pin to input mode (safe state)
    pinMode(SENSOR_PIN, INPUT);
    
    initialized = false;
    std::cout << "Digital sensor resources released" << std::endl;
}

bool DigSensor::read() {
    if (!initialized) {
        std::cerr << "Digital sensor not initialized" << std::endl;
        return false;
    }
    
    // Read the digital value from the sensor pin
    int value = digitalRead(SENSOR_PIN);
    
    // Return true if HIGH, false if LOW
    return (value == HIGH);
}

std::string DigSensor::getType() const {
    return sensorType;
}
