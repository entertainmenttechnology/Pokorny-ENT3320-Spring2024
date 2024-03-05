//More pins can be added depending on how many devices are used for the project
int pin0 = A0; // Pin for the first piezo sensor
int pin1 = A1; // Pin for the second piezo sensor
int limit = 500; // Threshold value to trigger the boolean statement

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Reads values of the first sensor
  int sensorVal0 = analogRead(pin0);
  // Read values of the second sensor
  int sensorVal1 = analogRead(pin1);

  // Checking if the devuce exceed the threshold for each sensor
  bool triggered0 = sensorVal0 > limit;
  bool triggered1 = sensorVal1 > limit;

  // Print sensor values and trigger status
  Serial.print("Sensor 0 value: ");
  Serial.print(sensorVal0);
  Serial.print(", Triggered: ");
  Serial.println(triggered0);

  Serial.print("Sensor 1 value: ");
  Serial.print(sensorVal1);
  Serial.print(", Triggered: ");
  Serial.println(triggered1);
}