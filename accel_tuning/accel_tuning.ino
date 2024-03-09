

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 mpu;
ACCEL_FS accelRange = ACCEL_FS::A2G;
GYRO_FS gyroRange = GYRO_FS::G250DPS;

void setup() {

  // set up i2c bus
  Wire.begin();

  // set up serial monitor
  Serial.begin(115200);

  // ensure mpu connection
  if (!mpu.testConnection()) {
    Serial.println("Failed connection to MPU6050.");
    while(1);
  }
  
  // settings
  mpu.initialize(accelRange, gyroRange);

  // do callibration
  mpu.CalibrateAccel(50);
  

}

void loop() {
  
  // individual axis
  // Serial.print(mpu.getAccelerationX());
  // Serial.print(" ");
  // Serial.print(mpu.getAccelerationY());
  // Serial.print(" ");
  // Serial.print(mpu.getAccelerationZ());
  // Serial.print(" ");
  // Serial.println();

  // magnitude
  double magnitude = sqrt( sq(mps(mpu.getAccelerationX())) + sq(mps(mpu.getAccelerationY())) + sq(mps(mpu.getAccelerationZ())) );
  Serial.println(magnitude, 4);

  delay(20);

}

// convert raw measurement to Gs
double Gs(int16_t rawData) {
  return ((double) rawData / (1<<12)) * 2;
}


// convert raw measurement to m/s^2
double mps(int16_t rawData) {
  return Gs(rawData * 9.81);
}
