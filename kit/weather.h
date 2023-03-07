#ifndef WEATHER_H
#define WEATHER_H

#include "config.h"

int wifi_init(String ssid, String pwd);
void time_zone_set(int country_num);
void get_time();
String weather_request(int country_num);
void weather_request2(int country_num, float *temperature, float *humidty, String *weather, int *weather_num);

#endif