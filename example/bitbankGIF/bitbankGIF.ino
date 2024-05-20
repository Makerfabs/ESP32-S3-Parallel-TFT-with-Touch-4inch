// Needs Larry Bank library: AnimatedGIF
#include <Arduino_GFX_Library.h>
#include <SD.h>
#include <AnimatedGIF.h>
#define GIF_FILENAME "/test1.gif"

// SD PINS
#define SD_SCK 12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS 10

#define GFX_BL -1
#define TFT_BL GFX_BL
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 480
#define DRAW_CENTERED_IMAGE true
uint16_t offsetX = 0;
uint16_t offsetY = 0;

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    1 /* CS */, 12 /* SCK */, 11 /* SDA */,
    45 /* DE */, 4 /* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */,
    39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */,
    0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */,
    6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    true /* IPS */, DISPLAY_WIDTH, DISPLAY_HEIGHT,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations),
    true /* BGR */);

AnimatedGIF gif;
File f;

void * GIFOpenFile(const char *fname, int32_t *pSize)
{
  f = SD.open(fname);
  if (f)
  {
    *pSize = f.size();
    return (void *)&f;
  }
  return NULL;
} /* GIFOpenFile() */

void GIFCloseFile(void *pHandle)
{
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
     f->close();
} /* GIFCloseFile() */

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
       return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();
    return iBytesRead;
} /* GIFReadFile() */

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  return pFile->iPos;
} /* GIFSeekFile() */

void GIFDraw(GIFDRAW *pDraw)
{
  gfx->draw16bitRGBBitmap(pDraw->iX + offsetX, pDraw->iY + pDraw->y + offsetY, (uint16_t *)pDraw->pPixels, pDraw->iWidth, 1);
} /* GIFDraw() */

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);
    Serial.println("Animated GIF Image Viewer");

    // Init Display
    gfx->begin();
    gfx->fillScreen(BLACK);
    gif.begin(LITTLE_ENDIAN_PIXELS);
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS, SPI, 20000000))
    {
        Serial.println(F("ERROR: File System Mount Failed!"));
        gfx->println(F("ERROR: File System Mount Failed!"));
    }
    xTaskCreatePinnedToCore(Task_TFT, "Task_TFT", 20480, NULL, 0, NULL, 0);
}

void loop()
{
}

void Task_TFT(void *pvParameters)
{
  // read the file into PSRAM
  uint8_t *pFile;
  int iSize;
   f = SD.open(GIF_FILENAME);
   if (f) {
      iSize = f.size();
      pFile = (uint8_t *)ps_malloc(iSize);
      f.read(pFile, iSize);
      f.close();
    }

    if (!gif.open(pFile, iSize, GIFDraw))
        {
            Serial.println(F("ERROR: open gifFile Failed!"));
            gfx->println(F("ERROR: open gifFile Failed!"));
            while (1) {};
        }

    uint8_t *buf = (uint8_t *)malloc(gif.getCanvasWidth() * gif.getCanvasHeight());
    while (1)
    {
        #if DRAW_CENTERED_IMAGE
        offsetX = (DISPLAY_WIDTH - gif.getCanvasWidth()) / 2;
        offsetY = (DISPLAY_HEIGHT - gif.getCanvasHeight()) / 2;
        #endif
        if (!buf) {
            Serial.println(F("buf malloc failed!"));
        } else {
            Serial.println(F("GIF video start"));
            gif.setFrameBuf(buf);
            gif.setDrawType(GIF_DRAW_COOKED);
            while (gif.playFrame(true, NULL))
            {
            }
        }
        gif.reset();
    } // while (1)
    free(buf);
    gif.close();
} /* Task_TFT() */