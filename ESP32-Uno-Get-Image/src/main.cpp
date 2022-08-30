#include <Arduino.h>

#include <esp_task_wdt.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SPIFFS.h"

//*******************************************************Wifi****************************************************
// Replace with your network credentials
const char *ssid = "ESP32-Cam-Wifi";
const char *password = "123456789";

// Your IP address or domain name with URL path
const char *serverNameImage = "http://192.168.4.1/640x480.jpg";

// location save file
String FILE_PHOTO = "/image/photo.jpg";

//*************************************************Defines Value*********************************************
// 45 seconds WDT
#define WDT_TIMEOUT 45

// define baud rate port
#define BAUD_RATE_Debug 115200

//**************************************************Functions***************************************************
void initConnectWifi(const char *id, const char *pass);
void initSPIFFSToSaveImage();
void listDir(String dir);
void removeLogFile(String log_name);
void downloadAndSaveFile(String fileName, String url);

void setup()
{
    // Enable WDT
    esp_task_wdt_init(WDT_TIMEOUT, true); // enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               // add current thread to WDT watch

    // init serial
    Serial.begin(BAUD_RATE_Debug);

    // init connect wifi
    initConnectWifi(ssid, password);

    // init SPIFFS
    initSPIFFSToSaveImage();

    // download image and save to flash SPIFFS
    downloadAndSaveFile(FILE_PHOTO, serverNameImage);
}

void loop()
{
    // Reset WDT
    esp_task_wdt_reset();
}

// initConnectWifi init connect Wifi
void initConnectWifi(const char *id, const char *pass)
{
    int count_time = 0;

    // Connect to Wi-Fi network with SSID and password
    WiFi.mode(WIFI_STA);
    WiFi.begin(id, pass);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");

        // check time connect wifi. If not connect wifi after 10s -> return
        count_time++;
        if (count_time >= 20)
        {
            Serial.println("Connect Wifi Fail.");
            return;
        }
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(id);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

// initSPIFFSToSaveImage init SPIFFS to save Image
void initSPIFFSToSaveImage()
{
    long file_size;

    // init SPIFFS on ESP32
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        ESP.restart();
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
    if (file_size >= 500000)
    {
        // del file if file_size >= 500KB
        Serial.println("Remove file.");
        removeLogFile(FILE_PHOTO);
    }
}

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

// removeLogFile remove log file
void removeLogFile(String log_name)
{
    SPIFFS.remove(log_name);
}

//downloadAndSaveFile download and save file to SPIFFS
void downloadAndSaveFile(String fileName, String url)
{
    HTTPClient http;

    Serial.println("[HTTP] begin...\n");
    Serial.println(fileName);
    Serial.println(url);
    http.begin(url);

    Serial.printf("[HTTP] GET...\n", url.c_str());
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\r\n", httpCode);

        File file = SPIFFS.open(fileName, FILE_WRITE);
        if (!file)
        {
            Serial.println("Failed to open file in writing mode");
            Serial.println("Maybe not enough space");
            return;
        }

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            // get lenght of document (is -1 when Server sends no Content-Length header)
            int len = http.getSize();

            // create buffer for read
            uint8_t buff[128] = {0};

            // get tcp stream
            WiFiClient *stream = http.getStreamPtr();

            // read all data from server
            while (http.connected() && (len > 0 || len == -1))
            {
                // get available data size
                size_t size = stream->available();
                if (size)
                {
                    // read up to 128 byte
                    int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                    // write it to Serial
                    // Serial.write(buff, c);
                    file.write(buff, c);
                    if (len > 0)
                    {
                        len -= c;
                    }
                }
                delay(1);
            }

            Serial.println();
            Serial.println("[HTTP] connection closed or file end.\n");
            Serial.println("[FILE] closing file\n");
            file.close();
        }
    }
    http.end();
}