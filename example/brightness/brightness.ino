#include <Arduino_GFX_Library.h>
#include <TAMC_GT911.h>

#define PWM_PIN 44
#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_INT -1
#define TOUCH_RST 38

#define TOUCH_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 0
#define TOUCH_MAP_X2 480
#define TOUCH_MAP_Y1 0
#define TOUCH_MAP_Y2 480

#define GFX_BL -1
#define TFT_BL GFX_BL

const int freq = 50000;
const int channel = 0;
const int resolution = 8;
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

int touch_last_x = 0, touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

void touch_init(void)
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  ts.begin();
}

void setup()
{
  USBSerial.begin(115200);
  
  pinMode(PWM_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);
  
  ledcSetup(channel, freq, resolution); // 设置通道
  ledcAttachPin(PWM_PIN, channel);  // 将通道与对应的引脚连接
  
  pinMode(TOUCH_RST, OUTPUT);
  delay(100);
  digitalWrite(TOUCH_RST, LOW);
  delay(1000);
  digitalWrite(TOUCH_RST, HIGH);
  delay(1000);
  
  #ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
  #endif
  
  digitalWrite(TOUCH_RST, LOW);
  delay(1000);
  digitalWrite(TOUCH_RST, HIGH);
  delay(1000);
  
  touch_init();

  //Init Display
  gfx->begin();
  gfx->fillScreen(BLACK);
  ledcWrite(channel, 240);
  //gfx->drawLine(100, 100, 400, 100, BLUE);
  gfx->drawRoundRect(100,200,300,40,40,WHITE);
  gfx->drawCircle(120, 220, 20, BLUE);
  gfx->fillCircle(120, 220, 20, BLUE);
  while (1)
  {
    ts.read();
    if (ts.isTouched) 
    {
      for (int i = 0; i < ts.touches; i++) 
      {
        if(abs(ts.points[i].y-480)>=150&&abs(ts.points[i].y-480)<=300)
        {
          if(abs(ts.points[i].x-480)>=100&&abs(ts.points[i].x-480)<=410)
          {
            gfx->fillScreen(BLACK);
            gfx->drawRoundRect(100,200,300,40,40,WHITE);
            gfx->fillRoundRect(100,200,abs(ts.points[i].x-380),40,40,BLUE);
            gfx->drawCircle(120, 220, 20, BLUE);
            gfx->fillCircle(120, 220, 20, BLUE);
            ledcWrite(channel, abs(ts.points[i].x)-130);
          }
          else if(abs(ts.points[i].x-480)>=410)
          {
            gfx->fillScreen(BLACK);
            gfx->drawRoundRect(100,200,300,40,40,WHITE);
            gfx->fillRoundRect(100,200,300,40,40,BLUE);
            gfx->drawCircle(120, 220, 20, BLUE);
            gfx->fillCircle(120, 220, 20, BLUE);
            ledcWrite(channel, 0);
          }
          else if(abs(ts.points[i].x-480)<=100)
          {
            gfx->fillScreen(BLACK);
            gfx->drawRoundRect(100,200,300,40,40,WHITE);
            gfx->drawCircle(120, 220, 20, BLUE);
            gfx->fillCircle(120, 220, 20, BLUE);
            ledcWrite(channel, 240);
          }
        }
        USBSerial.print("Touch "); USBSerial.print(i + 1); USBSerial.print(": ");;
        USBSerial.print("  x: "); USBSerial.print(abs(ts.points[i].x-480));
        USBSerial.print("  y: "); USBSerial.print(abs(ts.points[i].y-480));
        USBSerial.print("  size: "); USBSerial.println(ts.points[i].size);
        USBSerial.println(' ');
      }
      ts.isTouched = false;
      if (ts.touches > 2) break;
    }
    delay(100);
  }
}

void loop() 
{  
    //ledcWrite(channel, 0);//0--250-->亮--暗

}  
