

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

  // For the first one second, this for loop will run and compute the average of 200 data points for each acceleration axis. This value will then be used as an offset
  // for each axis before the magnitude is computed.
  double sumx = 0;
  double sumy = 0;
  double sumz = 0;
  for(int i = 1; i < 200; ++i) {
    double ax = mpu.getAccelerationX();
    double ay = mpu.getAccelerationY();
    double az = mpu.getAccelerationZ();
    sumx += ax;
    sumy += ay;
    sumz += az;
    delay(0.5);
  }

  double offsetx = sumx/200;
  double offsety = sumy/200;
  double offsetz = sumz/200;

  // magnitude (I divided this by 9.81 to make the serial print happen in G's)
  double magnitude = (sqrt(sq(mps(mpu.getAccelerationX() - offsetx)) + sq(mps(mpu.getAccelerationY() - offsety)) + sq(mps(mpu.getAccelerationZ() - offsetz)) ))/9.81;
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
