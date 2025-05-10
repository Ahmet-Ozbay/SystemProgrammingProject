#ifndef DIG_SENSOR_LIB_H
#define DIG_SENSOR_LIB_H

#include <string>

/**
 * @brief Class for interfacing with a digital sensor
 * 
 * This class provides methods to initialize and read data from a digital sensor
 * connected to the Raspberry Pi GPIO pin 17
 */
class DigSensor {
public:
    /**
     * @brief Constructor for the DigSensor class
     */
    DigSensor();
    
    /**
     * @brief Destructor for the DigSensor class
     */
    ~DigSensor();
    
    /**
     * @brief Initialize the digital sensor
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
     * @brief Read the value from the digital sensor
     * 
     * @return bool True if sensor is active (HIGH), false otherwise
     */
    bool read();
    
    /**
     * @brief Get the sensor type
     * 
     * @return std::string The type of the sensor
     */
    std::string getType() const;
    
private:
    // GPIO pin number for the sensor
    static const int SENSOR_PIN = 17;
    
    // Sensor type
    std::string sensorType;
    
    // Flag to track if the sensor is initialized
    bool initialized;
};

#endif // DIG_SENSOR_LIB_H
