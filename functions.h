void initWifi()
{
    Serial.printf("Attempting to connect to SSID: %s.\r\n", ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        uint8_t mac[6];
        WiFi.macAddress(mac);
        Serial.printf("You device with MAC address %02x:%02x:%02x:%02x:%02x:%02x connects to %s failed! Waiting 10 seconds to retry.\r\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ssid);
        WiFi.begin(ssid, pass);
        delay(10000);
    }
    Serial.printf("Connected to wifi %s.\r\n", ssid);
}

time_t initTime()
{
    time_t timeStamp;
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    while (true)
    {
        timeStamp = time(NULL);
        if (timeStamp <= 28812)
        {
            Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        }
        else
        {
            Serial.printf("Fetched NTP epoch time is: %lu.\r\n", timeStamp);
            return timeStamp;
            break;
        }
    }
}
