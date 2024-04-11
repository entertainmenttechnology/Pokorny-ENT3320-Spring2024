#include <Arduino.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <tables/saw8192_int8.h>
#include <tables/sin8192_int8.h>

Oscil <8192, AUDIO_RATE> aSaw(SAW8192_DATA);
Oscil <8192, AUDIO_RATE> aSin(SIN8192_DATA);

ADSR <AUDIO_RATE, AUDIO_RATE> envelope;

int attack = 1, 
    decay = 100,
    sustain = 100,
    release_ms = 1000, 
    attack_level = 255, 
    decay_level = 255;

int i = 12;

void setup() 
{
  Serial.begin(115200);
  pinMode(8, INPUT_PULLUP);
  aSaw.setFreq(1000);
  aSin.setFreq(440);
  startMozzi();
  envelope.noteOn();
}

void updateControl()
{
    envelope.setADLevels(attack_level, decay_level);
    envelope.setTimes(attack, decay, sustain, release_ms);

  if (!digitalRead(8))
  {
    envelope.noteOn();
    i++;
  }

  aSin.setFreq(mtof(i));
  aSaw.setFreq(mtof(i));

}

AudioOutput_t updateAudio()
{
  envelope.update();
  return MonoOutput::from16Bit((int) (envelope.next() * aSaw.next()));
}

void loop()
{
  audioHook();
}







