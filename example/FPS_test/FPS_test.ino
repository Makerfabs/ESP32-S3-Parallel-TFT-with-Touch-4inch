#include "Arduino_GFX_Library.h"

#define GFX_BL -1

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

void setup()
{
    Serial.begin(115200);
    gfx->begin();

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    gfx->fillScreen(BLACK);
}

static int colors[] = {RED, GREEN, BLUE, WHITE, YELLOW};

int i = 0;

void loop(void)
{

    // fps
    static int prev_sec;
    static int fps;
    ++fps;
    int sec = millis() / 1000;
    if (prev_sec != sec)
    {
        prev_sec = sec;
        gfx->setCursor(0, 470);
        gfx->fillRect(0, 470, 100, 10, BLACK);
        gfx->printf("fps:%03d", fps);
        fps = 0;
    }

    gfx->fillRect(0, 0, 480, 470, colors[i++]);
    if (i > 5)
        i = 0;
}