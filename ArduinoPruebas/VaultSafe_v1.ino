#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
//Pantalla LCD
LiquidCrystal_I2C lcd(0x27,16,2);

//Servo declaración
#define SERVO_PIN         6
#define SERVO_LOCK_POS   20
#define SERVO_UNLOCK_POS 90
Servo lockServo;

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//Clase nueva
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
String Password = "1234";
String SecureCode = "1111";
bool SensProx = 0;  //Sensor de Proximidad/Presencia
const int blockAlarm = 8;   //Input de la pasarela

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
  String result = "";
  while (result.length() < 4) {
    char key = myKeypad.getKey();
    if (key >= '0' && key <= '9') {
      lcd.print('*');
      result += key;
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
    CompareCode = Password;
  }else{
    CompareCode = SecureCode;
  }
  if (Code.equals(CompareCode)) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Cont. correcta");
    lcd.setCursor(0, 1);
    lcd.print("Puerta abierta");
    delay(2000);
    unlockVault();
    delay(5000);
    lockVault();
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
  showWaitScreen(100);
  compareCheck(Code, SCRCode);
  
}

void defaultMode(){
  lcd.noDisplay();
  lcd.noBacklight();
  lockVault();
  pinMode(blockAlarm, INPUT);
}

void lockVault() {   //bloquea la puerta
  lockServo.write(SERVO_LOCK_POS);
}

void unlockVault() {   //desbloquea la puerta
  lockServo.write(SERVO_UNLOCK_POS);
}


void setup() {
  lockServo.attach(SERVO_PIN);
  Serial.begin(115200);
  pinMode(blockAlarm, INPUT);

}

void loop() {
  SensProx = 1; // Variable para el sensor de Proximidad que se inserte más adelante.
  //Serial.print(blockAlarm);
  /*if(SensProx && !blockAlarm){ //caso de no alarma
    initialState();
    processCheck();
  }else if(SensProx && blockAlarm){ //salta la alarma
    lockVault();
    processCheck(!blockAlarm);

  }*/
  initialState();
  processCheck();

  defaultMode();
  delay(5000);
}
