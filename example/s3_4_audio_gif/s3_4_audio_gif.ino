

#include <Arduino_GFX_Library.h>
#include <SD.h>

#include "GifClass.h"
#include "Audio.h"

#define GIF_FILENAME "/test1.gif"

// I2S
#define I2S_DOUT 19
#define I2S_BCLK 20
#define I2S_LRCK 46

#define SD_SCK 12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS 10

#define GFX_BL -1
#define TFT_BL GFX_BL

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    1 /* CS */, 12 /* SCK */, 11 /* SDA */,
    45 /* DE */, 4 /* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */,
    39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */,
    0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */,
    6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    true /* IPS */, 480 /* width */, 480 /* height */,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations),
    true /* BGR */);

static GifClass gifClass;
Audio audio;

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);
    Serial.println("Animated GIF Image Viewer");

    // Init Display
    gfx->begin();
    gfx->fillScreen(BLACK);

    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS))
    {
        Serial.println(F("ERROR: File System Mount Failed!"));
        gfx->println(F("ERROR: File System Mount Failed!"));
    }

    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
    audio.setVolume(21);                           // 0...21
    audio.connecttoFS(SD, "/ChildhoodMemory.mp3"); //"/ChildhoodMemory.mp3"

    xTaskCreatePinnedToCore(Task_TFT, "Task_TFT", 20480, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(Task_Audio, "Task_Audio", 10240, NULL, 3, NULL, 1);
}

void loop()
{
}

void Task_TFT(void *pvParameters)
{
    while (1)
    {
        File gifFile = SD.open(GIF_FILENAME, "r");
        if (!gifFile || gifFile.isDirectory())
        {
            Serial.println(F("ERROR: open gifFile Failed!"));
            gfx->println(F("ERROR: open gifFile Failed!"));
        }
        else
        {
            // read GIF file header
            gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
            if (!gif)
            {
                Serial.println(F("gd_open_gif() failed!"));
            }
            else
            {
                uint8_t *buf = (uint8_t *)malloc(gif->width * gif->height);
                if (!buf)
                {
                    Serial.println(F("buf malloc failed!"));
                }
                else
                {
                    int16_t x = 8;
                    int16_t y = 8;

                    Serial.println(F("GIF video start"));
                    int32_t t_fstart, t_delay = 0, t_real_delay, delay_until;
                    int32_t res = 1;
                    int32_t duration = 0, remain = 0;
                    while (res > 0)
                    {
                        t_fstart = millis();
                        t_delay = gif->gce.delay * 10;
                        res = gifClass.gd_get_frame(gif, buf);
                        if (res < 0)
                        {
                            Serial.println(F("ERROR: gd_get_frame() failed!"));
                            break;
                        }
                        else if (res > 0)
                        {
                            // for (int i = 0; i < 6; i++)
                            //     for (int j = 0; j < 6; j++)
                            //         gfx->drawIndexedBitmap(x + 80 * i, y + 80 * j, buf, gif->palette->colors, gif->width, gif->height);

                            gfx->drawIndexedBitmap(240, 240, buf, gif->palette->colors, gif->width, gif->height);

                            t_real_delay = t_delay - (millis() - t_fstart);
                            duration += t_delay;
                            remain += t_real_delay;
                            delay_until = millis() + t_real_delay;
                            while (millis() < delay_until)
                            {
                                vTaskDelay(1);
                            }
                        }
                    }
                    Serial.println(F("GIF video end"));
                    Serial.print(F("duration: "));
                    Serial.print(duration);
                    Serial.print(F(", remain: "));
                    Serial.print(remain);
                    Serial.print(F(" ("));
                    Serial.print(100.0 * remain / duration);
                    Serial.println(F("%)"));

                    gifClass.gd_close_gif(gif);
                    free(buf);
                }
            }
        }
    }
}

void Task_Audio(void *pvParameters) // This is a task.
{
    while (1)
        audio.loop();
}