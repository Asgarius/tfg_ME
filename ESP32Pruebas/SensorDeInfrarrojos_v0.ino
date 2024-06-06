#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

const uint16_t recvPin = 15; // Infrared receiving pin
IRrecv irrecv(recvPin);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

void controller(unsigned long value){
  switch (value) {
    case 0xFF6897: // Receive the number '0'
    //case 0 
    break;
    case 0xFF30CF: // Receive the number '1'
    //case 1
    Serial.print("Estás en el case 1!\n");
    break;
    case 0xFF18E7: // Receive the number '2'
    //case 2
    Serial.print("Estás en el case 2!\n");
    break;
    case 0xFF7A85: // Receive the number '3'
    //case 3
    Serial.print("Estás en el case 3!\n");
    break;
  }
  delay(500);
}


void setup() {
  Serial.begin(115200);       // Initialize the serial port and set the baud rate to 115200
  irrecv.enableIRIn();        // Start the receiver
  // Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  // Serial.println(recvPin);   //print the infrared receiving pin
}

void loop() {
  if (irrecv.decode(&results)) {          // Waiting for decoding
    serialPrintUint64(results.value, HEX);// Print out the decoded results
    Serial.println("");
    controller(results.value);
    irrecv.resume();                      // Release the IRremote. Receive the next value
  }
  delay(500);
}
