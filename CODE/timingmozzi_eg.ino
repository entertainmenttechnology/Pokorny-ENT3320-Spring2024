#include <MozziGuts.h>
#include <tables/sin1024_int8.h>
#include <EventDelay.h>


int counter = 0; //counter variable
int interval = 64; //interval for audioticks. a sixteenth note is 1/4 a single beat. 256/4 = 64. 1024/16 = 64.
int resetLimit = 15;//reset once limit is reach


void setup() {


 startMozzi();
 Serial.begin(115200);


}


void loop() {
 int currentTick = audioTicks();


 //check if the ticks are incrementing pass zero
 if(currentTick % interval == 0) {
   counter++;


   //conce the limit is reached then it will reset when its incrementing
   if(coutner > resetLimit) {
       counter = 0;
     }
 }
   //print the counter value
   Serial.println(counter);


   EventDelay(10);
}
