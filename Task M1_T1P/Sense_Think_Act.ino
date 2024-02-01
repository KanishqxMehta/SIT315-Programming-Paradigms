void setup()
{
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(2, INPUT);
  pinMode(12, OUTPUT);
  
    digitalWrite(12, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(2), detectMotion, CHANGE);
}

void loop()
{
}

void detectMotion(){
    if (digitalRead(2)) {
    digitalWrite(11, HIGH);
    delay(250);
    digitalWrite(12, LOW);
    delay(250);
    Serial.println("Motion Detected");
  } else {
    digitalWrite(12, HIGH);
    delay(250);
    digitalWrite(11, LOW);
    delay(250);
    Serial.println("Motion Not Detected");
  }

}
