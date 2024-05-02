/* 
Simple Piezo Trigger Example by Ian Pokorny
Piezo Circuit: 
- Diode(1N4148) in series with piezo signal
- followed by 1Mohm/1Mohm voltage divider to ADC pin
*/

#define piezoPin A0   //define ADC pin

int threshold = 3;    //adjust trigger threshold here

void setup() 
{
  Serial.begin(115200);   //open serial port for monitoring
}

void loop() 
{
  int piezo = analogRead(piezoPin);   //piezo ADC read

  if (piezo >= threshold)   //if piezo read is equal or above threshold,
  {
    Serial.println(piezo);    //print piezo read to serial monitor
    //you trigger anything from here, like turn a bool value (flag) on & off
  }
}