const int numOfSensors = 2; // Number of piezo sensors we are using. Can be modify
const int pins[numOfSensors] = {A0, A1}; // Input the pin values on your arduino. this can be modify
const float limit = 600; // Limit value to trigger the boolean

bool trigger[numOfSensors] = {false}; // Status of the sensors inside an array

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Checking the num of sensors while checking the status and limit
  for (int i = 0; i < numOfSensors; i++) {
    int sensorVal = analogRead(pins[i]); // New value to read each pin

    trigger[i] = sensorVal > limit;

    // Statement to print pin and trigger status
    Serial.print("Sensor: ");
    Serial.print(i);

    // Print sensor value
    Serial.print(", Value: ");
    Serial.print(sensorVal);

    // Print trigger status
    Serial.print(", Triggered: ");

    if (trigger[i]) {
      Serial.println("triggered read");
    } 
    else {
      Serial.println("triggered not read");
    }
  }

  Serial.println();
  delay(10);

}