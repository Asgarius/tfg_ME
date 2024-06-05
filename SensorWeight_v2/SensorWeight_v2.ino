const int PIN_RES_IN = 4;
int weightVal, mapWeightVal = map(weightVal, 0, 1023, 0, 100);            // Para la lectura del sensor de fuerza resistivo

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RES_IN, INPUT);
}

void loop(){
  weightVal = analogRead(PIN_RES_IN);                 // La Resistencia es igual a la lectura del sensor, funcionamiento 100g - 10kg
  //mapWeightVal = map(weightVal, 200, 3500, 0, 100);     // Cambiar el rango de la lectura para 220ohm
  mapWeightVal = map(weightVal, 2400, 4100, 0, 100);      // Cambiar el rango de la lectura para 10Kohm

  Serial.print("Lectura Analogica = ");
  Serial.println(weightVal);
  Serial.print("Lectura Mapeada = ");
  Serial.println(mapWeightVal);
  Serial.println("----------------------------------");

  delay(1000); //Cien “ms” de espera en cada lectura
}