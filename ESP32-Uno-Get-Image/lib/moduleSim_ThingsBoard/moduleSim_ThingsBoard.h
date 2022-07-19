#ifndef moduleSim_ThingsBoard_H
#define moduleSim_ThingsBoard_H

//*******************************************Describe*****************************************
// Connect module Sim
//-----------------------------------------------
// Pin connect
// GPIO_25 (TXD)
// GPIO_26 (RXD)
//-----------------------------------------------
// Send data to PlatformIoT ThingsBoard
//----------------------------------------------
// Include Library
// plerup/EspSoftwareSerial@^6.15.2
// thingsboard/ThingsBoard@^0.6
// knolleary/PubSubClient@^2.8
// vshymanskyy/TinyGSM@^0.11.5
//-----------------------------------------------
//*******************************************END Describe*****************************************

#include <Arduino.h>

void initModuleGSM800();
bool startModuleGSM();
bool checkConnectModuleSim();
bool connectThingsBoardPlatform(const char *thingsBoard_server, const char *token_acc);
void showInformationModuleSim();
void resetModuleSim();
void sendDataToThingsBoardPlatform(String data_json);
void updatedCallback(const bool &success);
void updateFWDevice(const char *fw_title, const char *fw_ver);

#endif