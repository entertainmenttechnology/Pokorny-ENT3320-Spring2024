#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <mozzi_midi.h>
#include <ADSR.h>
#include <tables/sin2048_int8.h> 


#define CONTROL_RATE 128
Oscil <2048, AUDIO_RATE> aSin(SIN2048_DATA);

float midiSequence[] = {69,36,28,52,90}; // different notes in array
float midiNote = midiSequence[4]; // chose a position of the array


void setup(){
  
  aSin.setFreq(440);
	startMozzi(CONTROL_RATE);

  Serial.begin(115200);
}

void updateControl(){
  int freq = mtof(midiNote);
  aSin.setFreq(freq);

  Serial.println(freq);
}

int updateAudio(){
	return aSin.next();

}

void loop(){
	audioHook();
}
