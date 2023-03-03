// #include "weather.h"

// const char *ntpServer = "120.25.108.11";
// const long gmtOffset_sec = (-5) * 60 * 60; // China+8
// const int daylightOffset_sec = 0;
// const int timezone = 8;

// struct tm timeinfo;

// int wifi_init(String ssid, String pwd)
// {
//     // WiFi.begin(ssid.c_str(), pwd.c_str());
//     WiFi.begin(SSID, PWD);

//     int connect_count = 0;
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         vTaskDelay(500);
//         Serial.print(".");
//         connect_count++;
//         if (connect_count > 10)
//             return 0;
//     }

//     configTime((const long)(timezone * 3600), daylightOffset_sec, ntpServer);

//     return 1;
// }

// void get_time()
// {
//     if (!getLocalTime(&timeinfo))
//     {
//         Serial.println("Failed to obtain time");
//         return;
//     }
//     Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
// }