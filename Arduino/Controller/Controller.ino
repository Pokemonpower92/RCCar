/*
    Code for a remote controller.
    Does this work?
*/
#include <RF24.h>

// RF object.
RF24 radio(9, 10);

#define Y_PIN A1
#define X_PIN A0
#define S_PIN 2
#define LED_PIN 8


int y_state = 0;
int x_state = 0;
int s_state = 0;

int y_value = 0;
int x_value = 0;

const byte ADDR[6] = "00001";

// S.XXXX.YYYY
char packet[12];
void setup(){
    radio.begin();
    Serial.begin(9600);
    
    pinMode(Y_PIN, INPUT);
    pinMode(X_PIN, INPUT);
    pinMode(S_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    
    radio.setPayloadSize(sizeof(packet));
    radio.openWritingPipe(ADDR);
    radio.stopListening();
    Serial.println("Contoller starting.");
}

void loop(){

    char ss; 

    y_state = analogRead(Y_PIN);
    x_state = analogRead(X_PIN);
    s_state = digitalRead(S_PIN);

    y_value = map(y_state, 0, 1023, 512, -512);
    x_value = map(x_state, 0, 1023, -512, 512);

    if(s_state == 1) {
        ss = 'T';
    }
    else {
        ss = 'F';
    }
    sprintf(packet, "%c.%04d.%04d", ss, x_value, y_value);
    radio.write(&packet, sizeof(packet));
}
