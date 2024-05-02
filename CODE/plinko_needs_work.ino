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
#define LED11 12

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

int ledArray[9] = {3, 4, 5, 6, 7, 8, 9, 10, 11};
//int ledPattern1[8] = [HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW];
const int NUM_PINS = 15;//number of pins we're using
int piezovals[15];//values for each pin
int piezoPins[15] = {A0, A1, A2, A3, A4, A15, A6, A7, A8, A9, A10, A11, A12, A13, A14};//sensor pins
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
  //PEG 1
   if (piezovals[0] > 50) {
      Serial.print("PEG 1: ");
      Serial.println(piezovals[0]);

      Serial.println(triggerStatus[0]);
      envelope.noteOn();
      b = random(0, 20);
      Serial.println(b);

      digitalWrite(LED3, HIGH); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9
    }
    //PEG 2
     if (piezovals[1] > 50) {
      Serial.print("PEG 2: ");
      Serial.println(piezovals[1]);

      digitalWrite(LED3, HIGH); //1

      digitalWrite(LED4, HIGH); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9
    

      Serial.println(triggerStatus[1]); 
      envelope.noteOn();
      b = random(0, 20);
    } 
    //PEG 3
    if (piezovals[2] > 50) {
      Serial.print("PEG 3: ");
      Serial.println(piezovals[2]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, HIGH); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[2]);
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG 4
     /*if (piezovals[3] > 50) {
      Serial.print("PEG 4: ");
      Serial.println(piezovals[3]);

      digitalWrite(LED3, HIGH); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, HIGH); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[3]); 
      envelope.noteOn();
      b = random(0, 20);
    }*/
    //PEG 5
    /*if (piezovals[4] > 50) 
    {
      Serial.print("PEG 5:" );
      Serial.println(piezovals[4]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, HIGH); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[4]);
      envelope.noteOn();
      b = random(0, 20);
    }*/
    //PEG 6
     /*if (piezovals[5] > 100) {
      Serial.print("PEG 6: ");
      Serial.println(piezovals[5]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, HIGH); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[5]); 
      envelope.noteOn();
      b = random(0, 20);
    }*/
    //PEG 7
    /*if (piezovals[6] > 50) {
      Serial.print("PEG 7: ");
      Serial.println(piezovals[6]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, HIGH); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, HIGH); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[6]);
      envelope.noteOn();
      b = random(0, 20);
    } */
    //PEG 8
     /*if (piezovals[7] > 50) {
      Serial.print("PEG 8: ");
      Serial.println(piezovals[7]);

      digitalWrite(LED3, HIGH); //1

      digitalWrite(LED4, HIGH); //2

      digitalWrite(LED5, HIGH); //3

      digitalWrite(LED6, HIGH); //4

      digitalWrite(LED7, HIGH); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[7]); 
      envelope.noteOn();
      b = random(0, 20);
    }*/
    //PEG 9
    if (piezovals[8] > 10) {
      Serial.print("PEG 9: ");
      Serial.println(piezovals[8]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, HIGH); //5

      digitalWrite(LED8, HIGH); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, HIGH); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(mozziAnalogRead(A8));
      envelope.noteOn();
      b = random(0, 20);
    } 
    //PEG 10
     if (piezovals[9] > 10) {
      Serial.print("PEG 10: ");
      Serial.println(piezovals[9]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, HIGH); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, HIGH); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, HIGH); //9

      Serial.println(triggerStatus[9]); 
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG 11
   if (piezovals[10] > 50) {
      Serial.print("PEG 11: ");
      Serial.println(piezovals[10]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, HIGH); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[10]);
      envelope.noteOn();
      b = random(0, 20);
    } 
    //PEG 12
     if (piezovals[11] > 50) {
      Serial.print("PEG 12: ");
      Serial.println(piezovals[11]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, LOW); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, HIGH); //9

      Serial.println(triggerStatus[11]); 
      envelope.noteOn();
      b = random(0, 20);
    }
      //PEG 13
    if (piezovals[12] > 50) {
      Serial.print("PEG 13: ");
      Serial.println(piezovals[12]);

      digitalWrite(LED3, LOW); //1

      digitalWrite(LED4, HIGH); //2

      digitalWrite(LED5, LOW); //3

      digitalWrite(LED6, HIGH); //4

      digitalWrite(LED7, HIGH); //5

      digitalWrite(LED8, HIGH); //6

      digitalWrite(LED9, LOW); //7

      digitalWrite(LED10, HIGH); //8

      digitalWrite(LED11, LOW); //9

      Serial.println(triggerStatus[12]);
      envelope.noteOn();
      b = random(0, 20);
    }
    //PEG 14
    if (piezovals[13] > 50) {
      Serial.print("PEG 14: ");
      Serial.println(piezovals[13]);

      digitalWrite(LED3, HIGH); //1

      digitalWrite(LED4, HIGH); //2

      digitalWrite(LED5, HIGH); //3

      digitalWrite(LED6, HIGH); //4

      digitalWrite(LED7, HIGH); //5

      digitalWrite(LED8, HIGH); //6

      digitalWrite(LED9, HIGH); //7

      digitalWrite(LED10, HIGH); //8

      digitalWrite(LED11, HIGH); //9

      Serial.println(triggerStatus[13]); 
      envelope.noteOn();
      b = random(0, 20);
    }
      //PEG 15
      if (piezovals[14] > 50) {
      Serial.print("PEG 15: ");
      Serial.println(piezovals[14]);

      digitalWrite(LED3, HIGH); //1

      digitalWrite(LED4, LOW); //2

      digitalWrite(LED5, HIGH); //3

      digitalWrite(LED6, LOW); //4

      digitalWrite(LED7, HIGH); //5

      digitalWrite(LED8, LOW); //6

      digitalWrite(LED9, HIGH); //7

      digitalWrite(LED10, LOW); //8

      digitalWrite(LED11, HIGH); //9

      Serial.println(triggerStatus[14]); 
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
  //mod = map(aSin.next(), - 128, 128, -5, 5);
 //Serial.println(aSin.next()); 
  envelope.update();
  return MonoOutput::from16Bit((int) ((envelope.next() * (aSaw.next()))));
}

void loop() {
  audioHook();
 // NOTE: Ideally, Nothing else should go in LOOP()
}