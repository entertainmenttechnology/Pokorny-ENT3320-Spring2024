#include <MozziGuts.h>

const int numReadings = 10;

const int sensor = A0; 
const int POT = A1;
const int LED = 13;
//const int LED2 = 10;
//const int LED3 = 9; 
//const int LED4= 6; 
int value = 0;
int POTvalue = 0; 

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average

void setup() {
pinMode(LED, OUTPUT); 
//pinMode(LED2, OUTPUT);
//pinMode(LED3, OUTPUT);
//pinMode(LED4, OUTPUT); 

//initialize serial communication at 9600 bits per second
Serial.begin(9600); //9600 bits per second 
for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
value = analogRead(sensor);
POTvalue = analogRead(POT);
delay(10);
// LED wants 0-255, 0-1023 are the values from the sensor 
value = value; //dividing value by 4 reduce range to max 255
POTvalue = POTvalue;
analogWrite(LED, value); 
//analogWrite(LED2, value);
//analogWrite(LED3, POTvalue);
//analogWrite(LED4, POTvalue);

// subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(POT);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;

// calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(average);
  delay(1);  // delay in between reads for stability

//Serial.println(value);
Serial.println(POTvalue);
  }
}

