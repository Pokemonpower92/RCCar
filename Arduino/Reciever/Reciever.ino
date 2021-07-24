/*
    Code for a remote controller 
*/
#include <RF24.h>

// RF object.
RF24 radio(9, 10);
#define F_LED 8
#define B_LED 7
#define L_LED 6
#define R_LED 5
#define S_PIN 2
#define R_PIN 0

const byte ADDR[6] = "00001";

void setup(){
    radio.begin();
    Serial.begin(9600);

    for(int i = 8; i <= 5; i++){
      pinMode(i, OUTPUT);
    }
    pinMode(S_PIN, OUTPUT);
    pinMode(R_PIN, OUTPUT);

    radio.openReadingPipe(0, ADDR);
    radio.startListening();
    Serial.println("Receiver starting.");
}

void loop(){
    // S.XXXX.YYYY
    char packet[12];

    if(radio.available()){
      digitalWrite(R_PIN, HIGH);
      radio.read(&packet, sizeof(packet));
      Serial.println(packet);
      delay(500);
      digitalWrite(R_PIN, LOW);
      delay(500);
    }
    
    
}
