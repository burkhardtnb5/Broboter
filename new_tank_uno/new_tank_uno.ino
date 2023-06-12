#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = { "1Node", "2Node" };

struct Data_Package{
  uint8_t l = 0;
  uint8_t r = 0;
};
Data_Package controls;

void setup() {

  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  Serial.begin(9600);
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  radio.setPALevel(RF24_PA_MIN);
  radio.setPayloadSize(sizeof(controls));
  radio.openReadingPipe(0, address[0]);
  radio.startListening();

  

}

void loop() {
  uint8_t pipe;
    if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
      radio.read(&controls, bytes);             // fetch payload from FIFO
      Serial.print(F("Received "));
      Serial.print(bytes);  // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);  // print the pipe number
      Serial.print(F(": "));
      Serial.print(controls.l);
      Serial.print(" ");
      Serial.println(controls.r);  // print the payload's value

      analogWrite(6, controls.l);
      analogWrite(5, controls.r);

      delay(100);
    }
    else{
      analogWrite(6, 0);
      analogWrite(5, 0);
    }

}
