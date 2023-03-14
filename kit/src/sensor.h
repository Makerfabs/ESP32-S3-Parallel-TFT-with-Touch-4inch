#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"
#include "page.h"

extern String ssid;
extern String pwd;

extern Arduino_ST7701_RGBPanel *gfx;

void sensor_init();
void sensor_display();
void getSGP30Readings();
uint32_t getAbsoluteHumidity(float temperature, float humidity);
String processor(const String &var);
void tvoc_task();

#endif