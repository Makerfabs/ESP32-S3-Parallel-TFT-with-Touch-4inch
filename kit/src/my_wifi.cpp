
#include "my_wifi.h"

String ssid = "NULL";
String pwd = "";

int wifi_init()
{
    WiFi.begin(ssid.c_str(), pwd.c_str());
    // WiFi.begin(SSID, PWD);

    int connect_count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(500);
        Serial.print(".");
        connect_count++;
        if (connect_count > 10)
            return 0;
    }

    return 1;
}

int wifi_config_read()
{
    File my_file = SD.open("/wifi.txt", FILE_READ);
    String temp = "";
    char c = 0;
    int i = 0;

    if (my_file != NULL)
    {
        while (my_file.available())
        {
            c = my_file.read();
            Serial.write(c);

            if (c == '\n')
            {
                if (i == 0)
                {
                    ssid = cut_msg(temp);
                    temp = "";
                    i++;
                }
                else if (i == 1)
                {
                    pwd = cut_msg(temp);
                    my_file.close();

                    Serial.println(ssid);
                    Serial.println(pwd);
                    return 1;
                }
            }

            temp += c;
        }
        my_file.close();
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error opening wifi.txt");
        return 0;
    }
    return 1;
}

String cut_msg(String input)
{
    int a = -1;
    int b = -1;

    a = input.indexOf('[') + 1;
    b = input.indexOf(']');

    return input.substring(a, b);
}