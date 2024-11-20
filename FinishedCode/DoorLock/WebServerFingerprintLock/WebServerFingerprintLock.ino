/***************************************************
Agamemno Fingerprint Lock Project
By: Sami Hasan
This is the Fingerprint Storage/Node.js Server access Code
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
//#include <Wire.h>
RTC_DS3231 rtc;
//char t[32];
HardwareSerial mySerial(1);
int f = 0;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
File myFile;
const char* ssid = "WiFiSSID";
const char* password = "WiFiPassword";

const char* websocket_server_host = "server_ip";
const uint16_t websocket_server_port = 8888;
using namespace websockets;
WebsocketsClient client;
bool isWebSocketConnected;

void onWebSocketEvent(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
    isWebSocketConnected = true;
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
    isWebSocketConnected = false;
  } else if (event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  } else if (event == WebsocketsEvent::GotPong) {
    Serial.println("Got a Pong!");
  }
}


void setup()
{
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  
  connectWiFi();
  connectWebSocket();
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
void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void connectWebSocket() {
  client.onEvent(onWebSocketEvent);
  while (!client.connect(websocket_server_host, websocket_server_port, "/")) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WebSocket connected");
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
    Serial2.write('0');
    return -1;
  }
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  switch(finger.fingerID){
    case(1):
         logData("Sam Entered!");
         Serial2.write('1');
         break;
    case(2):
         logData("Sam Entered!");
         Serial2.write('1');
         break;
    case(3):
         logData("Maya Entered!");
         Serial2.write('3');
         break;
    case(4):
         logData("Maya Entered!");
         Serial2.write('3');
         break;
    case(5):
         logData("Hannah Entered!");
         Serial2.write('4');
         break;
    case(6):
         logData("Hannah Entered!");
         Serial2.write('4');
         break;
    case(7):
         logData("Nick Entered!");
         Serial2.write('2');
         break;
    case(8):
         logData("Nick Entered!");
         Serial2.write('2');
         break;
  }
  return finger.fingerID;
}
void logData(String str) {
  if (isWebSocketConnected) {
    // Construct JSON payload
    DateTime now = rtc.now();
    String payload = "{";
    payload += "\"timestamp\":\"" + String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " ";
    payload += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "\",";
    payload += "\"log\":\"" + str + "\"";
    payload += "}";
    client.send(payload);
    Serial.println("Log sent via WebSocket: " + payload);
  } else {
    Serial.println("WebSocket not connected. Log not sent.");
  }
}
// add this function to your sketch
int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
}
