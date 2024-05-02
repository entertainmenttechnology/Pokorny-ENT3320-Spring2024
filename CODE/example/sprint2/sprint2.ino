#include<Arduino.h>
#include<MozziGuts.h>
#include<Oscil.h>
#include<tables/sin2048_int8.h>
#include<RollingAverage.h>
#include<mozzi_midi.h>
#include<ADSR.h>


//arrays
int PiezoCount[6]={A1, A2, A3, A4, A5, A0};
int PiezoAve[6]={1, 2, 3, 4, 5, 6};
int MidiNote[6]={1, 2, 3, 4, 5, 6};

//tigger limit (change)
int trigger = 1;

//get average for each PiezoCount
RollingAverage<int, 8> pAverage1;
RollingAverage<int, 12> pAverage2;
RollingAverage<int, 8> pAverage3;
RollingAverage<int, 12> pAverage4;
RollingAverage<int, 8> pAverage5;
RollingAverage<int, 12> pAverage6;

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin1(SIN2048_DATA);
ADSR <AUDIO_RATE, AUDIO_RATE> envelope;

//creating enevelope values
int attack = 1;
int decay = 100;
int sustain = 100;
int release = 1000;
int AttackLevel = 225;
int DecayLevel = 225;

void setup() {
  // put your setup code here, to run once:
  startMozzi();
  Serial.begin(115200);
  NoteDelay.set(1000);
  //set up analog Read for mozzi
  PiezoCount[1] = mozziAnalogRead(A1);
  PiezoCount[2] = mozziAnalogRead(A2);
  PiezoCount[3] = mozziAnalogRead(A3);
  PiezoCount[4] = mozziAnalogRead(A4);
  PiezoCount[5] = mozziAnalogRead(A5);
  PiezoCount[6] = mozziAnalogRead(A0);

}

void updateControl(){
  //get average for all the piezo input
  //for (int i=1;i<=6;i+=1){
    //PiezoAve[i] = kAverage.next(PiezoCount[i]);
  //}
  PiezoAve[1] = pAverage1.next(PiezoCount[1]);
  PiezoAve[2] = pAverage2.next(PiezoCount[2]);
  PiezoAve[3] = pAverage3.next(PiezoCount[3]);
  PiezoAve[4] = pAverage4.next(PiezoCount[4]);
  PiezoAve[5] = pAverage5.next(PiezoCount[5]);
  PiezoAve[6] = pAverage6.next(PiezoCount[6]);

  //turn piezo into midi note
  for (int i=1; i<=6; i+=1){
    MidiNote[i]= mtof(PiezoAve[i]);
  }

  //set frequency to midi note
  for (int i =1; i<=6; i+=1){
    aSin1.setFreq(MidiNote[i]);
  }

  //create enevlope
  envelope.setADLevels(AttackLevel, DecayLevel);
  envelope.setTimes(attack, decay, sustain, release);

  if (PiezoCount[i]>trigger){
    envelope.noteOn();
    i++;
  }

  //testing area
  //Serial.println (PiezoCount[1]);
  //Serial.println (PiezoAve[1]);
  //Serial.println (MidiNote[1]);

}

int updateAudio(){
  //for each piezo, if limit pass, play sound
  //for (int i = 0; i<=6; i+=1){
    //if (PiezoCount[i]< trigger ){
      //Serial.println (MidiNote[1]);
      //return MonoOutput::from8Bit((aSin1.next()));
    //}
  //}
  envelope.update();
  return MonoOutput::from16Bit((int)(envelope.next()* aSin1.next()));
}
void loop() {
  // put your main code here, to run repeatedly:
  audioHook();
}
