#include "weather.h"

const char *ntpServer = "120.25.108.11";
// const int timezone = 8;

int zone_list[B_COUNTY_NUM] =
    {
        8,  // china
        3,  // russia
        -5, // usa
        0,  // england
        1,  // france
        10, // Australia
        -3, // Brazil
        1   // german
};

String city_list[B_COUNTY_NUM] =
    {
        "Beijing",    // china
        "Moscow",     // russia
        "Washington", // usa
        "London",     // england
        "Paris",      // france
        "Canberra",   // Australia
        "Brazil",   // Brazil
        "Berlin"     // german
};

String flag_list[B_COUNTY_NUM] =
    {
        "/flag/china.jpg",
        "/flag/russia.jpg",
        "/flag/usa.jpg",
        "/flag/england.jpg",
        "/flag/france.jpg",
        "/flag/australia.jpg",
        "/flag/brazil.jpg",
        "/flag/german.jpg"};

String wind_txt[] =
    {"north", "northeast", "east", "southeast",
     "south", "southwest", "west", "northwest"};

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

String weather_request(int country_num)
{

    HTTPClient http;
    String text = "";

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url

    // persional api,please change to yourself
    // bool begin(String url);
    String url = "https://free-api.heweather.net/s6/weather/now?location=" + city_list[country_num] + "&key=2d63e6d9a95c4e8f8d3f65d0b5bcdf7f&lang=en";
    http.begin(url);

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);

            // JSON
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            JsonObject obj = doc.as<JsonObject>();

            String cond_num = doc["HeWeather6"][0]["now"]["cond_code"];
            String cond_txt = doc["HeWeather6"][0]["now"]["cond_txt"];
            String tmp = doc["HeWeather6"][0]["now"]["tmp"];
            String hum = doc["HeWeather6"][0]["now"]["hum"];
            int wind_deg = doc["HeWeather6"][0]["now"]["wind_deg"];

            text = city_list[country_num] + cond_txt + ", " + tmp + " centigrade, " + wind_txt[wind_deg / 45] + " wind,relative humidity " + hum + " percent.";
            Serial.println(text);
        }
    }
    else
    {
        text = "Request Fail";
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    return text;
}

void weather_request2(int country_num, float *temperature, float *humidty, String *weather, int *weather_num)
{
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");

    String url = "https://free-api.heweather.net/s6/weather/now?location=" + city_list[country_num] + "&key=2d63e6d9a95c4e8f8d3f65d0b5bcdf7f&lang=en";
    http.begin(url);

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);

            // JSON
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            JsonObject obj = doc.as<JsonObject>();

            String cond_num = doc["HeWeather6"][0]["now"]["cond_code"];
            String cond_txt = doc["HeWeather6"][0]["now"]["cond_txt"];
            String tmp = doc["HeWeather6"][0]["now"]["tmp"];
            String hum = doc["HeWeather6"][0]["now"]["hum"];

            *temperature = tmp.toFloat();
            *humidty = hum.toFloat();
            *weather = cond_txt;
            *weather_num = cond_num.toInt();
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}