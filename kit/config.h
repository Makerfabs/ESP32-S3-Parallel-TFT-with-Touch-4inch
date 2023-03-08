#ifndef MF_CONFIG_H
#define MF_CONFIG_H

#include <SPI.h>
#include <FS.h>
#include <Wire.h>
#include <SD.h>
#include <Audio.h>
#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Adafruit_MLX90640.h>

#include "JpegFunc.h"

// SD
#define SD_SCK 12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS 10

// I2S
#define I2S_DOUT 19
#define I2S_BCLK 20
#define I2S_LRCK 46

// Touch
#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_INT -1
#define TOUCH_RST 38

#define TOUCH_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

#define B_NUM_MAIN 4
#define B_COUNTY_NUM 8

#define SSID "Makerfabs"
#define PWD "20160704"


// MLX90640
#define MLX_I2C_ADDR 0x33

#endif