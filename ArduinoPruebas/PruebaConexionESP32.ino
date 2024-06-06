const int disableAlarm = 9;  //Output de la pasarela
const int blockAlarm = 8;   //Input de la pasarela

bool _blockAlarm;
int var = 0;

void setup() {
  Serial.begin(9600);

  pinMode(blockAlarm, INPUT);    // Pin 8
  pinMode(disableAlarm, OUTPUT); // Pin 9
  digitalWrite(disableAlarm, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("LOL");
  delay(500);
  _blockAlarm = digitalRead(blockAlarm);
  while(_blockAlarm){    //ESCUCHANDO
    digitalWrite(disableAlarm, HIGH);
    Serial.println("CONSEGUIDO! Alarma desactivada ");
    delay(100);
    _blockAlarm = digitalRead(blockAlarm);
    digitalWrite(disableAlarm, LOW);
  }
  
}
