
/**************************************************************************/
/*!
Temperature Sensor
*/
/**************************************************************************/

#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

int TempSensor = A4;               // Set analog pin 4 for receiving analog temperature data
int GreenLED = 3;                  // Set digital pin 3 as green LED output
int RedLED = 4;                    // Set digital pin 3 as green LED output
boolean GreenLEDStatus = true;     // Set green LED to on
boolean RedLEDStatus = false;      // Set red LED to off
int TempThreshold = 23;            // Set temperature threshold in degrees C
int SampleSpeed = 500;             // Set sample speed (in milliseconds, 500 ms = 0.5 s)

void setup() {
  pinMode(TempSensor,INPUT);        // Set temperature sensor as an input
  pinMode(GreenLED,OUTPUT);         // Set green LED as an output
  pinMode(RedLED,OUTPUT);           // Set red LED as an output
  Serial.begin(9600);               // 
  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.
  
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
  // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
  // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }

  tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms

  tempsensor.wake();   // wake up, ready to read!

}
void loop() {

  // Read and print out the temperature
  float Temperature = tempsensor.readTempC();
  int TempSensorData=analogRead(TempSensor);

  // Switch green LED off and red LED on if temperature exceeds threshold

  if(Temperature>TempThreshold){

    GreenLEDStatus=false;
    RedLEDStatus=true;
    digitalWrite(GreenLED,LOW);
    digitalWrite(RedLED,HIGH);
    }

  // Switch red LED off and green LED on if temperature drops below threshold

  else{
    RedLEDStatus=false;
    GreenLEDStatus=true;
    digitalWrite(GreenLED,HIGH);
    digitalWrite(RedLED,LOW);
    }
  
  // Print temperature to serial monitor

  Serial.print(Temperature);
  Serial.println(" , "); 
  
  // Pause (for set sample speed time)

  delay(SampleSpeed);
  digitalWrite(RedLED,LOW);
}
