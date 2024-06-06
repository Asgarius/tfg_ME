#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"

#define SDA 13                    //Define SDA pins for LCD
#define SCL 14 

#define LEDS_COUNT  8             //Define RGB8
#define LEDS_PIN	  2
#define CHANNEL		  0

DHTesp dht;                       // create dht object
LiquidCrystal_I2C lcd(0x27,16,2); // initialize the LCD
int dhtPin = 5;                  // the number of the DHT11 sensor pin

const uint16_t recvPin = 15;  // Infrared receiving pin
IRrecv irrecv(recvPin);       // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);
int m_color[5][3] = { {255, 0, 0}, {255, 255, 0}, {0, 0, 255}, {255, 255, 255}, {0, 0, 0} };
int delayval = 100; //Change delay of RGB8 

//FUNCTIONS infrared
void controller(unsigned long value){
  switch (value) {
    case 0xFF6897: // Receive the number '0'
    //case 1: MODE NORMAL --> blue = 2, white = 4
    inicioRGB(2);
    break;

    case 0xFF30CF: // Receive the number '1'
    //case 1: alarma de incendios
    Serial.print("Código LEAF: aviso a emergencias \n_ALARMA DE INCENDIOS_\n");
    inicioRGB();
    inicioRGB_AL(1, 2); //yellow = 1, rotation = 2
    break;

    case 0xFF18E7: // Receive the number '2'
    //case 2: seguridad comprometida
    Serial.print("Código ROJO: aviso a seguridad \n_SEGURIDAD COMPROMETIDA_\n");
    inicioRGB();
    inicioRGB_AL(0); //rojo = 0
    break;


    case 0xFF7A85: // Receive the number '3'
    //case 3
    Serial.print("Estás en el case 3!\n");
    break;
  }
  delay(500);
}

void inicioRGB(int j = 4){
  //Inicio del led en un solo color
  for (int i = 0; i < LEDS_COUNT; i++){
    strip.setLedColorData(i, m_color[j][1], m_color[j][1], m_color[j][2]);// Set color data.
		strip.show();
  }
}

void inicioRGB_AL(int j, int ROT = 1){
  //Inicio del led en un solo color
  do{
    for (int i = 0; i < LEDS_COUNT; i++){
      strip.setLedColorData(i, m_color[j][1], m_color[j][1], m_color[j][2]);// Set color data.
      strip.show();
      delay(delayval*2);
      strip.setLedColorData(i, m_color[4][1], m_color[4][1], m_color[4][2]);// Set color data.
      strip.show();
      delay(delayval); 
    }

    for (int i = LEDS_COUNT-1; i > 0; i--){
      strip.setLedColorData(i, m_color[j][1], m_color[j][1], m_color[j][2]);// Set color data.
      strip.show();
      delay(delayval*2);
      strip.setLedColorData(i, m_color[4][1], m_color[4][1], m_color[4][2]);// Set color data.
      strip.show();
      delay(delayval); 
    }

    ROT--;
  }while(ROT > 1)
}

bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0) {
    return true;
  }
  return false;
}

//FUNCTIONS LCD
void TempHumidity(){
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
  delay(2000);

}

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();        // Start the receiver infrared
  strip.begin();
	strip.setBrightness(10);	
  
  Wire.begin(SDA, SCL);           // attach the IIC pin LCD
  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  dht.setup(dhtPin, DHTesp::DHT11); //attach the dht pin and initialize it
  delay(2000);
}

void loop() {
  TempHumidity();
  if (irrecv.decode(&results)) {          // Waiting for decoding
    serialPrintUint64(results.value, HEX);// Print out the decoded results
    Serial.println("");
    controller(results.value);
    irrecv.resume();                      // Release the IRremote. Receive the next value
  }
}












