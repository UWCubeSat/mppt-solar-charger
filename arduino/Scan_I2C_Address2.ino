/**
 * Returning I2C Address for 0x2c continuously
 * from AD5245, returns error codes 
 * 
 * Created by Katharine Lundblad on January 13, 2021 
 */

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner"); 
}

void loop() {
  while (!Serial.available()) {
    
  }

  byte error, address;
  int nDevices;
  int testAddress;
  Serial.println("Scanning...");
  nDevices = 0; 
  testAddress = 44; 
  Wire.beginTransmission(testAddress);
  error = Wire.endTransmission(); 
  if (error == 0) {
    Serial.print("I2C device found at address 0x");
    if (address<16) {
      Serial.print("0");
    }
    Serial.println(address,HEX);
    nDevices++;
  } else if (error == 4) {
    Serial.print("Unknow error at address 0x");
    if (address<16) {
      Serial.print("0");
    }
    Serial.println(address,HEX);
  } 
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }

  Serial.print("Error Code: ");
  Serial.print(error);
  delay(5000);
  
}
