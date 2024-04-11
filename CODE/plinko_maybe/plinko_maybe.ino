#include <Arduino.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <tables/triangle_warm8192_int8.h>
//#include <tables/sin8192_int8.h>

Oscil <8192, AUDIO_RATE> aTri(TRIANGLE_WARM8192_DATA);
//Oscil <8192, AUDIO_RATE> aSin(SIN8192_DATA);

ADSR <AUDIO_RATE, AUDIO_RATE> envelope;

int attack = 25, 
    decay = 100,
    sustain = 100,
    release_ms = 500, 
    attack_level = 255, 
    decay_level = 255;

int i = 36;

bool isPlaying = false; 

void setup() 
{
  Serial.begin(115200);
  pinMode(8, INPUT_PULLUP);
  aTri.setFreq(1000);
  //aSin.setFreq(440);
  envelope.setADLevels(attack_level, decay_level);
  envelope.setTimes(attack, decay, sustain, release_ms);
  envelope.setIdleLevel(0);
  startMozzi();
}

void updateControl()
{
    envelope.setADLevels(attack_level, decay_level);
    envelope.setTimes(attack, decay, sustain, release_ms);
    

  if (!digitalRead(8))
  {
    isPlaying = !isPlaying;
    envelope.noteOn();
    i++;
  } else {
    isPlaying = isPlaying;
  }

  

  if(i >= 127){
    i = 1;
  }

  aTri.setFreq(mtof(i)); //converts MIDI note # to frequency
  //Serial.println((int)(envelope.next() * aTri.next()));

 // Serial.println((int)(envelope.next()));
}

AudioOutput_t updateAudio()
{
  envelope.update();
  //return MonoOutput::from16Bit((int)(envelope.next() * aTri.next()));
  return MonoOutput::from16Bit(aTri.next());
}

void loop()
{
  audioHook();
}







