#include <Arduino.h>

// Select your modem:
// #define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM868
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_M95
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_MC60
// #define TINY_GSM_MODEM_MC60E
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

//***************************************Libraries***********************************************
#include "moduleSim_ThingsBoard.h"
#include <SoftwareSerial.h>
#include <TinyGsmClient.h>
#include "ThingsBoard.h"

//**************************************Define Module GSM********************************************
// Define Pin
#define RX_Sim_pin (26) //(D26)
#define TX_Sim_pin (25) //(D25)

// Software Serial
SoftwareSerial Serial_ModuleSim(RX_Sim_pin, TX_Sim_pin);

// Define baudrate for module GSM
static const uint32_t GSMBaud = 9600;

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[] = "v-internet";
const char user[] = "";
const char pass[] = "";

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";

// Initialize GSM modem
TinyGsm modem(Serial_ModuleSim);

// Initialize GSM client
TinyGsmClient client(modem);

//**********************************************ThingsBoard****************************************************
// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token

int count_disconnect_platformIoT;

// Initialize ThingsBoard instance
ThingsBoard tb(client);

//***************************************Functions************************************************
// initModuleGSM800 init module GSM sim800
void initModuleGSM800()
{
    Serial_ModuleSim.begin(115200);
    // Lower baud rate of the modem.
    // This is highly practical for Uno board, since SoftwareSerial there
    // works too slow to receive a modem data.
    Serial_ModuleSim.write("AT+IPR=9600\r\n");
    Serial_ModuleSim.end();
    Serial_ModuleSim.begin(GSMBaud);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println(F("Initializing modem..."));
    modem.restart();

    // Unlock your SIM card with a PIN if needed
    if (strlen(simPIN) && modem.getSimStatus() != 3)
    {
        modem.simUnlock(simPIN);
    }

    String modemInfo = modem.getModemInfo();
    Serial.print(F("Modem: "));
    Serial.println(modemInfo);
}

// startModuleGSM start module GSM
bool startModuleGSM()
{
    resetModuleSim();
    Serial.print(F("Waiting for network..."));
    if (!modem.waitForNetwork(10000))
    {
        Serial.println(" fail");
        return false;
    }
    Serial.println(" OK");

    Serial.print(F("Connecting to "));
    Serial.print(apn);

    if (!modem.gprsConnect(apn, user, pass))
    {
        Serial.println(" fail");
        return false;
    }

    Serial.println(" OK");
    return true;
}

// checkConnectModuleSim check connect of module sim.
//  Return true -> Connect success
//  return false -> Connect fail
bool checkConnectModuleSim()
{
    // check module sim800 connect network
    if (modem.isNetworkConnected())
    {
        Serial.println("Network connected");

        if (count_disconnect_platformIoT == 3)
        {
            // restart module sim
            count_disconnect_platformIoT = 0;
            bool status_connect = startModuleGSM();
            if (!status_connect)
            {
                return false;
            }
        }
    }
    else
    {
        // start and connect to GPRS
        bool status_connect = startModuleGSM();
        if (!status_connect)
        {
            return false;
        }
    }
    return true;
}

// connectThingsBoardPlatform connect ThingsBoard Platform
bool connectThingsBoardPlatform(const char *thingsBoard_server, const char *token_acc)
{
    // check status module sim
    bool status_module_sim;
    status_module_sim = checkConnectModuleSim();
    if (!status_module_sim)
    {
        return false;
    }

    // check connect ThingsBoard
    if (!tb.connected())
    {
        // Connect to the ThingsBoard
        Serial.print("Connecting to: ");
        Serial.print(thingsBoard_server);
        Serial.print(" with token ");
        Serial.println(token_acc);
        if (!tb.connect(thingsBoard_server, token_acc))
        {
            Serial.println("Failed to connect ThingsBoard");
            count_disconnect_platformIoT++;
            return false;
        }
    }
    Serial.println("Connected Success to ThingsBoard PlatformIoT");

    return true;
}

// showInformationModuleSim show information module sim
void showInformationModuleSim()
{
    Serial.print("Operator: ");
    Serial.println(modem.getOperator());

    Serial.print("IMEI module Sim: ");
    Serial.println(modem.getIMEI());

    Serial.print("Strength Signal: ");
    Serial.println(modem.getSignalQuality());

    Serial.print("Sim Number: ");
    Serial.println(modem.getSimCCID());
}

// resetModuleSim reset module sim use command: AT+CFUN=1,1
void resetModuleSim()
{
    Serial.println("Send command reset module sim");
    modem.sendAT("+CFUN=1,1");
}

// sendDataToThingsBoardPlatform send data (telemetry json string) to ThingsBoard Platform
void sendDataToThingsBoardPlatform(String data_json)
{
    Serial.println("Sending data...");

    tb.sendTelemetryJson(data_json.c_str());
    tb.loop();
}

void updatedCallback(const bool &success)
{
    if (success)
    {
        Serial.println("Done, Reboot now");
#if defined(ESP8266)
        ESP.restart();
#elif defined(ESP32)
        esp_restart();
#endif
    }
    else
    {
        Serial.println("No new firmware");
    }
}

void updateFWDevice(const char *fw_title, const char *fw_ver)
{
    bool is_update_success;

    Serial.println("Firmware Update...");
    Serial.println(tb.Firmware_OTA_Subscribe());

    // Serial.println(tb.Firmware_Send_FW_Info(fw_title, fw_ver));
    Serial.println(tb.Firmware_Send_State(fw_ver));

    is_update_success = tb.Firmware_Update(fw_title, fw_ver);

    updatedCallback(is_update_success);

    tb.Firmware_OTA_Unsubscribe();

    tb.loop();
}