#include <Wire.h>
#include <Arduino_LSM6DS3.h>

void setup() {
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  // Read accelerometer data
  float ax, ay, az;
  IMU.readAcceleration(ax, ay, az);

  // Read gyroscope data
  float gx, gy, gz;
  IMU.readGyroscope(gx, gy, gz);

  // Print the accelerometer data
  Serial.print("Acceleration (m/s^2): ");
  Serial.print("X = "); Serial.print(ax, 4);
  Serial.print(", Y = "); Serial.print(ay, 4);
  Serial.print(", Z = "); Serial.println(az, 4);

  // Print the gyroscope data
  Serial.print("Gyroscope (deg/s): ");
  Serial.print("X = "); Serial.print(gx, 4);
  Serial.print(", Y = "); Serial.print(gy, 4);
  Serial.print(", Z = "); Serial.println(gz, 4);

  delay(100); // Adjust the delay as needed to control the data rate
}
