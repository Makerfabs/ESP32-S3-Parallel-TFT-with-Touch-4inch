# Makerfabs ESP32-S3 4" Kit

```c++
/*
Version:        V1.0
Author:         Vincent
Create Date:    2023/3/14
Note:

*/
```

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

# Makerfabs ESP32-S3 Parallel TFT with Touch 4inch

## Intruduce

Product Link ：[ESP32-S3 Parallel TFT with Touch 4.0" 480*480 ST7701](https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-4-inch.html)

Wiki Link : [ESP32-S3 Parallel 4.0" TFT with Touch](https://wiki.makerfabs.com/ESP32_S3_Parallel_4.0_TFT_with_Touch.html)

# 

# Usage

## SD card prepare

Extract "\ESP32-S3-Parallel-TFT-with-Touch-4inch\kit\sd_file.zip"

Copy all files in sd_file to the SD card, making sure that wifi.txt,mp3,jpg are in the root directory.

The SD card shipped with the factory has already downloaded the necessary files.

Of course you can put your pictures and music on the SD card.

The image should be in.jpg format, preferably 480 by 480 pixels.

Music required format.mp3.

And edit "wifi.txt", change your own WiFi ssid and WiFi password.

```
SSID:[YOUR SSID]
PASSWORD:[YOUR PWD]
# Please change your own wifi setting in "[]".
# WIFI name and password can only contain letters and numbers, the program is not fully compatible with special characters.
```



Insert SD card.



## Music Frame

JPG pictures and MP3 music in the root directory of the SD card are automatically played.



## Global Weather

Click on the flags of different countries to toggle different periods and show the weather in the capital.



## TVOC Monitor

Mabee CO2 needs to be connected to the I2C interface.

Displays TVOC sensor measurements and provides a web page to access.

You need to set up WiFi to use it.

## Thermal Camera

You need to connect Mabee MLX90640 to the I2C interface

Simple thermal imaging camera.





# Code

## Bin Burn

Select ESP32 S3

0x0                    kit.ino.bootloader.bin 
0x8000             kit.ino.partitions.bin 
0xe000             boot_app0.bin 
0x10000           kit.ino.bin 

## Complier Option

Install board : ESP32 2.0.5

Install libraries from Library Manager：

- Using 1.3.1  Version  GFX_Library_for_Arduino

- Using 1.0.2  Version  TAMC_GT911

- Using 6.20.1  Version  ArduinoJson

- Using 1.0.2  Version  Adafruit_MLX90640

- Using 1.14.1  Version  Adafruit_BusIO

- Using 2.0.0  Version  Adafruit_SGP30_Sensor

- Using 2.2.0  Version  Adafruit_SHT31_Library

- Using 1.2.8  Version  JPEGDEC



Install libraries from Github：

- Using 2.0.0  Version  ESP32-audioI2S-master

- Using 1.2.3  Version  ESPAsyncWebServer-masterr 

- Using 1.1.1  Version  AsyncTCP-master



Use type-c use cable connect USB-TTL to PC.

Select "ESP32-S3 DEV Module"

Select PSRAM "OPI PSRAM"




