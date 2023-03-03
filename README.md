# Makerfabs ESP32-S3 Parallel TFT with Touch 4inch

```c++
/*
Version:        V1.1
Author:            Vincent
Create Date:    2023/2/4
Note:
        V1.2：Now support LovyanGFX！
        V1.1: Add example and update readme.
*/
```

[TOC]

![main](md_pic/main.jpg)

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

# Makerfabs ESP32-S3 Parallel TFT with Touch 4inch

## Intruduce

Product Link ：[ESP32-S3 Parallel TFT with Touch 4.0" 480*480 ST7701](https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-4-inch.html)

Wiki Link : [ESP32-S3 Parallel 4.0" TFT with Touch](https://wiki.makerfabs.com/ESP32_S3_Parallel_4.0_TFT_with_Touch.html)

The Latest ESP32-S3 4.0 inch display with 480*480 resolution, it integrated display/ touch/ audio speaker/ Lipo charger, to make this product ideal for applications such as IoT controlling, electronic photo frame, etc.

## Feature

- 4 inch RGB IPS display, it communicates with the controller ESP32S3 with 3SPI+RGB5/6/5 interface, with FPS >80;
- 5 points Touch;
- Onboard 16G SD card for storage;
- Lipo battery socket and charger, support any 3.7v lipo battery;
- On board speaker connector. This product is shipped with a 3 ohm 4w, 3528 speaker, the voice loud and clean, suitable for most applications;
- 2x Onboard Mabee connectors, support multiple Mabee/ Grove modules;

![main](md_pic/back.jpg)

# Code

## Complier Option

- Install board : ESP32 .
- Install libraries：

[ESP32-audioI2S]([GitHub - schreibfaul1/ESP32-audioI2S: Play mp3 files from SD via I2S](https://github.com/schreibfaul1/ESP32-audioI2S))

![library](md_pic/library.jpg)

- Use type-c use cable connect USB-TTL to PC.
- Select "ESP32-S3 DEV Module"
- Select PSRAM "OPI PSRAM"

"mf_Lvgl" library is modified from Lvgl library (version 8.3.2). 

Please unzip "lib/mf_Lvgl.zip" and copy to arduino library directory.

## Factory test program

\example\esp32s3_4.0_tft_fw_test

You can run this file test if you feel that your hardware is not working properly. 

In addition, it is necessary to decompress the compressed package in the sd_file folder and copy all the pictures and music in it to the TF card.