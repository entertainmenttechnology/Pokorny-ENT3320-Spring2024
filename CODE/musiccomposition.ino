#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <RollingAverage.h>
#include <EventDelay.h>

#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

// Define Oscillator class
class Oscillator {
  Oscil<SAW1024_NUM_CELLS, AUDIO_RATE> sawtooth; // Example oscillator type, you can change as needed
  //constructor to initialize
  public:
  Oscillator(float freq) {
    sawtooth.setFreq(freq);
  }

  void setFrequency(float freq) {
    sawtooth.setFreq(freq);
  }
  //output the Oscillator class
  float next() {
    return sawtooth.next();
  }
};

// LFO class
class LFO {
  Oscil<SAW1024_NUM_CELLS, CONTROL_RATE> sine; // Example LFO type, you can change as needed
  
  float baseFrequency;
  float modAmt; //the amount we can modify our starting LFO frequency 
  //constructors that will update our LFO
  LFO(baseFreq, modify) : baseFrequency(baseFreq), modAmt(modify) {
    sine.setFreq(baseFreq);
  }

  //this will update our frequency based on the frequency changing in our LFO
  public: 
  void updateFrequency(float modifyFreq) {
    float totalFreq = baseFreq + modifyFreq;//our total will cal the base with each updated freqiency
    sine.setFreq(totalFreq);
  }
  //output the LFO class
  float next() {
    return sine.next();
  }
};

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
  Envelope() {}

  //trigger the envvelope
  void trigger() {
    currentPoint = 0;
    targetPoint = 1;
    currentState = ATTACK;
    increment = 1 / (attackTime * CONTROL_RATE);
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
          targetLevel = sustainPoint;
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

// place sensor controls here to be updated
void updateControl() {
  float modLFO = 200;
  lfo.updateFrequency(modLFO)

  //the Oscillator class being called
  float oscOutput = osc.next();
  Oscillator osc1(baseFrequency);

  if(/* piezo trigger logic */) {
    return env.trigger();
  }
}

AudioOutput_t updateAudio() {
  // our objects
  Oscillator osc1(baseFrequency);
  LFO lfo(baseFrequency, modAmt);
  Envelope env(baseFrequency);

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
  osc1.setFrequency();
  osc2.setFrequency(baseFrequency + lfo.nect() * modAmt);

  // Generate LFO audio output
  return MonoOutput::from16Bit(osc1.next());

  // Generate Oscillator audio output
  float osc1Audio = osc1.next()
  return MonoOutput::from16bit(osc1Audio)

  // updates envelope state
  env.update();

  //receive the current point of where the envelope is at
  float enevlopePoint = env.getCurrentPoint();
}

void loop() {
  audioHook(); // fill the audio buffer, this is required
  // NOTE: Ideally, Nothing else should go in LOOP()
}