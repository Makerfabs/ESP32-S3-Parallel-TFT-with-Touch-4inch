#ifndef WEATHER_H
#define WEATHER_H

#include "config.h"

#define SSID "Makerfabs"
#define PWD "20160704"

int wifi_init(String ssid, String pwd);
void time_zone_set(int country_num);
void get_time();

#endif