#include <MozziGuts.h>
#include <Oscil.h> 
#include <RollingAverage.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <tables/triangle_warm8192_int8.h>
#include <tables/sin8192_int8.h>

Oscil <8192, AUDIO_RATE> aSaw(TRIANGLE_WARM8192_DATA);
Oscil <8192, AUDIO_RATE> aSin(SIN8192_DATA);

ADSR <AUDIO_RATE, AUDIO_RATE> envelope;

int attack = 1, 
    decay = 100,
    sustain = 100,
    release_ms = 500, 
    attack_level = 255, 
    decay_level = 255;

int b = 0;
int musicArray[20] = {36, 40, 43, 47, 48, 52, 55, 59, 60, 64, 67, 71, 72, 76, 79, 83, 84, 88, 91, 95};
//int b = 4;

const int NUM_PINS = 14;//number of pins we're using
int piezovals[14];//values for each pin
int piezoPins[14] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13};//sensor pins
int dataArray[NUM_PINS] = {};//our raw data array
int thresHoldON = 100; //can be changed
bool triggerStatus[NUM_PINS] = {false};
RollingAverage<int, 32> kAverage;


void setup() {
  //int musicArray[b] = {36, 40, 43, 47};
  //analogReference(INTERNAL1V1);
  Serial.begin(115200);
  aSaw.setFreq(1000);
  aSin.setFreq(5);
  startMozzi();
  envelope.noteOn();


}

void updateControl() {
  
  envelope.setADLevels(attack_level, decay_level);
  envelope.setTimes(attack, decay, sustain, release_ms);
  
  //within our pins we will read and figure our the average in our cycle. store our data into the array
  for (int i = 0; i < NUM_PINS; i++) {
    int piezoval = mozziAnalogRead(piezoPins[i]);
    piezovals[i] = piezoval;
    int avg = kAverage.next(piezoval);
    dataArray[i] = avg;

  //threshold logic where once the values are increasing and hitting a "peak" we ignore anything above it.
  if (piezovals > thresHoldON) {
    triggerStatus[i] = true;
  }
  else {
    triggerStatus[i] = false;
  }

    //switch case implements the rolling average within each pin
    switch(i) {
      case 0: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[0] = avg;
        break;
      case 1: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[1] = avg;
        break;
      case 2: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[2] = avg;
        break;
      case 3: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[3] = avg;
        break;
      case 4: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[4] = avg;
        break;
      case 5: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[5] = avg;
        break;
      case 6: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[6] = avg;
        break;
      case 7: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[7] = avg;
        break;
      case 8: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[8] = avg;
        break;
      case 9: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[9] = avg;
        break;
      case 10: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[10] = avg;
        break;
      case 11: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[11] = avg;
        break;
      case 12: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[12] = avg;
        break;
      case 13: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[13] = avg;
        break;
      default:
        break;
    }
  }
  
   if (piezovals[0] > 90) {
      Serial.print("3D: ");
      Serial.println(piezovals[1]);
      Serial.println(triggerStatus[1]);
      envelope.noteOn();
      b++;
    }

     if (piezovals[1] > 90) {
    Serial.print("WOOD: ");
    Serial.println(piezovals[0]);
    Serial.println(triggerStatus[0]); 
    envelope.noteOn();
    b++;
    
    }

  if (b > 20) {

    b = 0;
  }

    aSaw.setFreq(mtof(musicArray[b]));
    Serial.println(b); 
   

}

AudioOutput_t updateAudio(){
 
  envelope.update();
  return MonoOutput::from16Bit((int) (envelope.next() * aSaw.next()));
}

void loop() {
  audioHook();
 // NOTE: Ideally, Nothing else should go in LOOP()
}