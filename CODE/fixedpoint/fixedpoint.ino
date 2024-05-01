#include <MozziGuts.h> // the Mozzi library
#include <FixMath.h>   // the FixMath library

void setup() {
  Serial.begin(9600); // serial comms
  
  long inputNumber = 12345678;// an example number. can be changed later on

  // converts into fixed-point formats using the mozzi library
  fix16_t q0n8 = float_to_fix16(inputNumber / 256.0);//dividing by 256 (values from 8 bit) will move the decimal 8 places to the right.
  fix16_t q16n0 = float_to_fix16(inputNumber / 65536.0);//dividing by 65536 (values from 8 bit) will move the decimal 16 places to the right.
  

  // Print results to Serial Monitor
  Serial.println("Original Number: " + String(inputNumber));
  Serial.println("Q0n8: " + String(q0n8);
  Serial.println("Q16n0: " + String(q16n0);
}

void loop() {
  
}
