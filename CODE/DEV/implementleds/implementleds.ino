#include <MozziGuts.h>
#include <Oscil.h> 
#include <RollingAverage.h>
#include <EventDelay.h>
#include <ADSR.h>
#include <mozzi_midi.h>
#include <tables/triangle_warm8192_int8.h>
#include <tables/sin8192_int8.h>
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED7 7
#define LED8 8
#define LED9 9
#define LED10 10
#define LED11 11
Oscil <8192, AUDIO_RATE> aSaw(TRIANGLE_WARM8192_DATA);
Oscil <8192, AUDIO_RATE> aSin(SIN8192_DATA);
ADSR <AUDIO_RATE, AUDIO_RATE> envelope;
int attack = 100, 
    decay = 250,
    sustain = 25,
    release_ms = 500, 
    attack_level = 255, 
    decay_level = 255;
int mod;
int b = 0;
int musicArray[20] = {36, 40, 43, 47, 48, 52, 55, 59, 60, 64, 67, 71, 72, 76, 79, 83, 84, 88, 91, 95};
//int b = 4;
int ledArray[8] = {3, 4, 5, 6, 7, 8, 9, 10};
//int ledPattern1[8] = [HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW];
const int NUM_PINS = 15;//number of pins we're using
int piezovals[15];//values for each pin
int piezoPins[15] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14};//sensor pins
int dataArray[NUM_PINS] = {};//our raw data array
int thresHoldON = 100; //can be changed
bool triggerStatus[NUM_PINS] = {false};
RollingAverage<int, 32> kAverage;
//NUMBERS ARE LED LAYOUTS ON THE BOARD
void setup() {
  pinMode(LED3, OUTPUT);//1
  pinMode(LED4, OUTPUT);//2
  pinMode(LED5, OUTPUT);//3
  pinMode(LED6, OUTPUT);//4
  pinMode(LED7, OUTPUT);//5
  pinMode(LED8, OUTPUT);//6
  pinMode(LED9, OUTPUT);//7
  pinMode(LED10, OUTPUT);//8
  pinMode(LED11, OUTPUT);//9
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
  // Initialize dataArray with random values for each pin
 /*for (int j = 0; j < NUM_PINS; j++) {
  dataArray[j] = random(0, 1024); // range can change as needed
 } */
  
  //within our pins we will read and figure our the average in our cycle. store our data into the array
  for (int i = 0; i < NUM_PINS; i++) {
    int piezoval = mozziAnalogRead(piezoPins[i]);
    piezovals[i] = piezoval;
    int avg = kAverage.next(piezoval);
    //dataArray[i] = avg;
    //dataArray[i] = random(0, 1024);
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
      case 14: 
        piezovals[i] = mozziAnalogRead(piezoPins[i]);
        avg = kAverage.next(piezovals[i]);
        dataArray[14] = avg;
        break;
      default:
        break;
    }
  }
//the code below outputs a unique LED pattern and controls the oscillation/frequency randomly for every peg being hit.  
  //PEG1
   if (piezovals[0] > 60000000) {
      Serial.print("3D: ");
      Serial.println(piezovals[1]);
      digitalWrite(LED3, HIGH);
      Serial.println(triggerStatus[1]);
      envelope.noteOn();
      b = random(0, 20);
    }
    //peg2
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED3, HIGH);
      
      digitalWrite(LED4, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG3
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED4, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG4
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED3, HIGH);

      digitalWrite(LED5, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG5
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED5, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG5
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED5, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG6
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED6, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG7
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED4, HIGH);

      digitalWrite(LED6, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG8
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);

      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
      digitalWrite(LED7, HIGH);

      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG9
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);

      digitalWrite(LED7, HIGH);
      digitalWrite(LED8, HIGH);
      digitalWrite(LED10, HIGH);

      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG10
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);

      digitalWrite(LED7, HIGH);
      digitalWrite(LED9, HIGH);
      digitalWrite(LED11, HIGH);

      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG11
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED10, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG12
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);
      digitalWrite(LED11, HIGH);
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG13
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);

      digitalWrite(LED4, HIGH);
      digitalWrite(LED6, HIGH);
      digitalWrite(LED7, HIGH);
      digitalWrite(LED8, HIGH);
      digitalWrite(LED10, HIGH);
      
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG14
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);

      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
      digitalWrite(LED7, HIGH);
      digitalWrite(LED8, HIGH);
      digitalWrite(LED9, HIGH);
      digitalWrite(LED10, HIGH);
      digitalWrite(LED11, HIGH);
      
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG15
    if (piezovals[1] > 200) {
      Serial.print("WOOD: ");
      Serial.println(piezovals[0]);

      digitalWrite(LED3, HIGH);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED7, HIGH);
      digitalWrite(LED9, HIGH);
      digitalWrite(LED11, HIGH);
      
      Serial.println(triggerStatus[0]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    

  if (b > 20) {
    b = 0;
  }
    aSaw.setFreq(mtof(musicArray[b]));
    //Serial.println(b);
    //Serial.println(aSin.next());
    //mod = map(aSin.next(), - 127, 127, -5, 5);
   
}
AudioOutput_t updateAudio(){
  mod = map(aSin.next(), - 128, 128, -5, 5);
 //Serial.println(aSin.next()); 
  envelope.update();
  return MonoOutput::from16Bit((int) ((envelope.next() * (aSaw.next()+ mod))));
}
void loop() {
  audioHook();
 // NOTE: Ideally, Nothing else should go in LOOP()
}