#include <Keypad.h>
#include <Servo.h>

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
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  String result = "";
  while (result.length() < 1) {
    char key = myKeypad.getKey();
    if (key >= '0' && key <= '9') {
      result += key;
    }
  }
  Serial.print("Has salido del While :D\n");
  Serial.print(result);
  result = "";
  delay(1000);

}
