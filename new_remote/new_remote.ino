#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

#define CE_PIN 10
#define CSN_PIN 9

RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = { "1Node", "2Node" };

struct Data_Package{
  uint8_t l = 0;
  uint8_t r = 0;
};
Data_Package controls;

void setup(){

  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  Serial.begin(9600);
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
    }
  
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(controls));
  radio.openWritingPipe(address[0]);
  radio.stopListening();
}

void loop(){
  controls.l = map(analogRead(A2),1023,0,0,255);
  controls.r = map(analogRead(A3),1023,0,0,255);

  Serial.print(controls.l);
  Serial.println(controls.r);
  
  radio.write(&controls, sizeof(controls));
}