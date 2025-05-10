#ifndef KEYPAD_LIB_H
#define KEYPAD_LIB_H

#include <string>
#include <vector>

/**
 * @brief Class for interfacing with a matrix keypad
 * 
 * This class provides methods to initialize and read data from a matrix keypad
 * connected to the Raspberry Pi using GPIOs {16,20,21,12,06,13,19,26}
 */
class Keypad {
public:
    /**
     * @brief Constructor for the Keypad class
     */
    Keypad();
    
    /**
     * @brief Destructor for the Keypad class
     */
    ~Keypad();
    
    /**
     * @brief Initialize the keypad
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
     * @brief Get the pressed key
     * 
     * @return char The character associated with the pressed key, or '\0' if no key is pressed
     */
    char getKey();
    
    /**
     * @brief Get the stored key buffer
     * 
     * @return std::string The current key buffer
     */
    std::string getKeyBuffer();
    
    /**
     * @brief Clear the stored key buffer
     * 
     * @return void
     */
    void clearKeyBuffer();
    
private:
    // GPIO pin numbers for rows and columns
    static const std::vector<int> ROW_PINS;    // GPIO pins for rows
    static const std::vector<int> COL_PINS;    // GPIO pins for columns
    
    // Keypad layout
    static const char KEY_MAP[4][4];
    
    // Key buffer to store pressed keys until "#" is pressed
    std::string keyBuffer;
    
    // Flag to track if the keypad is initialized
    bool initialized;
    
    /**
     * @brief Set all row pins as output and set them to HIGH
     * 
     * @return void
     */
    void setAllRowsHigh();
    
    /**
     * @brief Read the keypad
     * 
     * Scans the keypad to detect pressed keys
     * 
     * @return char The character associated with the pressed key, or '\0' if no key is pressed
     */
    char scanKeypad();
};

#endif // KEYPAD_LIB_H