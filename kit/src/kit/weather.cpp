#include "weather.h"

const char *ntpServer = "120.25.108.11";
// const int timezone = 8;

int zone_list[B_COUNTY_NUM] =
    {
        8,  // china
        9,  // japan
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
        "Tokyo",      // japan
        "Washington", // usa
        "London",     // england
        "Paris",      // france
        "Canberra",   // Australia
        "Brazil",     // Brazil
        "Berlin"      // german
};

String flag_list[B_COUNTY_NUM] =
    {
        "/flag/china.jpg",
        "/flag/japan.jpg",
        "/flag/usa.jpg",
        "/flag/england.jpg",
        "/flag/france.jpg",
        "/flag/australia.jpg",
        "/flag/brazil.jpg",
        "/flag/german.jpg"};

String wind_txt[] =
    {"north", "northeast", "east", "southeast",
     "south", "southwest", "west", "northwest"};

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

    String url = "http://api.weatherapi.com/v1/current.json?key=271578bfbe12438085782536232404&q=" + city_list[country_num] + "&aqi=no";

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

            // int cond_num = doc["current"]["condition"]["code"];
            String icon_num = doc["current"]["condition"]["icon"];
            String cond_txt = doc["current"]["condition"]["text"];
            float tmp = doc["current"]["temp_c"];
            float hum = doc["current"]["humidity"];

            //"//cdn.weatherapi.com/weather/64x64/day/113.png"
            icon_num.replace("//cdn.weatherapi.com/weather/64x64/", "");
            icon_num.replace(".png", "");
            icon_num.replace("day/", "");
            icon_num.replace("night/", "");

            *temperature = tmp;
            *humidty = hum;
            *weather = cond_txt;
            *weather_num = icon_num.toInt();

            String text = city_list[country_num] + cond_txt + ",icon:" + icon_num + ", Temperature" + tmp + " C, humidity " + hum + " %.";
            Serial.println(text);
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}