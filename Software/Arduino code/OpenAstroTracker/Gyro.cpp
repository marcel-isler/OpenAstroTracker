#include "Gyro.hpp"

#include<Wire.h> // I2C communication library

Gyro::Gyro(float pitchAngleOffset, float rollAngleOffset) {
    _pitchAngleOffset = pitchAngleOffset;
    _rollAngleOffset = rollAngleOffset;

    // Initialize interface to the MPU6050
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    // read the values 100 times for them to stabilize
    for (int i = 0; i < 100; i++) {
        Wire.beginTransmission(MPU);
        Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
        AcX = Wire.read() << 8 | Wire.read(); // X-axis value
        AcY = Wire.read() << 8 | Wire.read(); // Y-axis value
        AcZ = Wire.read() << 8 | Wire.read(); // Z-axis value
    }
}

angle_t Gyro::getCurrentAngles() {
    // Read the accelerometer data
    Wire.beginTransmission(MPU);
    Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    AcX = Wire.read() << 8 | Wire.read(); // X-axis value
    AcY = Wire.read() << 8 | Wire.read(); // Y-axis value
    AcZ = Wire.read() << 8 | Wire.read(); // Z-axis value

    // Calculating the Pitch angle (rotation around Y-axis)
    pitchAngle = round((atan(-1 * AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2))) * 180.0 / PI) * 2.0) / 2.0;
    // Calculating the Roll angle (rotation around X-axis)
    rollAngle = round((atan(-1 * AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2))) * 180.0 / PI) * 2.0) / 2.0;
    struct angle_t result;
    // I mounted the device wrong, probably want to make that a configuration option to flip the axis
    result.pitchAngle = rollAngle;
    result.rollAngle = pitchAngle;
    return result;
}