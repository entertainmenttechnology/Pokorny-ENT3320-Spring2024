#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/sin2048_int8.h> 
#include <RollingAverage.h>

int potval0;
int potpin = A3;
int ave;
RollingAverage<int, 32> kAverage;

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin0(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin1(SIN2048_DATA);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  startMozzi();
}

void updateControl(){
  potval0 = mozziAnalogRead(potpin0);
  ave = kAverage.next(potval0);

  aSin0.setFreq(potval0);
  Serial.print("potval \t");
  Serial.print(potval0);

  aSin1.setFreq(ave);
  Serial.print("ave \t");
  Serial.print(ave);

  Serial.print("return \t");
  Serial.print("potval \t");
}

int updateAudio(){
  return MonoOutput::from16Bit((aSin1.next()+aSin0.next())/2);
}

void loop() {
  // put your main code here, to run repeatedly:
  audioHook();
}
