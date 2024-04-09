/*  Example playing a sinewave at a set frequency,
    using Mozzi sonification library.

    Demonstrates the use of Oscil to play a wavetable.

    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or
    check the README or http://sensorium.github.io/Mozzi/

    Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2012, CC by-nc-sa.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
//const int buttonPin = 2;

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

//int buttonState = LOW;             // tracks current reading of button pin
int buttonState;
int buttonFlag;
int lastbuttonState;
int i = 0;

int randFreqs[20] = {65, 82, 97, 123, 130, 164, 196, 246, 261, 329, 392, 493, 523, 659, 783, 987, 1046, 1318, 1568, 1975};
// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable


void setup(){
  
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  startMozzi(CONTROL_RATE); // :)
  aSin.setFreq(randFreqs[0]); // set the frequency

 
}


void updateControl(){
  lastbuttonState = buttonState;
  buttonState = digitalRead(2);

   Serial.println(buttonState);

  if (lastbuttonState == HIGH && buttonState == LOW ) 
  { 
    buttonFlag = !buttonFlag;
  } 
  if (buttonFlag)
  {
       i++;

    if (i > 20)
    {
      i = 0;
    }
  }
  
}


AudioOutput_t updateAudio(){
  if (buttonFlag == true) {
    return MonoOutput::from8Bit(aSin.next()); // return an int signal centred around 0
} else {
  
}

}

void loop(){
  audioHook(); // required here
}
