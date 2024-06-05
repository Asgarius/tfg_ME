//#include <ESP32Servo.h>

const int RedPin = 0;     // Define the traffic light pins  
const int YellowPin = 2;  
const int GreenPin = 15; 
const int servoPin = 19;        // Define the servo pin
const int buzzerPin = 18;       // Define the buzzer pin

//Servo myservo;            // Create servo object to control a servo
int Blink;
int delayValue = 100;     // Define the buzzer timer

void soundAlarm(int delayValue){
  digitalWrite(buzzerPin, HIGH);
  delay(delayValue);
  digitalWrite(buzzerPin, LOW);
}

void entranceMode(int Blinker = 0, int ColorPin = GreenPin){
  if(Blinker == 0){              // Open Door
    digitalWrite(ColorPin, HIGH);
    //llamada al servo open
    delay(2000);             
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

void setup() {
  pinMode(GreenPin, OUTPUT);      // Set lights pinModes  
  pinMode(YellowPin, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);     // Set buzzer pinMode
  entranceMode(2);

  Serial.begin(115200);

}

void loop() {

  /*
  while(digitalRead(SCRpushButton) == HIGH // or checkSensors active)
  {
    if (digitalRead(pushButton) == HIGH) { //the button is pressed: LOW
      entranceMode();
    }
    // Temperature and Humidity LCD
    
  }
  */
}





