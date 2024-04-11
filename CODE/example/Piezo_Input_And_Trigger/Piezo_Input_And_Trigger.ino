#include<MozziGuts.h>
#include<Oscil.h>
#include<tables/sin2048_int8.h>
#include<RollingAverage.h>
#include<mozzi_midi.h>

//code for each piezo input and trigger
//there might be spelling mistakes

//arrays
int PiezoCount[6]={A1, A2, A3, A4, A5, A6};
int PiezoAve[6]={1, 2, 3, 4, 5, 6};
int MidiNote[6]={1, 2, 3, 4, 5, 6};

//trigger limit to set off sound
int trigger=3;

RollingAverage<int, 32> kAverage;

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin1(SIN2048_DATA);

void setup() {
  // put your setup code here, to run once:
  startMozzi();
  Serial.begin(115200);
}

void updateControl(){
  //setup analog read
  PiezoCount[1] = mozziAnalogRead(A1);
  PiezoCount[2] = mozziAnalogRead(A2);
  PiezoCount[3] = mozziAnalogRead(A3);
  PiezoCount[4] = mozziAnalogRead(A4);
  PiezoCount[5] = mozziAnalogRead(A5);
  PiezoCount[6] = mozziAnalogRead(A6);
  //get average for all the piezo input
  for (int i=1;i<=6;i+=1){
    PiezoAve[i] = kAverage.next(PiezoCount[i]);
  }
  //trun piezo into midi note
  for (int i=1;i<=6;i+=1){
    MidiNote[i] = mtof(PiezoAve[i]);
  }
  //set frequency to midi note
  for (int i=1;i<=6;i+=1){
    aSin1.setFreq(MidiNote[i]);
  }
}

int updateAudio(){
  //for each piezo, if limit pass, play sound
  for (int i=1;i<=6;i+=1){
    if (PiezoCount[i]<=trigger){
      return MonoOutput::from6Bit(aSin.next());
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  audioHook();
}
