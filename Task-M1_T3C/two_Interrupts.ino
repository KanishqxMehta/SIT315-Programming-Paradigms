int Reading;

void setup()
{
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  //pinMode(A0, INPUT);
  pinMode(2, INPUT);
  digitalWrite(12, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(2), detectMotion, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), detectForce, CHANGE);  
}

void loop()
{
  
}

void detectForce() {
  Reading = analogRead(3);

  int forceThreshold = 500;

  // Add some interval to avoid rapid on/off switching
  int interval = 8;

  if (Reading > forceThreshold + interval) {
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }

  Serial.println(Reading);
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
