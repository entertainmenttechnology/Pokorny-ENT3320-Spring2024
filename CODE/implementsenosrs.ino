#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/sin2048_int8.h> 
#include <RollingAverage.h>

int NUM_PINS = 14;//number of pins we're using
float piezovals[14];//values for each pin
float piezoPins[14] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13};//sensor pins
float dataArray[NUM_PINS] = {};//our raw data array
RollingAverage<int, 32> kAverage;

void setup() {
  Serial.begin(11620);
  startMozzi();
}

void updateControl() {
  //within our pins we will read and figure our the average in our cycle. store our data into the array
  for (int i = 0; i < NUM_PINS; i++) {
    float piezoval = mozziAnalogRead(piezoPins[i]);
    piezovals[i] = piezoval;
    float avg = kAverage.next(piezoval);
    dataArray[i] = avg;
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
}

void loop() {

 // NOTE: Ideally, Nothing else should go in LOOP()
}