#include <Keypad.h>
#include <ESP32Servo.h>
#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA 13                    //Define SDA pins
#define SCL 14                    //Define SCL pins

DHTesp dht;                       // create dht object
LiquidCrystal_I2C lcd(0x27,16,2); // initialize the LCD
int dhtPin = 5;                  // the number of the DHT11 sensor pin

//define the symbols on the buttons of the keypad
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {14, 27, 26, 25};   // connect to the row pinouts of the keypad
byte colPins[4] = {4, 21, 22, 23};   // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

Servo  myservo;     // Create servo object to control a servo
int servoPin = 19;  // Define the servo pin
int buzzerPin = 18; // Define the buzzer pin

char passWord[] = {"1234"}; // Save the correct password

void PassWordFunct(){
  static char keyIn[4];     // Save the input character
  static byte keyInNum = 0; // Save the the number of input characters
  char keyPressed = myKeypad.getKey();  // Get the character input
  // Handle the input characters
  if (keyPressed) {
    // Make a prompt tone each time press the key
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    // Save the input characters
    keyIn[keyInNum++] = keyPressed;
    Serial.print("Numero guardado: ");
    Serial.print(keyPressed);
    Serial.print("\n");
    // Judge the correctness after input
    if (keyInNum == 4) {
      bool isRight = true;            // Save password is correct or not
      for (int i = 0; i < 4; i++) {   // Judge each character of the password is correct or not
        Serial.print(keyIn[i]);
        if (keyIn[i] != passWord[i])
          isRight = false;            // Mark wrong passageword if there is any wrong character.
      }
      Serial.print("\n");
      if (isRight) {                  // If the input password is right
        lcd.print("Cont correcta");           
        lcd.setCursor(0, 1);              //set the cursor to column 0, line 1
        myservo.write(90);           // Open the switch
        delay(5000);                  // Delay a period of time
        myservo.write(0);            // Close the switch
        lcd.setCursor(0, 0);              //set the cursor to column 0, line 0
        lcd.print("Puerta abierta");         
        delay(2000);
        
        // Serial.println("Contraseña correcta");
        // Serial.println("Puerta abierta");
      }
      else {                          // If the input password is wrong
        digitalWrite(buzzerPin, HIGH);// Make a wrong password prompt tone
        delay(1000);
        digitalWrite(buzzerPin, LOW);
        Serial.println("La contraseña no es correcta");
      }
      keyInNum = 0; // Reset the number of the input characters to 0
    }
  }
}
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

bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0) {
    return true;
  }
  return false;
}

void setup() {
  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500);  // attaches the servo on servoPin to the servo object
                                        // set the high level time range of the servo motor for an accurate 0 to 180 sweep
  myservo.write(0);                     // Set the starting position of the servo motor
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  Wire.begin(SDA, SCL);           // attach the IIC pin
  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  dht.setup(dhtPin, DHTesp::DHT11); //attach the dht pin and initialize it
  
}

void loop() {
  PassWordFunct();
  TempHumidity();
}


