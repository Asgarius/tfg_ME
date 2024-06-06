const int PIRPin= 7;
int contador = 0;
void setup()
{
  pinMode(PIRPin, INPUT);
  Serial.begin(115200);
}

void loop()
{
  bool _PIRPin = digitalRead(PIRPin);
 
  if (_PIRPin == HIGH)
  {
    Serial.println("DETECTADO: ");
    Serial.print(contador);
    Serial.println();
    contador++;
  }
  else
  {
    Serial.print("AAAAAAAAAAAAA\n ");
    contador = 0;
  }
  delay(500);
}