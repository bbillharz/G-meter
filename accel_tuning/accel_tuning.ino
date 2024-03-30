

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Streaming.h>

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

int16_t xMax = 0, yMax = 0, zMax = 0;
int16_t xMin = 0, yMin = 0, zMin = 0;

void loop() {

  // wait to read
  while (!Serial.available());
  Serial.read();
  
  // read axis
  int16_t newX, newY, newZ;
  mpu.getAcceleration(&newX, &newY, &newZ);

  // find max and min
  xMax = max(xMax, newX);
  yMax = max(yMax, newY);
  zMax = max(zMax, newZ);

  xMin = min(xMin, newX);
  yMin = min(yMin, newY);
  zMin = min(zMin, newZ);

  Serial << Gs(xMax) << " | " << Gs(yMax) << " | " << Gs(zMax) << endl;
  Serial << Gs(xMin) << " | " << Gs(yMin) << " | " << Gs(zMin) << endl;

  // magnitude
  // double magnitude = sqrt( sq(mps(mpu.getAccelerationX())) + sq(mps(mpu.getAccelerationY())) + sq(mps(mpu.getAccelerationZ())) );
  // Serial.println(magnitude, 4);

}

// convert raw measurement to Gs
double Gs(int16_t rawData) {
  return ((double) rawData / (1<<12)) * 2;
}


// convert raw measurement to m/s^2
double mps(int16_t rawData) {
  return Gs(rawData * 9.81);
}
