// Only use for non-commercial purposes
//If you create the virtual pins on Blynk
//To synchronize with this code, change the virtual pins as follow:
//On-board value    Blynk pin      Range
// Temperature          V0         0-50*C
// Humidity             V1         0-100%
// Heat index           V2         -10-60*C
// Terminal             V3         <No need>
//////////////////////////////////////Library//////////////////////////////////////////////////////////
#define BLYNK_PRINT Serial
  
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <DHT.h>
#include <SimpleTimer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPtimeESP.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Setup///////////////////////////////////////////////////////////
#define APSSID "djxone123456"
#define APPSK  "runandgo.xyz"
#define startWaterTime 8 //Time water start [integer]
#define endWaterTime 16 //Time water end [integer]
#define timeBTW 2 // After timeBTW hour(s) [integer], water again
#define waterLength 20 //(second) [integer]
#define timeSleep 10 //(second) [integer]
#define DHTPIN D5
#define Relay D7 // Pump
const String Ver = "v1.0.3";
const String dateup = "Sat, April 1,2023";

#define BLYNK_TEMPLATE_ID "TMPK6J0wmBkSf"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "Ab2axLrAcGf3ya_fW0phaU1nbp6TkMB_L" // Paste Blynk token in here
//////////////////////////////////////////////////////////////////////////////////////////////////////////
ESP8266WiFiMulti WiFiMulti;
WidgetTerminal terminal(V3);
SimpleTimer timer;
strDateTime dateTime;

#define DHTTYPE DHT11   // DHT 11
bool processing = 0;

#include "RTC_setting.h"
#include "ota_firmware.h"
#include "Blynkprocess.h"

//////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  // Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);
  WiFi.begin(APSSID, APPSK);
  WiFiMulti.addAP(APSSID, APPSK);
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, APSSID, APPSK);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  
  dht.begin();

  terminal.clear();
  terminal.print("Esp8266 IoT ");
  terminal.println(Ver);
  terminal.print("(c) 2023 runandgo.\n");
  terminal.print("Connected.");
  terminal.flush();

  timer.setInterval(5000L, sendUptime); // check Wifi connection after 5s
}

void sendUptime()
{   
  if(processing) return;
  
  if (WiFi.status() != WL_CONNECTED) //If no wifi connection, deep sleep on timeSleep seconds.
  {
    ESP.deepSleep(timeSleep * (long)1e6);
  }
    
  UpdateDHT(); //Reads information on DHT sensor and sends to Blynk
  if(getTimeWatering()) AcessWater();// Water
}

void loop() {
  Blynk.run();
  timer.run();  
}
