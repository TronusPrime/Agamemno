 //Receiver code 
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



  // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/2, /*tx =*/3);
SoftwareSerial link(6 , 7); // Rx, Tx

#define FPSerial softSerial

DFRobotDFPlayerMini myDFPlayer;

#define CE_PIN   9
#define CSN_PIN 10
const byte slaveAddress[5] = {'R','x','A','A','A'};
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
char dataToSend = 'N';

void printDetail(uint8_t type, int value);

  
 // byte greenLED = 12;
  //char cString[20];
  //byte chPos = 0;
 // byte ch = 0;
//  char dataStr[6];

  void setup() 
  {
    //Serial.begin(9600);    //setup serial monitor
    #if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
#else
  FPSerial.begin(9600);
#endif
  digitalWrite(5,LOW);
  Serial.begin(9600);
 
  Serial.println("SimpleTx Starting");
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3,5); // delay, count
  radio.openWritingPipe(slaveAddress);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));

  link.begin(9600); //setup software serial
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
    //pinMode(greenLED, OUTPUT);

  }

  void loop()  
  {  
 // if (link.available())
 //     Serial.write(link.read());
 // if (Serial.available())
 //     link.write(Serial.read());  
   
   char ch;
   while(link.available())
   {
    
    //read incoming char by char:
     ch = link.read();
     if(ch != ')'){
     Serial.println((char) ch);
     if (ch == '1'){
      //  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
        playTrackX(2);
        Serial.println("Sam");    
     }else if (ch == '3'){
      playTrackX(4);
      Serial.println("Maya"); 
     }
     else if (ch == '0'){
      playTrackX(7);
      Serial.println("Unknown"); 
     }
     delay(3000);
     send(ch);
     }
   }

   //digitalWrite(greenLED, HIGH); //flash led to show data is arriving
   //delay(10);
   //digitalWrite(greenLED, LOW);
  // }
  // cString[chPos] = 0; //terminate cString
  // chPos = 0;
   
  // Serial.print(cString);
  }
  void send(char ch) {

    if (ch == '1' || ch == '3'){
      dataToSend = 'Y';
    }else{
      dataToSend = 'N';
    }
    bool rslt;
    
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2
    Serial.print(dataToSend);
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }
} 
  void playTrackX (byte x) {
  
  FPSerial.write((byte)0x7E);
  FPSerial.write((byte)0xFF);
  FPSerial.write((byte)0x06);
  FPSerial.write((byte)0x03);
  FPSerial.write((byte)0x00);
  FPSerial.write((byte)0x00);
  FPSerial.write((byte)x);
  FPSerial.write((byte)0xEF);
}
