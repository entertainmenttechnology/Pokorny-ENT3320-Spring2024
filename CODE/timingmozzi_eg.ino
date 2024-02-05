#include <MozziGuts.h>

int counter = 0; //counter variable
int interval = 1000; //interval for audioticks
int resetLimit = 15;//reset once limit is reach

void start() {

  startMozzi();

}

void loop() {

  int currentTick = audioTicks();

  //check if the ticks are incrementing pass zero
  if(currentTick & audioTicks == 0) {
    counter++;

    //conce the limit is reached then it will reset when its incrementing
    if(currentTick > resetLimit) {
        count = 0;
      }
  }
  //print the counter value
  Serial.println(counter);

  eventDelay(10); 
}