  /*https://hetpro-store.com/TUTORIALES/sensor-de-fuerza-o-presion-mf01/*/

int AnalogPin = 0;   // Sensor conectado a Analog 0
int LEDpin = 6;      // LED conectado a Pin 6 (PWM)
int ResRead;         // La Lectura de la Resistencia por División de Tensión
int BrilloLED;

void setup()
{
  Serial.begin(9600); // Enviaremos la información de depuración a través del Monitor de Serial
  pinMode(LEDpin, OUTPUT);
}
void loop()
{
  ResRead = analogRead(AnalogPin); // La Resistencia es igual a la lectura del sensor (Analog 0)
  Serial.print("Lectura Analogica = ");
  Serial.println(ResRead);

  BrilloLED = map(ResRead, 0, 1023, 0, 255);
  // Cambiar el rango de la lectura analógica (0-1023)
  // Utilizamos en analogWrite 8 bits (0-255) configurados en el map
  analogWrite(LEDpin, BrilloLED);

  delay(100); //Cien “ms” de espera en cada lectura
}