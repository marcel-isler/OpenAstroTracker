#include "Configuration_adv.hpp"

struct angle_t { float pitchAngle; float rollAngle; };

class Gyro {
public:
  Gyro(float pitchAngleOffset, float rollAngleOffset);

  angle_t getCurrentAngles();

private:
  float _pitchAngleOffset;
  float _rollAngleOffset;

  const int MPU = 0x68; // I2C address of the MPU6050 accelerometer
  int16_t AcX, AcY, AcZ;
  int16_t AcXOffset, AcYOffset, AcZOffset;
  int axis = 0;
  float pitchAngle = 0.0;
  float rollAngle = 0.0;
};
