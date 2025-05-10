#include "../include/KeypadLib.h"
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>

// Define the static member variables
const std::vector<int> Keypad::ROW_PINS = {16, 20, 21, 12};
const std::vector<int> Keypad::COL_PINS = {6, 13, 19, 26};

// Define the keypad layout
const char Keypad::KEY_MAP[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad::Keypad() : initialized(false), keyBuffer("") {
    // Constructor implementation
}

Keypad::~Keypad() {
    // Call release to clean up if initialized
    if (initialized) {
        release();
    }
}

void Keypad::init() {
    // Initialize wiringPi library
    if (wiringPiSetupGpio() == -1) {
        std::cerr << "Failed to initialize wiringPi" << std::endl;
        return;
    }
    
    // Configure row pins as output
    for (int pin : ROW_PINS) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH); // Set to HIGH by default
    }
    
    // Configure column pins as input with pull-up resistors
    for (int pin : COL_PINS) {
        pinMode(pin, INPUT);
        pullUpDnControl(pin, PUD_UP); // Enable pull-up resistors
    }
    
    initialized = true;
    keyBuffer.clear();
    std::cout << "Keypad initialized successfully" << std::endl;
}

void Keypad::release() {
    if (!initialized) {
        return;
    }
    
    // Reset all pins to input mode (safe state)
    for (int pin : ROW_PINS) {
        pinMode(pin, INPUT);
    }
    
    for (int pin : COL_PINS) {
        pinMode(pin, INPUT);
    }
    
    initialized = false;
    keyBuffer.clear();
    std::cout << "Keypad resources released" << std::endl;
}

void Keypad::setAllRowsHigh() {
    for (int pin : ROW_PINS) {
        digitalWrite(pin, HIGH);
    }
}

char Keypad::scanKeypad() {
    if (!initialized) {
        std::cerr << "Keypad not initialized" << std::endl;
        return '\0';
    }
    
    // Scan the keypad
    for (size_t r = 0; r < ROW_PINS.size(); r++) {
        // Set current row LOW
        setAllRowsHigh();
        digitalWrite(ROW_PINS[r], LOW);
        
        // Check each column in the current row
        for (size_t c = 0; c < COL_PINS.size(); c++) {
            // If a key is pressed (column reads LOW)
            if (digitalRead(COL_PINS[c]) == LOW) {
                // Debounce
                delay(20);
                if (digitalRead(COL_PINS[c]) == LOW) {
                    // Wait until key is released
                    while (digitalRead(COL_PINS[c]) == LOW) {
                        usleep(1000); // Sleep for 1ms
                    }
                    // Return the key character
                    return KEY_MAP[r][c];
                }
            }
        }
    }
    
    // No key pressed
    return '\0';
}

char Keypad::getKey() {
    if (!initialized) {
        std::cerr << "Keypad not initialized" << std::endl;
        return '\0';
    }
    
    char key = scanKeypad();
    
    // If a key is pressed
    if (key != '\0') {
        // If '#' is pressed, reset the buffer
        if (key == '#') {
            // Don't add '#' to the buffer
            return key;
        }
        // Otherwise, add the key to the buffer
        keyBuffer += key;
    }
    
    return key;
}

std::string Keypad::getKeyBuffer() {
    return keyBuffer;
}

void Keypad::clearKeyBuffer() {
    keyBuffer.clear();
}