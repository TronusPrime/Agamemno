#include <Servo.h>

// SimpleRx - the slave or the receiver
#include <SoftwareSerial.h>

#define RX 3
#define TX 2

#define BAUDRATE 9600

char c = ' ';
const int pinButton = 5;

SoftwareSerial BTSerial(RX, TX); // (RX, TX)
Servo myservo;
int ls = 100;    // ls
int us = 20;

//===========

void setup() {
    pinMode(pinButton, INPUT_PULLUP);
    myservo.attach(8);  // attaches the servo on pin 9 to the servo object
    myservo.write(ls); // InitialLock State
    Serial.begin(9600);
    
  // HM-10 default speed in AT command mode
  BTSerial.begin(BAUDRATE);
}

//=============

 
void loop() {
  failsafe();

  // Keep reading from HM-10 and send to Arduino Serial Monitor  
  while(BTSerial.available() > 0) {

    // Read from the bluetooth buffer
    c = BTSerial.read();

    // Write the received bluetooth data to the Serial Monitor
    if (c == 'Y' || c == 'N'){
          Serial.println(c);
          lock(c);
    }
      failsafe();
  }
  failsafe();
  delay(1000);
}

void lock(char c) {
  if(c == 'Y'){
    myservo.write(us);
    delay(15000); //wait 15 seconds
    myservo.write(ls);
  }else{
    myservo.write(ls);
  }
}
void failsafe(){
     int stateButton = digitalRead(pinButton);
    if(stateButton == 0) {
         myservo.write(us);
         delay(15000); //wait 15 seconds
         myservo.write(ls); 
  } else {
        myservo.write(ls);
  }
}
