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




