#include "config.h"
#include "page.h"

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(1 /* CS */, 12 /* SCK */, 11 /* SDA */, 45 /* DE */, 4 /* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */, 39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */, 0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */, 6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    true /* IPS */, 480 /* width */, 480 /* height */,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations),
    true /* BGR */);

TAMC_GT911 ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

Audio audio;

int touch_flag = 0;
int touch_x = -1;
int touch_y = -1;

void setup()
{

    Serial.begin(115200);

    // TFT init
    gfx->begin();
    gfx->fillScreen(WHITE);

    // Touch init
    pinMode(TOUCH_RST, OUTPUT);
    delay(100);
    digitalWrite(TOUCH_RST, LOW);
    delay(500);
    digitalWrite(TOUCH_RST, HIGH);
    delay(500);
    digitalWrite(TOUCH_RST, LOW);
    delay(500);
    digitalWrite(TOUCH_RST, HIGH);
    delay(500);

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    ts.begin();
    ts.setRotation(TOUCH_ROTATION);

    // File init
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS))
    {
        Serial.println(F("ERROR: SD Mount Failed!"));
    }

    wifi_config_read();

    // 不能设为同优先级，不然可能跳过同级有限的任务
    // 需要有个0优先级的来刷新看门狗
    xTaskCreatePinnedToCore(Task_TFT, "Task_TFT", 20480, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(Task_Touch, "Task_Touch", 2048, NULL, 1, NULL, 0);
}

void loop()
{
}

void Task_TFT(void *pvParameters) // This is a task.
{
    while (1)
    {
        page_main();
        Serial.println(F("Task_TFT"));
        vTaskDelay(100);
    }
}

void Task_Touch(void *pvParameters) // This is a task.
{
    while (1)
    {
        ts.read();
        if (touch_flag == 0)
        {
            if (ts.isTouched)
            {
                for (int i = 0; i < ts.touches; i++)
                {
                    touch_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, 480 - 1);
                    touch_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, 480 - 1);

                    // Serial.print("Touch ");
                    // Serial.print(i + 1);
                    // Serial.print(": ");
                    // ;
                    // Serial.print("  x: ");
                    // Serial.print(ts.points[i].x);
                    // Serial.print("  y: ");
                    // Serial.print(ts.points[i].y);
                    // Serial.print("  size: ");
                    // Serial.println(ts.points[i].size);
                    // Serial.println(' ');
                }
                // 多读一次，清空脏数据，不过好像没啥用
                ts.read();
                ts.isTouched = false;
                touch_flag = 1;

                Serial.print("  touch_x: ");
                Serial.print(touch_x);
                Serial.print("  touch_y: ");
                Serial.println(touch_y);
            }
        }

        if (touch_flag == -1)
            vTaskDelete(NULL);

        vTaskDelay(100);
    }
}