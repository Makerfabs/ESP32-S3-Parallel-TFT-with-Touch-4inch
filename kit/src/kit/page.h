#ifndef PAGE_H
#define PAGE_H

#include "config.h"
#include "Button.h"
#include "music.h"
#include "image.h"
#include "weather.h"
#include "my_wifi.h"
#include "cam.h"
#include "sensor.h"

#define COLOR_BACKGROUND 0xEF9E
#define COLOR_BUTTON WHITE
#define COLOR_TEXT 0x322B
#define COLOR_LINE BLACK
#define COLOR_SHADOW 0x4BAF

extern Arduino_ST7701_RGBPanel *gfx;
extern Audio audio;

extern int touch_flag;
extern int touch_x;
extern int touch_y;

extern String city_list[B_COUNTY_NUM];
extern int zone_list[B_COUNTY_NUM];
extern String flag_list[B_COUNTY_NUM];

extern String ssid;
extern String pwd;

void page_main();
void page_1();
void page_2();
void page_3();
void page_4();

void time_display(int x, int y, int country_num);
void weather_display(int x, int y, int country_num);

void drawButton(Button b);
void drawFlag(Button b);

#endif