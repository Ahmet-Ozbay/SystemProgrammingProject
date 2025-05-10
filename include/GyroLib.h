#ifndef GYRO_LIB_H
#define GYRO_LIB_H
#include <cstdint>
/**
 * @brief Class for interfacing with the MPU9250 gyroscope/accelerometer sensor
 * 
 * This class provides methods to initialize and read data from the MPU9250 sensor
 * connected to the Raspberry Pi via I2C (GPIO pins 2 and 3)
 */
class Gyro {
public:
    /**
     * @brief Constructor for the Gyro class
     */
    Gyro();

    /**
     * @brief Destructor for the Gyro class
     */
    ~Gyro();

    /**
     * @brief Initialize the MPU9250 sensor
     * 
     * @return void
     */
    void init();

    /**
     * @brief Get gyroscope X-axis value
     * 
     * @return double Gyroscope X-axis value in degrees per second
     */
    double getGyroX();

    /**
     * @brief Get gyroscope Y-axis value
     * 
     * @return double Gyroscope Y-axis value in degrees per second
     */
    double getGyroY();

    /**
     * @brief Get gyroscope Z-axis value
     * 
     * @return double Gyroscope Z-axis value in degrees per second
     */
    double getGyroZ();

    /**
     * @brief Get accelerometer X-axis value
     * 
     * @return double Accelerometer X-axis value in m/s^2
     */
    double getAccX();

    /**
     * @brief Get accelerometer Y-axis value
     * 
     * @return double Accelerometer Y-axis value in m/s^2
     */
    double getAccY();

    /**
     * @brief Get accelerometer Z-axis value
     * 
     * @return double Accelerometer Z-axis value in m/s^2
     */
    double getAccZ();

    /**
     * @brief Get sensor temperature
     * 
     * @return double Temperature value in degrees Celsius
     */
    double getTemp();

private:
    // Handle for the I2C device
    int i2c_fd;
    
    // MPU9250 register addresses
    static const int MPU9250_ADDRESS = 0x68;
    static const int ACCEL_XOUT_H = 0x3B;
    static const int GYRO_XOUT_H = 0x43;
    static const int TEMP_OUT_H = 0x41;
    
    // Scaling factors for raw data
    static constexpr double GYRO_SCALE = 131.0;  // For +/- 250 deg/s range
    static constexpr double ACCEL_SCALE = 16384.0;  // For +/- 2g range
    
    /**
     * @brief Read a 16-bit value from the sensor
     * 
     * @param reg_addr Register address to read from
     * @return int16_t 16-bit signed integer read from the register
     */
    int16_t readRawValue(int reg_addr);
};

#endif // GYRO_LIB_H
