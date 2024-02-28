const int numOfSensors = 1; // Number of piezo sensors we are using. Can be modify
int pins[numOfSensors] = {A0}; // Input the pin values on your arduino. this can be modify
int limit = 600; // Limit value to trigger the boolean
int sensorVal[numOfSensors] = {0};


bool trigger[numOfSensors] = {false}; // Status of the sensors inside an array


void setup() {
 Serial.begin(115200);
}

void loop() {
 // Checking the num of sensors while checking the status and limit
 for (int i = 0; i < numOfSensors; i++) {
    sensorVal[i] = analogRead(pins[i]); // New value to read each pin

    trigger[i] = sensorVal > limit;//device is trigger once the sensor values are greater than the limit

    //Statement to print pin and trigger status
    Serial.print("Sensor: ");
    Serial.print(numOfSensors);
    
    // Print sensor value
    Serial.print(", Value: ");
    Serial.print(sensorVal[i]);
  }
}