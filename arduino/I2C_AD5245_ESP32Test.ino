/*
 * Using the I2C or "WIRE" interface to control 
 * the AD5245 Digital Potentiometer using the ESP32 DOIT (30 GPIO version)
 * to step through 64 levels of resistance
 * 
 * Created by Katharine Lundblad on December 30th 2020
 */

 #include "Arduino.h"
 #include <Wire.h>

 /* Define pins */
 #define SDA 21;
 #define SCL 22;
 #define CSB 3;
 #define SDO 1;
 
void setup() {
  Wire.begin(); // join i2c bus
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);

  /* SCL and SDA are active low */
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);
  delay(1); 
  digitalWrite(SDA, LOW); 
}

byte val = 0; 

/* Testing potentiometer to step through levels of resistance */
void loop() {
  Wire.beginTransmission(44);
  Wire.write(byte(0x00));
  Wire.write(val); 
  Wire.endTransmission();

  val++;
  if (val == 64) {
    val = 0; 
  }
  delay(500); 
}
