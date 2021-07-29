/*
    This is the code for a remote controlled 
    car. 
*/
#include <RF24.h>

// RF object.
RF24 radio(9, 10);

// Motor Pins
#define ENA A0
#define ENB A1

#define LEFT_REV 8
#define LEFT_FWD 7

#define RIGHT_REV 4
#define RIGHT_FWD 3

// Analog stick mappings.
int X_VEL = 0;
int Y_VEL = 0;
int STATE = 0;

uint8_t address[][6] = {"00001"};

// S.XXXX.YYYY
char packet[12];
void setup(){
    radio.begin();
    Serial.begin(9600);
    
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(LEFT_FWD, OUTPUT);
    pinMode(LEFT_REV, OUTPUT);
    pinMode(RIGHT_FWD, OUTPUT);
    pinMode(RIGHT_REV, OUTPUT);

    radio.setPayloadSize(sizeof(packet));
    radio.openReadingPipe(1, address[0]);
    radio.startListening();
    Serial.println("Receiver starting.");
}

void loop(){
    if(radio.available()){
      uint8_t bytes = radio.getPayloadSize();
      radio.read(&packet, bytes);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println(packet);

      // Read the packet and drive the car. 
      process_packet(packet, STATE, X_VEL, Y_VEL);
      move_car(X_VEL, Y_VEL);
    }
}

void process_packet(char *packet, int &state, int &x, int &y) {
  // Derive state, x, and y from packet. 
  // packet is a char* or size 12 formatted as:
  // S.XXXX.YYYY

  char S;
  char X[5]; 
  char Y[5];

  S = packet[0];
  
  strncpy(X, packet + 2, 4);
  X[4] = '\0';

  strncpy(Y, packet + 7, 4);
  Y[4] = '\0';

  if (S == 'T') state = 1;
  else state = 0;

  x = atoi(X);
  y = atoi(Y);
}

void move_car(int x, int y) {

  int left_speed  = abs(map(y, -512, 512, -250, 250));
  int right_speed = abs(map(y, -512, 512, -250, 250));

  int x_mapped = abs(map(x, -512, 512, -250, 250));

  // Determine if we're moving forwards or backwards. 
  if (y > 20) {
      digitalWrite(LEFT_FWD, HIGH);
      digitalWrite(RIGHT_FWD, HIGH);

      digitalWrite(LEFT_REV, LOW);
      digitalWrite(RIGHT_REV, LOW);
  }
  else if (y < -20){
      digitalWrite(LEFT_FWD, LOW);
      digitalWrite(RIGHT_FWD, LOW);

      digitalWrite(LEFT_REV, HIGH);
      digitalWrite(RIGHT_REV, HIGH);
  }
  else {
      left_speed = 0;
      right_speed = 0;
  }

  // Left and right movement.
  if (x > 20) {
    // If we're going right then add to left
    // and subtract from the right.
    left_speed  += x_mapped;
    right_speed -= x_mapped;

    if(left_speed > 250) left_speed  = 250;
    if(right_speed < 0 ) right_speed = 0;
  }
  else if (x < -20){
    // Opposite for left turns. 
    left_speed  -= x_mapped;
    right_speed += x_mapped;

    if(right_speed > 250) right_speed  = 250;
    if(left_speed < 0 ) left_speed = 0;
  }
  
  analogWrite(ENA, left_speed);
  analogWrite(ENB, right_speed);
}
