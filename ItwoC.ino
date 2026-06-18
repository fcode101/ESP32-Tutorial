#include <Arduino.h>
#include <Wire.h>

#define DEVICE_ADDR 0x08

void setup() {
  Wire.begin(21, 22);   // SDA, SCL
  Serial.begin(115200);
}

void loop() {

  Wire.beginTransmission(DEVICE_ADDR);
  Wire.write("Hello from ESP32");
  byte status = Wire.endTransmission();

  if (status == 0) {
    Serial.println("Data Sent");
  } else {
    Serial.println("Transmission Failed");
  }

  Wire.requestFrom(DEVICE_ADDR, 1);

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print("Received: ");
    Serial.println(c);
  }

  delay(2000);
}

/*
Author : F-CODE101
GitHub : https://github.com/fcode101
YouTube : https://www.youtube.com/@F-Code101

This is an I2C communication example.

The working principle is the same for all sensors and modules that support I2C.
However the transmitted and received data will vary depending on the device.

For more details visit my YouTube channel and Instagram page.

*/
