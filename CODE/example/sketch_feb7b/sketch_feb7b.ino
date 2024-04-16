int Button = 12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Button, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(Button)==1){
    Serial.write('1');

  }
  delay(20);

}
