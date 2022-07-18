#include "Arduino.h"

#include "WifiCam.hpp"
#include <WiFi.h>
#include <esp_task_wdt.h>

//**************************************Define Others********************************************
// Replace with your network credentials
const char *ssid = "ESP32-Cam-Wifi";
const char *password = "123456789";

// 45 seconds WDT
#define WDT_TIMEOUT 45

esp32cam::Resolution initialResolution;

WebServer server(80);

//*************************************Function***************************************************
void initWifiAP(const char *id, const char *pass);

void setup()
{
  // Enable WDT
  esp_task_wdt_init(WDT_TIMEOUT, true); // enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);               // add current thread to WDT watch

  Serial.begin(115200);
  Serial.println("Start ESP32-Cam");

  initWifiAP(ssid, password);

  {
    using namespace esp32cam;

    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok)
    {
      Serial.println("camera initialize failure");
      delay(5000);
      ESP.restart();
    }
    Serial.println("camera initialize success");
  }

  addRequestHandlers();
  server.begin();
}

void loop()
{
  server.handleClient();

  // Reset WDT
  esp_task_wdt_reset();
}

// initWifiAP init wifi Access Point
void initWifiAP(const char *id, const char *pass)
{
    Serial.print("Setting AP (Access Point).");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(id, pass);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}
