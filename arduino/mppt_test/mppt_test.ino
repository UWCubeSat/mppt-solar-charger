//Andrew Buckingham
//1/26/21
//scans the IMON output over the entire range of the AD5245 and sets the AD5245 to the value where current is highest
#include <Wire.h>

#define imon A0

//potentiometer value
byte val;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(imon, INPUT);

  //sets the potentiometer value to attain maximum current through output
  byte val = scan();
  Serial.print("Maximum current occurs at value: ");
  Serial.print(val);
  Serial.println(" out of 255");
}

//after scanning and finding the maximum value, continually write that value to the potentiometer
void loop() {
  Wire.beginTransmission(44); //address of device is 44

  Wire.write(byte(0x00)); //instruction byte for writing
  Wire.write(val); //value from 0 to 255 to corresponding to 50k to 0 resistance (backwards)
  Wire.endTransmission();

  delay(200);

  //Prints the current imon value every cycle
  Serial.println(analogRead(imon));
}

//steps through every potentiometer value and tracks the current throughout
//returns the potentiometer value where the maximum current occurs
int scan() {
  int maxCurrent = 0;
  int maxVal = 0;
  int current = 0;

  //i is the potentiometer value being tested
  for(int i = 0; i < 255; i ++) {
    Wire.beginTransmission(44); 

    Wire.write(byte(0x00)); 
    Wire.write(i); 
    Wire.endTransmission();  
  
    delay(20);
  
    current = analogRead(imon);
    if(current > maxCurrent) {
      maxCurrent = current;
      maxVal = i;
    }
  }
  return maxVal;
}
