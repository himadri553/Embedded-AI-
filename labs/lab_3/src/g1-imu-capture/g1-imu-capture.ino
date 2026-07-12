#include <Arduino_BMI270_BMM150.h>

const float kTargetHz = 100.0f;
const unsigned long kIntervalUs = (unsigned long)(1000000.0f / kTargetHz);

void setup() {
  Serial.begin(115200);
  while (!Serial) { }

  if (!IMU.begin()) {
    Serial.println("IMU.begin() failed");
    while (1) { }
  }

  Serial.println("millis,ax,ay,az,gx,gy,gz");
  Serial.print("# accel rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.print(" Hz, gyro rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
}

void loop() {
  static unsigned long dueUs = 0;
  unsigned long clockUs = micros();

  if ((long)(clockUs - dueUs) < 0) return;
  dueUs = clockUs + kIntervalUs;

  if (!IMU.accelerationAvailable() || !IMU.gyroscopeAvailable()) return;

  float accX, accY, accZ, gyrX, gyrY, gyrZ;
  IMU.readAcceleration(accX, accY, accZ);
  IMU.readGyroscope(gyrX, gyrY, gyrZ);

  Serial.print(millis()); Serial.print(',');
  Serial.print(accX, 4);  Serial.print(',');
  Serial.print(accY, 4);  Serial.print(',');
  Serial.print(accZ, 4);  Serial.print(',');
  Serial.print(gyrX, 4);  Serial.print(',');
  Serial.print(gyrY, 4);  Serial.print(',');
  Serial.println(gyrZ, 4);
}
