#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <RollingAverage.h>
#include <EventDelay.h>
#include <tables/saw1024_int8.h>
#include "Arduino.h"
#ifndef Oscillator_h
#define Oscillator_h
#ifndef LFO_h
#define LFO_h
#ifndef Envelope_h
#define Envelope_h

#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

float piezoPin = A0;

Oscil<SAW1024_NUM_CELLS, AUDIO_RATE> aSaw; // Example oscillator type, you can change as needed

// Define Oscillator class
class Oscillator {
 Oscil<SAW1024_NUM_CELLS, AUDIO_RATE> aSaw; // Example oscillator type, you can change as needed
 //constructor to initialize
 public:
 Oscillator(float freq) {
   aSaw.setFreq(freq);
 }

 void setFrequency(float freq) {
   aSaw.setFreq(freq);
 }
 //output the Oscillator class
 float next() {
   return aSaw.next();
 }
};
#endif
// LFO class
class LFO {
 private:
    Oscil<SAW1024_NUM_CELLS, CONTROL_RATE> sine; // Example LFO type, you can change as needed
    float baseFrequency;
    float modAmt;

public:
    // Factory function to create LFO instances
    static LFO create(float baseFreq, float modify) {
        return LFO(baseFreq, modify);
    }

    void updateFrequency(float modifyFreq) {
        float totalFreq = baseFrequency + modifyFreq;
        sine.setFreq(totalFreq);
    }

    float next() {
        return sine.next();
    }

private:
    // Private constructor
    LFO(float baseFreq, float modify) : baseFrequency(baseFreq), modAmt(modify), sine() {
        sine.setFreq(baseFreq);
    }
};
#endif
// Envelope class
class Envelope {
 // the parameters for the envelope
 float attackTime; 
 float decayTime;  
 float sustainPoint;//after the decay this will control what the volume levels  will be
 float releaseState = 1.5; //the time to release the sound

 enum State { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE }; // Enum to define envelope states
 float currentPoint;
 float targetPoint;
 float counter; // value for the attack to be release
 State currentState; // state of the envelope

 //constructor of envelope
 public:
  Envelope(float attack, float decay, float sustain, float release) :
    attackTime(attack), decayTime(decay), sustainPoint(sustain), releaseState(release),
    counter(0), targetPoint(0), currentState(IDLE), currentPoint(0) {}

 //trigger the envvelope
 void trigger() {
   currentPoint = 0;
   targetPoint = 1;
   currentState = ATTACK;
   counter = 1 / (attackTime * CONTROL_RATE);
 }
 //releasing the envelope 
 void release() {
   currentState = RELEASE;
   targetPoint = 0;
   counter = 1 / (releaseState * CONTROL_RATE);
 }

/*upadte the state with a each state attack, decay, & release
the curremt state in decay & release cases will euqal to state deayTime/releaseTime
while attack case will be substaint time and point when incrementing. update class. 
*/
 void update() {
   switch (currentState) {
     case ATTACK:
       currentPoint += counter;
       if (currentPoint >= targetPoint) {
         currentState = DECAY;
         targetPoint = sustainPoint;
         counter = (sustainPoint - currentPoint) / (decayTime * CONTROL_RATE);
       }
       break;

     case DECAY:
       currentPoint += counter;
       if (currentPoint <= targetPoint) {
         currentState = SUSTAIN;
       }
       break;

     case RELEASE:
       currentPoint -= counter;
       if (currentPoint <= targetPoint) {
         currentState = IDLE;
       }
       break;

     default:
       break;
   }
 }

 float getCurrentPoint() {
   return currentPoint;
 }
};
#endif

void setup() {
  startMozzi(CONTROL_RATE);
}

LFO lfo = LFO::create(440, 200); //(baseFreq, modulation sound)  
// place sensor controls here to be updated
void updateControl() {
 float modLFO = 200;
 float baseFreq = 100; 
 lfo.updateFrequency(modLFO);

 //the Oscillator class being called
  Oscillator osc1(440); // Declare osc locally within updateControl
  float oscOutput = osc1.next();

  Envelope env(0.1, 0.2, 0.5, 1.0);
  if(piezoPin = mozziAnalogRead(A0)) {
    return env.trigger();
  }
}

float baseFrequency = 440.0; // Example base frequency
float modAmt = 200.0;
AudioOutput_t updateAudio() {
 // our objects
 Oscillator osc1(baseFrequency);
 LFO lfo = LFO::create(baseFrequency, modAmt);
 Envelope env(0.1, 0.2, 0.5, 1.0);

   /* For mono output, the return value of this function is really just a signed integer
 However, for best portability of your sketch to different boards and configurations,
 pick one of the variants below, depending on the "natural" range of the audio values
 you generate:
 */

 //PICK ONLY ONE:
 return MonoOutput::from8Bit(0);  // if your signal is in 8 bit range
 /* OR */
 return MonoOutput::fromAlmostNBit(9, 0);  // if your signal is between -244 and 243 (_almost_ 9 bits is a special case on AVR boards)
 /* OR */
 return MonoOutput::fromAlmostNBit(9, 0).clip();  // To clip (instead of overflow), should a few stray values exceed the allowable range
 /* OR */
 return MonoOutput::from16Bit(0);  // if your signal is in 16 bit range, e.g. the product of two 8 bit numbers
 /* OR */
 return MonoOutput::fromNBit(21, 0);  // if your signal happens to be in 21 bit range

 // Update oscillator frequencies based on LFO

 float modulatedFrequency = baseFrequency + lfo.next() * modAmt;
  osc1.setFrequency(modulatedFrequency);

  // Generate Oscillator audio output
  float oscillatorOutput = osc1.next();

  // Update envelope state
  env.update();

  // Apply envelope modulation
  float envelopeLevel = env.getCurrentPoint();
  float audioOutput = oscillatorOutput * envelopeLevel;

  // Return the audio output
  return MonoOutput::from16Bit(audioOutput);
}

void loop() {
 audioHook(); // fill the audio buffer, this is required
 // NOTE: Ideally, Nothing else should go in LOOP()
}