// Only use for non-commercial purposes
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
#define APSSID "Runandgo"
#define APPSK  "djxone123456"
#define startWaterTime 8 //Time water start [integer]
#define endWaterTime 16 //Time water end [integer]
#define timeBTW 2 // After timeBTW hour(s) [integer], water again
#define waterLength 20 //(second) [integer]
#define timeSleep 10 //(second) [integer]
#define DHTPIN D5
#define Relay D7 // Pump
const String Ver = "v1.0.2";
const String dateup = "Mon, June 14,2021";
const char *auth = "bTTEbBILDbMs-Lm_N4TJ5vMtBDnXubBk"; // Paste Blynk token in here
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
  Blynk.begin(auth, APSSID, APPSK, "blynk-cloud.com", 80);

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  
  dht.begin();

  terminal.clear();
  terminal.print("Esp8266 IoT ");
  terminal.println(Ver);
  terminal.print("(c) 2021 runandgo.\n");
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
