
int sensorPin = 12; // the number of the infrared motion sensor pin

void setup() {
  pinMode(sensorPin, INPUT);  // initialize the sensor pin as input
  // pinMode(ledPin, OUTPUT);    // initialize the LED pin as output
}

void loop() {
  // Turn on or off LED according to Infrared Motion Sensor
  digitalWrite(ledPin, digitalRead(sensorPin));
  if(digitalRead(sensorPin)){
    //case KeyPad of the SCR-CAGE


  }
  delay(1000);
}
