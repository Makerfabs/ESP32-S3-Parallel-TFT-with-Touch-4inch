

//for board v1.2


#include <Arduino_GFX_Library.h>
#include <SD.h>
#include <TAMC_GT911.h>
#include <Wire.h>
#include "Audio.h"
#include "SPI.h"
#include "FS.h"

//jpeg
#include "JpegFunc.h"

#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18

#define JPEG_FILENAME_LOGO "/logo.jpg"
#define JPEG_FILENAME_COVER "/cover.jpg"
#define JPEG_FILENAME_COVER_01 "/cover01.jpg"

#define JPEG_FILENAME_01 "/image01.jpg"
#define JPEG_FILENAME_02 "/image02.jpg"
#define JPEG_FILENAME_03 "/image03.jpg"
#define JPEG_FILENAME_04 "/image04.jpg"
#define JPEG_FILENAME_05 "/image05.jpg"
#define JPEG_FILENAME_06 "/image06.jpg"
#define JPEG_FILENAME_07 "/image07.jpg"

//microSD card
#define SD_SCK  12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS   10

#define AUDIO_FILENAME_01   "/ChildhoodMemory.mp3"

#define AUDIO_FILENAME_02   "/SoundofSilence.mp3"

#define AUDIO_FILENAME_03   "/MoonlightBay.mp3"

//I2S
#define I2S_DOUT      19
#define I2S_BCLK      20
#define I2S_LRCK      46

Audio audio;

#define TOUCH_INT -1
#define TOUCH_RST 38

#define TOUCH_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

#define GFX_BL -1
#define TFT_BL GFX_BL

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
  1 /* CS */, 12 /* SCK */, 11 /* SDA */,
  45 /* DE */, 4/* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */,
  39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */,
  0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */,
  6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
  bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
  true /* IPS */, 480 /* width */, 480 /* height */,
  st7701_type1_init_operations, sizeof(st7701_type1_init_operations),
  true /* BGR */);


int touch_last_x = 0, touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

int ColorArray[] = {BLACK, BLUE, GREEN, WHITE, RED, ORANGE, NAVY, DARKGREEN, DARKCYAN, MAROON, PURPLE, OLIVE, LIGHTGREY, DARKCYAN, DARKGREY, MAGENTA, YELLOW, GREENYELLOW, PINK};
//int ColorArray[]={BLACK,BLUE,GREEN,WHITE,RED,ORANGE};


void touch_init(void)
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  ts.begin();
  ts.setRotation(TOUCH_ROTATION);
}

bool touch_touched(void)
{
  ts.read();
  if (ts.isTouched) {
    for (int i = 0; i < ts.touches; i++) {
    touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, 480 - 1);
    touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, 480 - 1);

      Serial.print("Touch "); Serial.print(i + 1); Serial.print(": ");;
      Serial.print("  x: "); Serial.print(ts.points[i].x);
      Serial.print("  y: "); Serial.print(ts.points[i].y);
      Serial.print("  size: "); Serial.println(ts.points[i].size);
      Serial.println(' ');
      break;
    }
    ts.isTouched = false;
    return true;
  }
  else
  {
    return false;
  }
}

// pixel drawing callback
static int jpegDrawCallback(JPEGDRAW *pDraw)
{
  // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
  return 1;
}

void imageDisplay(int number)
{
  unsigned long  start=0;

  #if 0
  int w = gfx->width();
  int h = gfx->height();

  unsigned long start = millis();
  jpegDraw(JPEG_FILENAME_01, jpegDrawCallback, true /* useBigEndian */,
           random(w * 2) - w /* x */,
           random(h * 2) - h /* y */,
           w /* widthLimit */, h /* heightLimit */);
  Serial.printf("Time used: %lums\n", millis() - start);
  delay(1000);
  #endif

  switch(number)
  {
    case 0:
    case 1:
      start = millis();
      jpegDraw(JPEG_FILENAME_01, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
      Serial.printf("Time used: %lums\n", millis() - start);

      break;
    case 2:
      start = millis();
      jpegDraw(JPEG_FILENAME_02, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
      Serial.printf("Time used: %lums\n", millis() - start);
      
      break;
      
    case 3:
      start = millis();
      jpegDraw(JPEG_FILENAME_03, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
      Serial.printf("Time used: %lums\n", millis() - start);
      
      break;

    case 4:
      start = millis();
      jpegDraw(JPEG_FILENAME_04, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
      Serial.printf("Time used: %lums\n", millis() - start);
      
      break;
   
    case 5:
      start = millis();
      jpegDraw(JPEG_FILENAME_05, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
      Serial.printf("Time used: %lums\n", millis() - start);
      
      break;

    case 6:
      start = millis();
      jpegDraw(JPEG_FILENAME_06, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
      Serial.printf("Time used: %lums\n", millis() - start);
      
      break;

    default:
      break;  
  }
  return;
}

void setup()
{

  Serial.begin(115200);

//  pinMode(TFT_BL, OUTPUT);
//  digitalWrite(TFT_BL, HIGH);

  pinMode(TOUCH_RST, OUTPUT);
  delay(100);
  digitalWrite(TOUCH_RST, LOW);
  delay(1000);
  digitalWrite(TOUCH_RST, HIGH);
  delay(1000);
  
  //while (!Serial);
  Serial.println("JPEG Image Viewer V1.4");

  digitalWrite(TOUCH_RST, LOW);
  delay(1000);
  digitalWrite(TOUCH_RST, HIGH);
  delay(1000);
  
  touch_init();

  //Init Display
  gfx->begin();
  gfx->fillScreen(WHITE);
  gfx->setTextSize(4);
  gfx->setTextColor(BLACK);
  gfx->setCursor(180, 50);
  gfx->println(F("Makerfabs"));
  
  gfx->setTextSize(3);
  gfx->setCursor(30, 100);
  gfx->println(F("4.0inch TFT with Touch "));

  delay(1000);
  gfx->setCursor(0, 20);
  //gfx->println(F("RED"));
  gfx->fillScreen(RED);
  Serial.println("--RED--");
  delay(1000);
  //gfx->println(F("GREEN"));
  gfx->fillScreen(GREEN);
  Serial.println("--GREEN--");
  delay(1000);
  //gfx->println(F("BLUE"));
  gfx->fillScreen(BLUE);
  Serial.println("--BLUE--");
  delay(1000);
  //gfx->println(F("WHITE"));
  gfx->fillScreen(WHITE);
  Serial.println("--WHITE--");
  delay(1000);
  gfx->fillScreen(BLACK);
  Serial.println("--BLACK--");
  delay(1000);

#if 1
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  if (!SD.begin(SD_CS))
  {
    Serial.println(F("ERROR: SD Mount Failed!"));
    //while(1)
    {
      gfx->fillScreen(WHITE);
      gfx->setTextSize(3);
      gfx->setTextColor(RED);
      gfx->setCursor(50, 180);
      gfx->println(F("ERROR: SD Mount Failed!"));
      delay(3000);
    }
  }
  else
  {
    unsigned long start = millis();
    jpegDraw(JPEG_FILENAME_LOGO, jpegDrawCallback, true /* useBigEndian */,
             0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
    Serial.printf("Time used: %lums\n", millis() - start);
  }

  delay(1000);
  Serial.println("ESP32S3 4.0inch LCD V1.0");

  gfx->fillScreen(WHITE);
  gfx->fillRect(200, 160, 260, 160, BLACK);

  gfx->setTextSize(4);
  gfx->setTextColor(WHITE);
  gfx->setCursor(200 + 20, 180);
  gfx->println("3 * POINTS ");
  gfx->setCursor(200 + 20, 220);
  gfx->println("TOUCH TO");
  gfx->setCursor(200 + 20, 260);
  gfx->println("CONTINUE");

  jpegDraw(JPEG_FILENAME_07, jpegDrawCallback, true /* useBigEndian */,
           0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
     
  delay(500);

  while (1)
  {
    ts.read();
    if (ts.isTouched) {
      for (int i = 0; i < ts.touches; i++) {
        //gfx->fillScreen(WHITE);
        int temp = random(0, 6);
        gfx->fillScreen(ColorArray[temp]);

        gfx->setTextSize(4);
        //if(num) gfx->setTextColor(BLACK);
        //else
        if (temp == 4)
          gfx->setTextColor(WHITE);
        else
          gfx->setTextColor(RED);
        gfx->setCursor(320, 400);
        gfx->print("X: ");
        gfx->println(String(ts.points[i].x));
        gfx->setCursor(320, 440);
        gfx->print("Y: ");
        gfx->println(String(ts.points[i].y));


        Serial.print("Touch "); Serial.print(i + 1); Serial.print(": ");;
        Serial.print("  x: "); Serial.print(ts.points[i].x);
        Serial.print("  y: "); Serial.print(ts.points[i].y);
        Serial.print("  size: "); Serial.println(ts.points[i].size);
        Serial.println(' ');

      }

      ts.isTouched = false;
      if (ts.touches > 2) break;
    }
    delay(100);
  }
#endif

  unsigned long start = millis();
  jpegDraw(JPEG_FILENAME_01, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
  Serial.printf("Time used: %lums\n", millis() - start);
  delay(1000);    
  start = millis();
  jpegDraw(JPEG_FILENAME_02, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
  Serial.printf("Time used: %lums\n", millis() - start);
  delay(1000);  
  start = millis();
  jpegDraw(JPEG_FILENAME_03, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
  Serial.printf("Time used: %lums\n", millis() - start);
  delay(1000);  
  start = millis();
  jpegDraw(JPEG_FILENAME_04, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
  Serial.printf("Time used: %lums\n", millis() - start);
  delay(1000);  
  start = millis();
  jpegDraw(JPEG_FILENAME_05, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
  Serial.printf("Time used: %lums\n", millis() - start);
  delay(1000);  
  start = millis();
  jpegDraw(JPEG_FILENAME_06, jpegDrawCallback, true /* useBigEndian */,
               0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);
  Serial.printf("Time used: %lums\n", millis() - start);

  
/**  
  for(int i=1;i++;i<7)
  {
    imageDisplay(i);
    delay(1000);
  }
  delay(2000);
**/
  audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
  audio.setVolume(21); // 0...21
  audio.connecttoFS(SD, AUDIO_FILENAME_01);  //"/ChildhoodMemory.mp3"
  
}

void loop()
{
#if 1
  audio.loop();
  if (Serial.available()) { // put streamURL in serial monitor
    audio.stopSong();
    String r = Serial.readString(); r.trim();
    if (r.length() > 5) audio.connecttoFS(SD, r.c_str());
    log_i("free heap=%i", ESP.getFreeHeap());
    //gfx->fillScreen(GREEN);
  }
#endif
/**
  if(touch_touched()==true)
  {
      int temp = random(0, 6);
      imageDisplay(temp);
  }
**/
/**
  ts.read();
  if (ts.isTouched) {
      int temp = random(0, 6);
      imageDisplay(temp);
      //ts.isTouched = false;
  }
**/   
}
