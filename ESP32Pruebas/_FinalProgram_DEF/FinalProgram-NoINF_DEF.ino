#include "BluetoothSerial.h"
#include "string.h"
#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA 13                  // Define SDA pins for LCD Pin 13
#define SCL 14                  // Define SCL pins for LCD Pin 14

DHTesp dht;                       // create dht object
LiquidCrystal_I2C lcd(0x27,16,2); //initialize the LCD
const int dhtPin = 5;                   // the number of the DHT11 sensor pin

BluetoothSerial SerialBT;         // Bluetooth
char buffer[20];
static int count = 0;

unsigned long ModeStandard = 0xFF6897;
unsigned long ModeEmergency = 0xFF30CF;
unsigned long ModeSCR = 0xFF18E7;

const int RedPin = 0;           // Define the traffic light Pin 0 
const int YellowPin = 2;        // Define the traffic light Pin 2
const int GreenPin = 15;        // Define the traffic light Pin 15
const int buzzerPin = 18;       // Define the buzzer Pin 18
int Blink;
int delayValue = 100;           // Define the buzzer timer

const int ResPinIN = 4;         // Define resistance WeightSensor Pin 4
int weightVal, mapWeightVal = map(weightVal, 2400, 4100, 0, 100);

const int AlarmOFF = 22;  //ARDp9 //Input Conversor Pin 22 
const int AlarmON = 23;   //ARDp8 //Output Conversor Pin 23
bool _AlarmOFF;                     //Varible SCR
bool defuseAlarm = 0;               //Varible emergencias

void lecturaBluetooth();
void lecturaBluetooth1();


void entranceMode(int Blinker = 0, int ColorPin = GreenPin){
  if(Blinker == 0){              // Open Door
    digitalWrite(ColorPin, HIGH);
    delay(3000);             
    digitalWrite(ColorPin, LOW);
    delay(500);
      
  }else if(Blinker == 1){         // Emergency or SCR-Mode
    digitalWrite(ColorPin, HIGH);
    delay(500);             
    digitalWrite(ColorPin, LOW);
    delay(500); 
    
  }else if(Blinker == 2){         // Inicio coche fantástico
    for(int i=0; i < Blinker; i++){
      digitalWrite(GreenPin, HIGH);
      delay(200);
      digitalWrite(YellowPin, HIGH);
      delay(200);
      digitalWrite(RedPin, HIGH);
      delay(200);
      digitalWrite(RedPin, LOW);
      delay(200);
      digitalWrite(YellowPin, LOW);
      delay(200);
      digitalWrite(GreenPin, LOW);
      delay(200);
    }
  }
}
bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0) {
    return true;
  }
  return false;
}
bool TempHumidityCH(){
  float Hum = dht.getHumidity();
  float Tem = dht.getTemperature();
  if(Hum > 90 || Tem > 50){
    return true;
    Serial.println("Fallo TempYHum");
  }else{
    return false;
  }
}
bool WeightCH(){
  weightVal = analogRead(ResPinIN);                 // La Resistencia es igual a la lectura del sensor, funcionamiento 100g - 10kg
  //mapWeightVal = map(weightVal, 200, 3500, 0, 100);     // Cambiar el rango de la lectura para 220ohm
  mapWeightVal = map(weightVal, 2400, 4100, 0, 1000);      // Cambiar el rango de la lectura para 10Kohm
  if(mapWeightVal > 900){
    Serial.print("Fallo Weight: ");
    Serial.println(weightVal);
    return true;
  }
  return false;
}
int checkSensors(){
  if(TempHumidityCH()){
    return 2; // Emergency
  }else if(WeightCH()){
    return 1; // SCR
  }
  return 0;
}
void TempHumidityLCD(){
  // read DHT11 data and save it 
  flag:TempAndHumidity DHT = dht.getTempAndHumidity();
  if (dht.getStatus() != 0) {       //Determine if the read is successful, and if it fails, go back to flag and re-read the data
    goto flag;
  }  
  lcd.setCursor(0, 0);              //set the cursor to column 0, line 0
  lcd.print("Temperatura:");        
  lcd.print(DHT.temperature); 
  lcd.setCursor(0, 1);              //set the cursor to column 0, line 1 
  lcd.print("Humedad    :");        
  lcd.print(DHT.humidity);  
  //delay(2000);

}
void soundAlarm(){
  digitalWrite(buzzerPin, HIGH);
  delay(delayValue);
  digitalWrite(buzzerPin, LOW);
}
void controller(unsigned long value = 0){ 
  switch (value) {
    case 0xFF6897:      //case 0: modo normal
      //Bucle que haga la función de mostrar en el RGB8 una luz azul y una canción.
      Serial.println("ModeStandard");
      entranceMode();
    break;

    case 0xFF30CF:      //case 1: alarma de incendios
      //Activa una pequeña alarma que funciona al mismo tiempo que una rotatoria amarilla, se desactiva con un pulsador.
      Serial.println("Emergency");
      do{
        entranceMode(1, YellowPin);
        soundAlarm();
        lecturaBluetooth1();
      }while(!defuseAlarm);
      defuseAlarm = 0;
      Serial.println("SALIÓ");
    break;

    case 0xFF18E7:      //case 2: seguridad comprometida
      //Alarma silenciosa, envía un mensaje por bluetooth al usuario y cierra cámara acorazada, se desactiva en la cámara acorazada.
      Serial.println("Fallo de seguridad");
      digitalWrite(AlarmON, HIGH);
      do{
        //Acción
        entranceMode(1, RedPin);
        _AlarmOFF = digitalRead(AlarmOFF);
      }while(_AlarmOFF);
      digitalWrite(AlarmON, LOW);
      Serial.println("CONSEGUIDO! Alarma desactivada ");
    break;
    
    case DEFAULT:
      Serial.println("CASO DEFAULT");
      delay(1000);
    break;
  }
}
void lecturaBluetooth1(){
  while(SerialBT.available()){
    buffer[count] = SerialBT.read();
    count++;
  }
  if(count>0){
    Serial.print(buffer);
    if(strncmp(buffer, "DESACTIVAR", 10)==0){
      SerialBT.write(SerialBT.print("Alarma Desactivada\n"));
      defuseAlarm = true;
    }
  }
  count=0;
  memset(buffer,0,20);
}
void lecturaBluetooth(){
  while(SerialBT.available()){
    buffer[count] = SerialBT.read();
    count++;
  }
  if(count>0){
    Serial.print(buffer);
    if(strncmp(buffer, "Algo", 4)==0){  // Prueba
      controller();
      Serial.print("CONTROL");
    }
    if(strncmp(buffer, "0xFF6897", 8)==0){
      SerialBT.write(SerialBT.print("Abrir Puerta"));
      controller(ModeStandard);
    }
    if(strncmp(buffer, "0xFF30CF", 8)==0){
      SerialBT.write(SerialBT.print("Alarma de Incendios\n"));
      SerialBT.write(SerialBT.print("OFF para desactivar la alarma\n"));
      controller(ModeEmergency);
    }
    if(strncmp(buffer,"0xFF18E7", 8)==0){
      SerialBT.write(SerialBT.print("Modo3"));
      controller(ModeSCR);
    }

    count=0;
    memset(buffer,0,20);
  }
}



void setup() {
  pinMode(GreenPin, OUTPUT);      // Set lights pinModes  
  pinMode(YellowPin, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);     // Set buzzer pinMode
  entranceMode(2);

  Wire.begin(SDA, SCL);           // attach the IIC pin LCD
  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  dht.setup(dhtPin, DHTesp::DHT11); //attach the dht pin and initialize it

  SerialBT.begin("SCR-Bank"); //Bluetooth device name
  Serial.begin(115200);
  Serial.println("\nThe device started, now you can pair it with bluetooth!");
}

void loop() {
  int Sensors = checkSensors();
  //while(Sensors == 0){
  while(Sensors != 2){ // WeightSensor doesn´t work¿?
    TempHumidityLCD();
    lecturaBluetooth();
  }
  SerialBT.write(SerialBT.print("Alerta!\n"));
  Serial.print(Sensors);
  if (checkSensors() == 1){
    controller(ModeEmergency);
  }else{
    controller(ModeSCR);
  }
  delay(1000);
}

