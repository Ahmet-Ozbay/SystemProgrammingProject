#ifndef RELAY_LIB_H
#define RELAY_LIB_H

/**
 * @brief Class for interfacing with a relay
 * 
 * This class provides methods to initialize and control a relay
 * connected to the Raspberry Pi GPIO pin 27
 */
class Relay {
public:
    /**
     * @brief Constructor for the Relay class
     */
    Relay();
    
    /**
     * @brief Destructor for the Relay class
     */
    ~Relay();
    
    /**
     * @brief Initialize the relay
     * 
     * @return void
     */
    void init();
    
    /**
     * @brief Clean up and release GPIO pins
     * 
     * @return void
     */
    void release();
    
    /**
     * @brief Set the state of the relay
     * 
     * @param state True to turn the relay ON, false to turn it OFF
     * @return bool True if operation was successful, false otherwise
     */
    bool set(bool state);
    
    /**
     * @brief Get the current state of the relay
     * 
     * @return bool True if relay is ON, false if it's OFF
     */
    bool getState() const;
    
private:
    // GPIO pin number for the relay
    static const int RELAY_PIN = 27;
    
    // Current state of the relay
    bool currentState;
    
    // Flag to track if the relay is initialized
    bool initialized;
};

#endif // RELAY_LIB_H