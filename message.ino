#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <DHT.h>


static DHT dht(DHT_PIN, DHT_TYPE);

void initSensor()
{
    dht.begin();
}
bool readMessage(int messageId, char *payload)
{
  float inTemp = dht.readTemperature();
  float inHum = dht.readHumidity();
  time_t timeStamp = initTime();
  //unsigned long timeStamp = initTime();
  bool temperatureAlert = false;

  StaticJsonDocument<MESSAGE_MAX_LEN> root;
  //root["deviceId"] = DEVICE_ID;
  //root["messageCount"] = messageId;
  root["temperature"] = inTemp;
  root["humidity"] = inHum;
  root["epochtime"] = timeStamp;

  if(root["temperature"] < TEMPERATURE_ALERT){
    temperatureAlert = true;
  }
  else temperatureAlert = false;

  serializeJson(root,payload,MESSAGE_MAX_LEN);
  return temperatureAlert;
}
void parseTwinMessage(char *message)
{
      StaticJsonDocument<MESSAGE_MAX_LEN> root;
      DeserializationError err = deserializeJson(root, message);
    
    if (err)
    {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }
      
}
