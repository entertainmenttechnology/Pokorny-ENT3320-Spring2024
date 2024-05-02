
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/triangle2048_int8.h>
//const int buttonPin = 2;

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> cTri(TRIANGLE2048_DATA);

//int buttonState = LOW;             // tracks current reading of button pin
int buttonState;
int buttonFlag;
int lastbuttonState;
// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable


void setup(){
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  startMozzi(CONTROL_RATE); // :)
  cTri.setFreq(460);
}


void updateControl(){
  lastbuttonState = buttonState;
  buttonState = digitalRead(2);

   Serial.println(buttonState);

  if (lastbuttonState == HIGH && buttonState == LOW ) {
    buttonFlag = !buttonFlag;
  } 
  
}


AudioOutput_t updateAudio(){
  if (buttonFlag == true) {
    return MonoOutput::from8Bit(cTri.next()); 
} else {

}

}

void loop(){
  audioHook(); // required here
}