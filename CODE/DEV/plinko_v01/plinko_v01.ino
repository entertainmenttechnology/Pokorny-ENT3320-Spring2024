/*
PLINKO-MOZZI Template for CITYTECH ENT3320-SP24

Board: Arduino Mega

ADC: 16x - Piezo [A0-A15] (run thru our haf-wave rect / voltage divider circuit)

AUDIO Circuit: [Tip = Digital Pin 09, Sleeve = GND Pin]

Mozzi documentation/API:
https://sensorium.github.io/Mozzi/doc/html/index.html

*/
#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <RollingAverage.h>
#include <EventDelay.h>

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable
/*
The CONTROL_RATE has a default value of 64, but you can change it if you want control updates to happen more frequently. Like the audio rate, it must be a literal number and power of two to allow Mozzi to optimise internal calculations for run-time speed.
*/

void setup() 
{
  startMozzi(CONTROL_RATE);
}

void updateControl() 
{
  //put changing controls (ie. sensor updates) here
}

AudioOutput_t updateAudio() 
{
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

}

void loop() 
{
  audioHook(); // fill the audio buffer, this is required
  //NOTE: Ideally, Nothing else should go in LOOP()
}