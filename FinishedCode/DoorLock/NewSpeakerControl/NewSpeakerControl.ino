#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

int serial1 = 2;
int serial2 = 3;
int serial3 = 6;
int serial4 = 7;
#define RX 10
#define TX 11
 
SoftwareSerial BTSerial(RX, TX); // (RX, TX)

#define BAUDRATE 9600

SoftwareSerial FPSerial(serial1, serial2); // RX, TX
SoftwareSerial link(serial3, serial4); // RX, TX

DFRobotDFPlayerMini myDFPlayer;
String dataToSend = "N";
void printDetail(uint8_t type, int value);

void setup()
{
  pinMode(serial1, INPUT);
  pinMode(serial2, OUTPUT);
  pinMode(serial3, INPUT);
  pinMode(serial4, OUTPUT);
  //digitalWrite(5, LOW); 
  BTSerial.begin(BAUDRATE);
  FPSerial.begin(9600);
  link.begin(9600);
  Serial.begin(9600);
  //
  Serial.println(F("Initializing DFPlayer ..."));

  if (!myDFPlayer.begin(FPSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  } 
  else
  {
    Serial.println(F("DFPlayer Mini online."));
  }

  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
}

void loop()
{
   char ch;
   while(link.available())
   {
    
    //read incoming char by char:
     ch = link.read();
     if(ch != ')'){
  //    Serial.println("\n");
 //    Serial.println((char) ch);
     if (ch == '1'){
      //  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
        playTrackX(2);
     //   Serial.println("Sam");    
     }else if (ch == '2'){
        playTrackX(3);
     }else if (ch == '3'){
        playTrackX(4);
    //   Serial.println("Maya"); 
     }
     else if (ch == '0'){
        playTrackX(7);
    //  Serial.println("Unknown"); 
     }else if (ch == '4'){
        playTrackX(8);
     }
    // delay(3000);
     send(ch);
     }
   }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
  void send(char ch) { 

    if (ch == '1' || ch == '3' || ch == '2' || ch == '4'){
      dataToSend = "Y";
    }else{
      dataToSend = "N";
    }
    bool rslt;
    
    BTSerial.print(dataToSend);

  // Print the same string to the Serial Monitor for feedback
   // Serial.println(dataToSend);
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2
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
