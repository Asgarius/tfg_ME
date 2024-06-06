/*https://www.luisllamas.es/multitarea-en-arduino-blink-sin-delay/*/

unsigned long interval1 = 1000;
unsigned long interval2 = 800;
unsigned long previousMillis1;
unsigned long previousMillis2;

void debug(String text)
{
  Serial.print(millis());
  Serial.print('\t');
  Serial.println(text);
}

void setup() {
  Serial.begin(115200);
  previousMillis1 = millis();
  previousMillis2 = millis();
}

void loop() {
  unsigned long currentMillis = millis();

  // Gestionar el desbordamiento
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1)
  {
    action1();
    previousMillis1 = millis();
  }
  if ((unsigned long)(currentMillis - previousMillis2) >= interval2)
  {
    action2();
    previousMillis2 = millis();
  }
}

void action1()
{
  debug("Action1");
}

void action2()
{
  debug("Action2");
}