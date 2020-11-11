#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>

static char *connectionString = "HostName=IoThubprojekt.azure-devices.net;DeviceId=projektFeather;SharedAccessKey=/lNhuaCAg6IvU7EqiugCo/MHulu/vp4zIJfb9VCYduk=";
static char *ssid = "Lembish";
static char *pass = "31anyaoyao";
#include "config.h"
#include "functions.h"

static bool messagePending = false;
static bool messageSending = false;
static int interval = INTERVAL;
static int tempAlert = TEMPERATURE_ALERT;

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

void setup()
{
    pinMode(PHOTO_PIN,INPUT);
    initSerial();
    initWifi();
    initTime();
    initSensor();

    iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    if (iotHubClientHandle == NULL)
    {
        Serial.println("Failed on IoTHubClient_CreateFromConnectionString.");
        while (1);
    }

    IoTHubClient_LL_SetOption(iotHubClientHandle, "product_info", "HappyPath_AdafruitFeatherHuzzah-C");
    
    IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, receiveMessageCallback, NULL);
    IoTHubClient_LL_SetDeviceMethodCallback(iotHubClientHandle, deviceMethodCallback, NULL);
    IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, twinCallback, NULL);
}

static int messageCount = 1;
void loop()
{
    if (!messagePending && messageSending)
    {
        char messagePayload[MESSAGE_MAX_LEN];
        bool temperatureAlert = readMessage(messageCount, messagePayload);
        
        sendMessage(iotHubClientHandle, messagePayload, temperatureAlert);
        
        messageCount++;
        delay(interval);
    }
    IoTHubClient_LL_DoWork(iotHubClientHandle);
    delay(10);
}
