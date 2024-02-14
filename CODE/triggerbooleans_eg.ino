const int numOfSensors = 2; //number of piezo sensors we are using. can be changed
const int pins[numOfSensors] = {A0, A1}; //input the pin values we're using in the arduino
const float limit = 1000; //limit value that can trigger the boolean

bool trigger[numOfSensors] = {false}; //status of the sensors inside an array

void setup() {

  Serial.begin(115200);

}

void loop() {
  //checking the num of sensors
  for(int i = 0; i < numOfSensors; i++) {
    int sensorVal = analogRead(pins[i]); //new value to read each pin
    bool checkTrigger = trigger[i]; //new bool to check each pin

    //when pin is greater than the thresholdd limit then output the boolean status??
    if()

    //statement to for pin status??
    if(sensorVal != checkTrigger) {

    }

  }

  Serial.println()

}
