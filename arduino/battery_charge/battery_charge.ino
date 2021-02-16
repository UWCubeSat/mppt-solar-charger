//Andrew Buckingham
//2/16/21
//
//Performs periodic maximum power point scans and charges a single cell lithium ion battery from mppt board
//Constant current charging is done in software by this program and constant voltage is provided by the board, which limits
//the maximum output voltage to around 3.9v. 


#include <Wire.h>

//which pin the imon output of the MPPT board is connected to
#define imon A0

//the value that analogRead(imon) will return when maximum safe charging current is applied
//low values will probably cause issues in the program, current may vary anywhere from 10 below to 10 above this value to prevent excess scanning
//formula to convert to current in mA from a value here: maxCurrent / 1023 * 5 / gain / 0.05 * 1000 = current (mA)
#define maxCurrent 150

//the maximum safe potentiometer value to keep the TR/SS pin below 4 volts (because I had to remove the protection diodes on my board)
//equation: 4V = Vpanel * ( (minPotVal / 255) * 100k + 6.2k ) / (309k + 6.2k + (minPotVal / 255) * 100k), plus a bit of safety margin
//^ this is just voltage divider equation, check schematic for more information
#define minPotVal 150

//the total gain in the system for the imon voltage, the mppt board provides a gain of 20 and then any additional gain from external circuitry should be included here
//used for calculating a current value in mA to display in serial output
#define gain 20

//the interval between scans, interval roughly corresponds to time between scans in seconds
#define interval 10


int val;
int counter;

void setup() {
  pinMode(imon, INPUT);
  Serial.begin(9600);
  Wire.begin();
  val = getHighVal();
  counter = 0;
}

void loop() {
  
  Wire.beginTransmission(44);
  Wire.write(byte(0x00));
  Wire.write(val);
  Wire.endTransmission();

  delay(1000);
  
  int current = avgRead(imon);

  Serial.print("Current is ");
  //first convert the current reading to the actual voltage of the imon pin, then calculate the actual current using the gain and the value of the current monitoring resistor
  Serial.print( (double(current) / 1023.0) * 5.0 / gain / 0.05 * 1000);
  Serial.print(" mA, reading of ");
  Serial.println(current);

  //if the current exceeds the maximum current or the allotted time has passed, run another scan. 
  if((current > maxCurrent + 10) || (counter == interval)) {
    val = getHighVal();
    counter = 0;
  } else {
    counter ++;
  }
  

  
}

//performs maximum power point scan and returns the potentiometer value where maximum safe current is delivered to the battery
int getHighVal() {

  Serial.println("Scanning... ");
  
  int highCurrent = 0;
  int highVal = 255;
  int current = 0;

  for(int i = 255; i > minPotVal; i--) {
    Wire.beginTransmission(44);

    Wire.write(byte(0x00));
    Wire.write(i);
    Wire.endTransmission();

    //slows down the scan when the current starts to flow to ensure accurate measurement
    if(current > 2) {
      delay(100);
    } else  {
      delay(20);
    }

    current = avgRead(imon);
    
    if(current > highCurrent) {
      highCurrent = current;
      highVal = i;

      if(highCurrent > (maxCurrent - 10)) {
        Serial.println("Current limited to maximum safe value...");
        return highVal;
      }
    }
    
  }
  
  return highVal;
}

//performs 50 analog reads of a pin and returns the average value
int avgRead(int pin) {
  int avg = 0;

  for(int i = 0; i < 50; i ++) {
    avg += analogRead(pin);
  }

  avg = avg / 50;
  
  return avg;
}
