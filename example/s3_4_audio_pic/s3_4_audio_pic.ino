#include <SPI.h>
#include <FS.h>
#include <Wire.h>
#include <SD.h>

#include "Audio.h"
#include "Arduino_GFX_Library.h"
#include "JpegFunc.h"

// microSD card
#define SD_SCK 12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS 10

// I2S
#define I2S_DOUT 19
#define I2S_BCLK 20
#define I2S_LRCK 46

// TFT
#define TFT_BL -1

#define AUDIO_FILENAME_01 "/ChildhoodMemory.mp3"
#define AUDIO_FILENAME_02 "/SoundofSilence.mp3"
#define AUDIO_FILENAME_03 "/MoonlightBay.mp3"

#define IMAGE_COUNT 7

// String image_list[IMAGE_COUNT] = {
//     "/1.jpg",
//     "/2.jpg",
//     "/3.jpg",
//     "/4.jpg",
//     "/5.jpg",
//     "/6.jpg",
//     "/7.jpg",
//     "/8.jpg",
//     "/9.jpg",
//     "/10.jpg",
//     "/11.jpg",
//     "/12.jpg",
//     "/13.jpg",
//     "/14.jpg",
//     "/15.jpg",
//     "/16.jpg",
//     "/17.jpg",
//     "/18.jpg"};

String image_list[IMAGE_COUNT] = {

    "/image01.jpg",
    "/image02.jpg",
    "/image03.jpg",
    "/image04.jpg",
    "/image05.jpg",
    "/image06.jpg",
    "/image07.jpg"

};

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(1 /* CS */, 12 /* SCK */, 11 /* SDA */, 45 /* DE */, 4 /* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */, 39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */, 0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */, 6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    true /* IPS */, 480 /* width */, 480 /* height */,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations),
    true /* BGR */);

Audio audio;

int image_index = 0;
long runtime = 0;

void setup()
{

    Serial.begin(115200);

    gfx->begin();
    gfx->fillScreen(WHITE);

    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS))
    {
        Serial.println(F("ERROR: SD Mount Failed!"));
    }

    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
    audio.setVolume(21); // 0...21
    // audio.setBufsize(0,1000000);
    audio.connecttoFS(SD, AUDIO_FILENAME_01); //"/ChildhoodMemory.mp3"

    xTaskCreatePinnedToCore(Task_TFT, "Task_TFT", 20480, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(Task_Audio, "Task_Audio", 10240, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
}

void loop()
{
}

static int jpegDrawCallback(JPEGDRAW *pDraw)
{
    // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}

void Task_TFT(void *pvParameters) // This is a task.
{
    while (1) // A Task shall never return or exit.
    {
        Serial.println(image_list[image_index % IMAGE_COUNT]);
        jpegDraw(image_list[image_index++ % IMAGE_COUNT].c_str(), jpegDrawCallback, true, 0, 0, gfx->width(), gfx->height());

        vTaskDelay(2000);
    }
}

void Task_Audio(void *pvParameters) // This is a task.
{
    while (1)
        audio.loop();
}

void audio_eof_mp3(const char *info)
{ // end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);

    audio.connecttoFS(SD, AUDIO_FILENAME_01); //"/ChildhoodMemory.mp3"
}
