// Declaration of pins.
const int ForceSensor = A0;
const int MotionSensor = A1;
const int PhotoResistor = A2;
const int SoilMoisture = A3;

const int Led1 = 8;
const int Led2 = 10;
const int Led3 = 11;
const int Led4 = 12;
const int TimerLED = 9;

void setup()
{
  Serial.begin(9600);
  pinMode(ForceSensor, INPUT);
  pinMode(MotionSensor, INPUT);
  pinMode(PhotoResistor, INPUT);
  pinMode(SoilMoisture, INPUT);
  
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  
  pinMode(TimerLED, OUTPUT);
  
  PCMSK1 |= bit(PCINT8); // Enable interrupt A0
  PCMSK1 |= bit(PCINT9); // Enable interrupt A1
  PCMSK1 |= bit(PCINT10);// Enable interrupt A2
  PCMSK1 |= bit(PCINT11);// Enable interrupt A3

  PCICR |= bit(PCIE1);
  
  cli();  // Disable Interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 15625;  // For 1 second interval with a 16MHz clock
  
  TCCR1B |= (1 << WGM12) | (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);	// Timer Mask
 
  sei();  // Enable Interrupts
}

void loop()
{
}

ISR(PCINT1_vect) 
{
  if(digitalRead(ForceSensor))
  {
    digitalWrite(Led1, HIGH);
  } else digitalWrite(Led1, LOW);
  
  if(digitalRead(PhotoResistor))
  {
    digitalWrite(Led2, HIGH);
  } else digitalWrite(Led2, LOW);
  
  if(digitalRead(SoilMoisture))
  {
    digitalWrite(Led3, HIGH);
  } else digitalWrite(Led3, LOW);
  
  if(digitalRead(MotionSensor))
  {
    digitalWrite(Led4, HIGH);
  } else digitalWrite(Led4, LOW);
}

ISR(TIMER1_COMPA_vect) // Performing the Timer Interrrupt
{
  int LEDState = digitalRead(TimerLED);
  digitalWrite(TimerLED, !LEDState);
}
