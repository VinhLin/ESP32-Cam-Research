#include <Arduino.h>

// #define _TASK_TIMECRITICAL      // Enable monitoring scheduling overruns
#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
//#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
// #define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
// #define _TASK_LTS_POINTER       // Compile with support for local task storage pointer
// #define _TASK_PRIORITY          // Support for layered scheduling priority
// #define _TASK_MICRO_RES         // Support for microsecond resolution
// #define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 ONLY)
// #define _TASK_DEBUG             // Make all methods and variables public for debug purposes
// #define _TASK_INLINE         // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
#define _TASK_TIMEOUT

#include <esp_task_wdt.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TaskScheduler.h>
#include "moduleSim_ThingsBoard.h"

//*******************************************************Wifi****************************************************
// Replace with your network credentials
const char *ssid = "ESP32-Cam-Wifi";
const char *password = "123456789";

// Your IP address or domain name with URL path
const char *serverNameImage = "http://192.168.4.1/640x480.jpg";

// count disconnect wifi
int count_disconnect;

//*************************************************Defines Value*********************************************
// 45 seconds WDT
#define WDT_TIMEOUT 45

// define baud rate port
#define BAUD_RATE_Debug 115200

// define account ThingsBoard
#define THINGSBOARD_SERVER "thingsboard.cloud"
#define TOKEN "a2NqWYATG2uTSBuz3ZPD"

//**************************************************Functions***************************************************
void initConnectWifi(const char *id, const char *pass);
String httpGETRequest(const char *serverName);
String getImage();
void sendJsonDataAPICallback();

//**************************************************Tasks*******************************************************
Scheduler ts;
// Sample code: https://github.com/arkhipenko/TaskScheduler/blob/master/examples/Scheduler_example02/Scheduler_example02.ino
// https://github.com/arkhipenko/TaskScheduler/blob/master/examples/Scheduler_example17_Timeout/Scheduler_example17_Timeout.ino
Task taskSendImageData(45 * TASK_SECOND, TASK_FOREVER, &sendJsonDataAPICallback, &ts, true);

void setup()
{
    // Enable WDT
    esp_task_wdt_init(WDT_TIMEOUT, true); // enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               // add current thread to WDT watch

    // init serial
    Serial.begin(BAUD_RATE_Debug);

    // init connect wifi
    initConnectWifi(ssid, password);

    // set point-in-time for scheduling start
    ts.startNow();
}

void loop()
{
    // init TaskScheduler
    ts.execute();

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

// httpGETRequest http GET Request to web server
String httpGETRequest(const char *serverName)
{
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Send HTTP POST request
    int httpResponseCode = http.GET();

    String payload = "";

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
}

String getImage()
{
    String image_data;

    if (WiFi.status() == WL_CONNECTED)
    {
        // Get data
        image_data = httpGETRequest(serverNameImage);

        // debug
        Serial.printf("image_data: %s\r\n", image_data.c_str());
    }
    else
    {
        // count disconnect wifi
        count_disconnect++;
        Serial.printf("Disconnect Wifi Access Point %d time.\r\n", count_disconnect);
    }

    return image_data;
}

// sendJsonDataAPICallback send json data API Callback
void sendJsonDataAPICallback()
{
    String data;
    // get data image
    data = getImage();

    if (count_disconnect == 3)
    {
        // reconnect wifi
        initConnectWifi(ssid, password);
        count_disconnect = 0;
    }

    // send data to ThingsBoard
    if (data != "")
    {
        if (!connectThingsBoardPlatform(THINGSBOARD_SERVER, TOKEN))
        {
            showInformationModuleSim();
            return;
        }

        sendDataToThingsBoardPlatform(data);
    }
}