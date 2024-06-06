const int AlarmOFF = 22;   //Input de la pasarela, Pin 22
const int AlarmON = 23;    //Output de la pasarela, Pin 23

bool _AlarmOFF;
bool A = true;

void setup() {
  Serial.begin(115200);

  pinMode(AlarmON, OUTPUT);    
  pinMode(AlarmOFF, INPUT);    
  
}

void loop() {
  while(A){
    Serial.println("Se activa alarma: ");
    digitalWrite(AlarmON, HIGH);
    do{
      //Acción
      _AlarmOFF = digitalRead(AlarmOFF);
    }while(_AlarmOFF);
    Serial.println(_AlarmOFF);
    digitalWrite(AlarmON, LOW);
    Serial.println("CONSEGUIDO! Alarma desactivada ");
    A = false;
  }


}
