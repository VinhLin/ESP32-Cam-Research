#include <Arduino.h>

//***************************************Libraries***********************************************
#include "SPIFFS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

//**************************************Define Others********************************************
// Replace with your network credentials
const char *ssid = "WifiAP_Image";
const char *password = "123456789";

// storage path
String FILE_PHOTO = "/image/photo.jpg";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// listDir list dir in SPIFFS
//  listDir("/") -> list all dir in SPIFFS
void listDir(String dir)
{
    File root = SPIFFS.open(dir);

    File file = root.openNextFile();

    while (file)
    {
        Serial.print("FILE: ");
        Serial.println(file.name());

        file = root.openNextFile();
    }
}

// initSPIFFS init, check, make and read file log
void initSPIFFS()
{
    long file_size;

    // init SPIFFS on ESP32
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    Serial.print("Total Bytes of SPIFFS: ");
    Serial.println(SPIFFS.totalBytes());

    // list all dir in SPIFFS
    listDir("/");

    // check file size
    file_size = SPIFFS.usedBytes();
    Serial.print("File Size: ");
    Serial.println(file_size);
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

// initWebServer
void initWebServer()
{
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello, world"); });

    server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, FILE_PHOTO, "image/jpeg", true); });

    // Start server
    server.begin();
}

// logDeviceFunction init all function log of ESP32 device
void logDeviceFunction()
{
    // init SPIFFS on ESP32
    initSPIFFS();

    // init Wifi Access Point
    initWifiAP(ssid, password);

    // init web server
    initWebServer();
}

void setup()
{
    // init serial
    Serial.begin(115200);

    logDeviceFunction();
}

void loop()
{
}