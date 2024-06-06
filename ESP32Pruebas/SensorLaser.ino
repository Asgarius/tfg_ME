const int LDRPin = 18;
const int LaserPin = 19;
int y;

/*void setup() {
  pinMode(LaserPin, OUTPUT);
}
void loop() {
  digitalWrite(LaserPin, HIGH);
  delay(1000);
  digitalWrite(LaserPin, LOW);
  delay(1000);
}*/

void setup() {
  pinMode(LaserPin, OUTPUT);  //definir pin como salida
  pinMode(LDRPin, INPUT);
  Serial.begin(115200);
}
 
void loop(){
  digitalWrite(LaserPin, HIGH);
  delay(1000);
  int ldrStatus = analogRead(LDRPin); //Guarda el valor analógico
  //y = map(ldrStatus, 100, 450, 255, 0); //100-450 a 255-0
  Serial.print("Luz del Laser ");
  Serial.println(ldrStatus);
  Serial.println("----------------------------------");
  delay(1000);
  digitalWrite(LaserPin, LOW);
  delay(1000);
  
}



