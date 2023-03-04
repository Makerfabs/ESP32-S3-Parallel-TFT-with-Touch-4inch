#include "weather.h"

const char *ntpServer = "120.25.108.11";
// const int timezone = 8;

int wifi_init(String ssid, String pwd)
{
    // WiFi.begin(ssid.c_str(), pwd.c_str());
    WiFi.begin(SSID, PWD);

    int connect_count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(500);
        Serial.print(".");
        connect_count++;
        if (connect_count > 10)
            return 0;
    }

    // configTime((const long)(timezone * 3600), 0, ntpServer);

    return 1;
}

void time_zone_set(int country_num)
{
    int zone_list[6] =
        {
            8,  // china
            3,  // russia
            -5, // usa
            0,  // england
            1,  // france
            1   // german
        };
    configTime((const long)(zone_list[country_num] * 3600), 0, ntpServer);
}

void get_time()
{
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}