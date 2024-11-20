/***************************************************
Agamemno Fingerprint Lock Project
By: Sami Hasan
This is the Fingerprint Storage Code
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
//#include <Wire.h>
RTC_DS3231 rtc;
//char t[32];
SoftwareSerial mySerial(2, 3);
SoftwareSerial link(6, 7); //RX, TX -> 
int f = 0;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
File myFile;

void setup()
{
  link.begin(9600);
  Serial.begin(9600);

  if (!SD.begin(10)) {
    Serial.println(F("initialization failed!"));
    while (1);
  }
  Serial.println(F("initialization done.")); 

    if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }
 //_______________________________-
   //  THIS WILL RESET TIME WHEN UNCOMMENTED
     //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 //_______________________________-


  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println(F("\n\nAdafruit finger detect test"));
  //myFile = SD.open("test.txt", FILE_WRITE);

  
        myFile = SD.open("test.txt", FILE_WRITE);
      if (myFile) {
    Serial.print(F("Writing to test.txt..."));
    myFile.println("______________________________New Session");
    // close the file:
    myFile.close();
    Serial.println(F("done.")); 
  } else {
    // if the file didn't open, print an error:
    Serial.println(F("error opening test.txt"));
  } 

  
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println(F("Found fingerprint sensor!"));
  } else {
    Serial.println(F("Did not find fingerprint sensor"));
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print(F("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example."));
  }
  else {
    Serial.println(F("Waiting for valid finger..."));
      Serial.print(F("Sensor contains ")); Serial.print(finger.templateCount); Serial.println(F(" templates"));
  }

//  Wire.begin();
//  rtc.begin();
// rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
//  //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));
  
}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_NOTFOUND) {
    logData("Unrecognized User Detected!");
    link.write('0');
    return -1;
  }
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  switch(finger.fingerID){
    case(1):
         logData("Sam Entered!");
         link.write('1');
         break;
    case(2):
         logData("Sam Entered!");
         link.write('1');
         break;
    case(3):
         logData("Maya Entered!");
         link.write('3');
         break;
    case(4):
         logData("Maya Entered!");
         link.write('3');
         break;
    case(5):
         logData("Hannah Entered!");
         link.write('4');
         break;
    case(6):
         logData("Hannah Entered!");
         link.write('4');
         break;
    case(7):
         logData("Nick Entered!");
         link.write('2');
         break;
    case(8):
         logData("Nick Entered!");
         link.write('2');
         break;
  }
  return finger.fingerID;
}
void logData(String str){
  Serial.print(F("SRAM left: "));
  Serial.println(freeRam());
      myFile = SD.open("test.txt", FILE_WRITE);
      if (myFile) {
    DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print('-');
    myFile.print(now.month(), DEC);
    myFile.print('-');
    myFile.print(now.day(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);

    myFile.print("-----"); // delimiter between timestamp and data
    myFile.print(str);
    myFile.write("\n"); // new line
    myFile.close();
    Serial.println(F("done."));
  } else {
    // if the file didn't open, print an error:
    Serial.println(F("error opening test.txt"));
  } 
}
// add this function to your sketch
int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
}
