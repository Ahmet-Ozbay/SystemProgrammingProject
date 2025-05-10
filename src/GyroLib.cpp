#include "../include/GyroLib.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstdint>
#include <cstring>
#include <iostream>

Gyro::Gyro() : i2c_fd(-1) {
    // Initialize with an invalid file descriptor
}

Gyro::~Gyro() {
    // Close the I2C device if it's open
    if (i2c_fd >= 0) {
        close(i2c_fd);
    }
}

void Gyro::init() {
    // Open the I2C device
    i2c_fd = open("/dev/i2c-1", O_RDWR);
    if (i2c_fd < 0) {
        std::cerr << "Failed to open I2C device" << std::endl;
        return;
    }

    // Set the I2C slave address
    if (ioctl(i2c_fd, I2C_SLAVE, MPU9250_ADDRESS) < 0) {
        std::cerr << "Failed to set I2C slave address" << std::endl;
        close(i2c_fd);
        i2c_fd = -1;
        return;
    }

    // Wake up the MPU9250 (Power management register)
    uint8_t data[2] = {0x6B, 0x00};
    if (write(i2c_fd, data, 2) != 2) {
        std::cerr << "Failed to wake up the MPU9250" << std::endl;
        close(i2c_fd);
        i2c_fd = -1;
        return;
    }

    // Configure gyroscope range (± 250 degrees/s)
    data[0] = 0x1B;
    data[1] = 0x00;
    if (write(i2c_fd, data, 2) != 2) {
        std::cerr << "Failed to configure gyroscope range" << std::endl;
        close(i2c_fd);
        i2c_fd = -1;
        return;
    }

    // Configure accelerometer range (± 2g)
    data[0] = 0x1C;
    data[1] = 0x00;
    if (write(i2c_fd, data, 2) != 2) {
        std::cerr << "Failed to configure accelerometer range" << std::endl;
        close(i2c_fd);
        i2c_fd = -1;
        return;
    }

    std::cout << "MPU9250 initialized successfully" << std::endl;
}

int16_t Gyro::readRawValue(int reg_addr) {
    // Check if the device is initialized
    if (i2c_fd < 0) {
        std::cerr << "I2C device not initialized" << std::endl;
        return 0;
    }

    // Set the register address
    uint8_t reg = static_cast<uint8_t>(reg_addr);
    if (write(i2c_fd, &reg, 1) != 1) {
        std::cerr << "Failed to set register address" << std::endl;
        return 0;
    }

    // Read 2 bytes (16-bit value)
    uint8_t data[2];
    if (read(i2c_fd, data, 2) != 2) {
        std::cerr << "Failed to read register data" << std::endl;
        return 0;
    }

    // Combine the bytes into a 16-bit signed integer
    return (data[0] << 8) | data[1];
}

double Gyro::getGyroX() {
    return readRawValue(GYRO_XOUT_H) / GYRO_SCALE;
}

double Gyro::getGyroY() {
    return readRawValue(GYRO_XOUT_H + 2) / GYRO_SCALE;
}

double Gyro::getGyroZ() {
    return readRawValue(GYRO_XOUT_H + 4) / GYRO_SCALE;
}

double Gyro::getAccX() {
    return readRawValue(ACCEL_XOUT_H) / ACCEL_SCALE * 9.81;  // Convert to m/s^2
}

double Gyro::getAccY() {
    return readRawValue(ACCEL_XOUT_H + 2) / ACCEL_SCALE * 9.81;  // Convert to m/s^2
}

double Gyro::getAccZ() {
    return readRawValue(ACCEL_XOUT_H + 4) / ACCEL_SCALE * 9.81;  // Convert to m/s^2
}

double Gyro::getTemp() {
    // Temperature formula from MPU9250 datasheet
    return readRawValue(TEMP_OUT_H) / 333.87 + 21.0;  // Convert to Celsius
}