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

// Define Oscillator class
class Oscillator {
    Oscil<SAW1024_NUM_CELLS, AUDIO_RATE> aSaw;
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

// Define LFO class
class LFO {
private:
    Oscil<SAW1024_NUM_CELLS, CONTROL_RATE> sine;
    float baseFrequency;
    float modAmt; //the amount we can modify our starting LFO frequency 

public:
     //constructors that will update our LFO
    LFO(float baseFreq, float modify) : baseFrequency(baseFreq), modAmt(modify) {
        sine.setFreq(baseFreq);
    }
    //this will update our frequency based on the frequency changing in our LFO
    void updateFrequency(float modifyFreq) {
        float totalFreq = baseFrequency + modifyFreq; //our total will cal the base with each updated freqiency
        sine.setFreq(totalFreq);
    }
    //output the LFO class
    float next() {
        return sine.next();
    }
};

// Define Envelope class
class Envelope {
private:

    float attackTime;
    float decayTime;
    float sustainPoint; //after the decay this will control what the volume levels  will be
    float releaseState = 1.5; //the time to release the sound
    enum State { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE };
    float currentPoint; 
    float targetPoint; 
    float counter; // value for the attack to be release
    State currentState; // state of the envelope

public:
    Envelope(float attTime, float decTime, float susPoint) : attackTime(attTime), decayTime(decTime), sustainPoint(susPoint) {}

    void trigger() {
        currentPoint = 0;
        targetPoint = 1;
        currentState = ATTACK;
        counter = 1 / (attackTime * CONTROL_RATE);
    }

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

// Global objects
Oscillator osc1(440); // Initial frequency is 440 Hz
LFO lfo(10, 200);     // Base frequency 10 Hz, modulation amplitude 200
Envelope env(0.1, 0.2, 0.5); // Attack: 0.1 sec, Decay: 0.2 sec, Sustain level: 0.5

// Update sensor controls
void updateControl() {
    float modLFO = 200;
    lfo.updateFrequency(modLFO);

     //the Oscillator class being called
    float oscOutput = osc.next();
    Oscillator osc1(baseFrequency);

    if (mozziAnalogRead(piezoPin)) {
        env.trigger();
    }  
    else {
        env.release(); // Release the envelope when the sensor control is not triggered
    }
}

// Update audio
AudioOutput_t updateAudio() {

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

    updateContril();

    float modulatedFrequency = osc1.next() + lfo.next() * lfo.modAmt; // Apply LFO modulation directly to oscillator frequency
    osc1.setFrequency(modulatedFrequency);
    
    // updates envelope state
    env.update();
    float envelopePoint = env.getCurrentPoint();
    // Generate Oscillator audio output
    return MonoOutput::from16Bit(osc1.next() * envelopePoint); // Apply envelope to oscillator output
}

void loop() {
    audioHook(); // Fill the audio buffer, this is required
    // NOTE: Ideally, Nothing else should go in LOOP()
}