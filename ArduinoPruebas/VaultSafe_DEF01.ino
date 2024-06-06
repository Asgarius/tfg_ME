#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
//Pantalla LCD
LiquidCrystal_I2C lcd(0x27,16,2);

//Servo declaración
#define SERVO_PIN         6             // PIN
#define SERVO_LOCK_POS   20
#define SERVO_UNLOCK_POS 90
Servo lockServo;

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};             // PINES
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};         // PINES
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//Clase nueva
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
const String Password = "1234";   // Contraseña cámara acorazada
const String SecureCode = "1111"; // Contraseña para desactivar SCR
const int disableAlarm = 9;       // Output de la pasarela
const int blockAlarm = 8;         // Input de la pasarela
const int PIRPin= 7;              // Sensor PIR

bool _PIRPin;                     // Variables con sus IDs
bool _blockAlarm, exitAlarm = 0;
int contador = 0;

void initialState(bool SCRCode = 1){
  // Inicializar el LCD
  lcd.init();
  lcd.backlight();
  if(SCRCode){
    lcd.setCursor(2, 0);
    lcd.print("Bienvenido a");
    delay(1000);
    lcd.setCursor(0, 1);
    String message = "SecureBank - SCR";
    for (int i = 0; i < message.length(); i++) {
      delay(150);
      lcd.print(message[i]);
    }
  }else{
    lcd.setCursor(0, 0);
    lcd.print("Fallo Seguridad");
    lcd.setCursor(0, 1);
    lcd.print("PUERTA BLOQUEADA");
    delay(1000);
  }
  delay(1000);
}
String secretCode(bool SCRCode) {
  lcd.clear();
  lcd.setCursor(2, 0);
  if(SCRCode){
    lcd.print("Password:");
  }else{
    lcd.print("SCR Password:");
  }

  lcd.setCursor(5, 1);
  lcd.print("[____]");
  lcd.setCursor(6, 1);
  String result = enterPass(Password.length()); //Utiliza el tamaño de la contraseña estándar
  return result;
}
String enterPass(int tamanio){
  String result = "";
  if(tamanio == 4){
    while (result.length() < tamanio) {
      char key = myKeypad.getKey();
      if (key >= '0' && key <= '9') {
        result += key;
        lcd.print('*');
      }
    }
  }else
  {
    while (result.length() < tamanio) {
      char key = myKeypad.getKey();
      if (key >= '0' && key <= '9') {
        result += key;
      }
    }
  }
  return result;
}
void showWaitScreen(int delayMilis) {
  lcd.setCursor(2, 1);
  lcd.print("[..........]");
  lcd.setCursor(3, 1);
  for (byte i = 0; i < 10; i++) {
    delay(delayMilis);
    lcd.print("=");
  }
}
void compareCheck(String Code, bool SCRCode){
  String CompareCode;
  if(SCRCode){
    CompareCode = Password; //Estándar
  }else{
    CompareCode = SecureCode; //Seguridad
  }
  if (Code.equals(CompareCode) && SCRCode) { //Estándar
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Cont. correcta");
    lcd.setCursor(0, 1);
    lcd.print("Puerta abierta");
    delay(2000);
    unlockVault();
    delay(5000);
    lockVault();
  } else if (Code.equals(CompareCode) && !SCRCode) { //Seguridad
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Cont. correcta");
    lcd.setCursor(0, 1);
    lcd.print("SCR DISABLE");
    delay(100);
    unlockVault();
    delay(5000);
    lockVault();
    exitAlarm = 1;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cont. incorrecta");
    lcd.setCursor(5, 1);
    lcd.print("ERROR");
    delay(2000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Apagando...");
  delay(2000);
  defaultMode();
}
void processCheck(bool SCRCode = 1){
  String Code = secretCode(SCRCode);
  showWaitScreen(200);
  compareCheck(Code, SCRCode);
}
void defaultMode(){
  lcd.noDisplay();
  lcd.noBacklight();
  lockVault();
}
void lockVault() {   //bloquea la puerta
  lockServo.write(SERVO_LOCK_POS);
}
void unlockVault() {   //desbloquea la puerta
  lockServo.write(SERVO_UNLOCK_POS);
}
void peopleInside(int contador) { //Contabiliza las personas que se encuentran en la Cámara Acorazada
  if(contador < 0) contador = 0;
  Serial.print("Personas en la cámara acorazada: ");
  Serial.print(contador);
}
bool detectPIR(){
  while(contador < 2){
    bool _PIRPin = digitalRead(PIRPin);
    if (_PIRPin == HIGH)
    {
      Serial.println("DETECTADO ");
      Serial.println();
      contador++;
    }
    else
    {
      Serial.print("----------\n ");
    }
    delay(500);
    if(contador == 2){
      contador = 0;
      return _PIRPin;
    }
  }
}

void setup() {
  lockServo.attach(SERVO_PIN);
  Serial.begin(115200);

  pinMode(blockAlarm, INPUT);    // Pin 8
  pinMode(disableAlarm, OUTPUT); // Pin 9
  pinMode(PIRPin, INPUT);
  defaultMode();
  //_blockAlarm = 1; //PRUEBA
}

void loop() {
  _blockAlarm = digitalRead(blockAlarm);
  while(!_blockAlarm) { //MODO ESTÁNDAR
    Serial.println("STANDART MODE\n ");
    _PIRPin = detectPIR();
    if (_PIRPin == HIGH) //Acceso desde el exterior
    { Serial.print("CASE 1\n ");
      initialState();
      processCheck();
      //contador++;
      //peopleInside(contador);
    }
    /*else //Acceso desde el interior
    { Serial.print("CASE 2\n ");
      initialState();
      processCheck();
      contador--;
      peopleInside(contador);
    }*/
    _blockAlarm = digitalRead(blockAlarm);
  }
  defaultMode();
  delay(500);

  String result = enterPass(1); //Espera a que se introduzca un número
  bool _PIRPin = detectPIR();
  if (_PIRPin && result) { 
    initialState(!_blockAlarm);
    processCheck(!_blockAlarm);
    if(exitAlarm == 1){
    digitalWrite(disableAlarm, HIGH);
    delay(500);
    digitalWrite(disableAlarm, LOW);
    exitAlarm = 0;
    }
  }

}





