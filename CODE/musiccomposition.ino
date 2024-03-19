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
  void setFrequency(float freq) {
    sawtooth.setFreq(freq);
  }
  float next() {
    return sawtooth.next();
  }
};

// Define LFO class
class LFO {
  Oscil<SIN1024_NUM_CELLS, CONTROL_RATE> sine; // Example LFO type, you can change as needed
  
  float baseFrequency;
  float modAmount; //the amount we can modify our starting LFO frequency 
  //constructors that will update our LFO
  LFO(baseFreq, modify) : baseFrequency(baseFreq), modAmount(modify) {
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

// Define Envelope class
class Envelope {
  ADSR adsr;

  void trigger() {
    adsr.noteOn();
  }
  void update() {
    adsr.update();
  }
};

void setup() {
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  // Put changing controls (i.e., sensor updates) here
}

AudioOutput_t updateAudio() {
  // our objects
  Oscillator osc1;
  Oscillator osc2;
  LFO lfo;
  Envelope env;

  // Read input from piezo sensor or other triggers
  // Determine when to trigger the envelope and generate MIDI notes
  // Example:
  if (/* condition */) {
    // Trigger envelope
    env.trigger();
    // Generate MIDI note
    // MIDI.sendNoteOn(/* Note value */, /* Velocity */, /* Channel */);
  }

  // Update oscillator frequencies based on LFO
  osc1.setFrequency();
  osc2.setFrequency(baseFrequency + lfo.nect() * modAmount);

  // Update envelope state
  env.update();

  // Generate audio output
  return MonoOutput::from16Bit(osc1.next() * osc2.next());
}

void loop() {
  audioHook(); // fill the audio buffer, this is required
  // NOTE: Ideally, Nothing else should go in LOOP()
}