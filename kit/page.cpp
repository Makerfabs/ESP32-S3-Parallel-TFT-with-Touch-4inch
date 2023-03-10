#include "page.h"

int touch_x_temp = 0;
int touch_y_temp = 0;

// Pages
void page_main()
{
    Serial.println(F("Main page"));

    gfx->fillScreen(COLOR_BACKGROUND);
    gfx->setCursor(20, 20);
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(4);
    gfx->println("Main page");

    Button b[B_NUM_MAIN];
    String b_name[] = {"A", "B", "C", "D"};

    b[0].set(40, 200, 160, 80, "xiangce", ENABLE);
    b[1].set(280, 200, 160, 80, "weather", ENABLE);
    b[2].set(40, 320, 160, 80, "sensor", ENABLE);
    b[3].set(280, 320, 160, 80, "rechengxiang", ENABLE);

    drawButton(b[0]);
    drawButton(b[1]);
    drawButton(b[2]);
    drawButton(b[3]);

    while (1)
    {

        if (touch_flag == 1)
        {
            // 防止按键检测期间被修改
            touch_x_temp = touch_x;
            touch_y_temp = touch_y;

            if (b[0].checkTouch(touch_x_temp, touch_y_temp) != UNABLE)
            {
                touch_flag = 0;
                page_1();
            }
            if (b[1].checkTouch(touch_x_temp, touch_y_temp) != UNABLE)
            {
                touch_flag = 0;
                page_2();
            }
            if (b[2].checkTouch(touch_x_temp, touch_y_temp) != UNABLE)
            {
                touch_flag = 0;
                page_3();
            }
            if (b[3].checkTouch(touch_x_temp, touch_y_temp) != UNABLE)
            {
                touch_flag = 0;
                page_4();
            }
            touch_flag = 0;
        }
        vTaskDelay(10);
    }
}

// 电子相册
void page_1()
{
    Serial.println(F("1 page"));

    Button b_mute;
    Button b_next;

    b_mute.set(20, 420, 80, 40, "Mute", ENABLE);
    b_next.set(120, 420, 80, 40, "Next", ENABLE);

    gfx->fillScreen(BLACK);

    img_init();
    drawButton(b_mute);
    drawButton(b_next);

    music_init();

    vTaskDelay(500);

    long runtime_1 = 0;

    while (1)
    {
        if ((millis() - runtime_1) > 3000)
        {
            img_next();
            drawButton(b_mute);
            drawButton(b_next);
            runtime_1 = millis();
        }

        if (touch_flag == 1)
        {
            touch_x_temp = touch_x;
            touch_y_temp = touch_y;

            if (b_mute.checkTouch(touch_x_temp, touch_y_temp) != UNABLE)
            {
                vTaskDelay(200);
                Serial.println(F("Mute"));
                music_mute();
            }

            if (b_next.checkTouch(touch_x_temp, touch_y_temp) != UNABLE)
            {
                vTaskDelay(200);
                Serial.println(F("Next"));
                next_song();
            }
            touch_flag = 0;
        }
        vTaskDelay(100);
    }
}

// 时钟+天气
void page_2()
{
    Serial.println(F("2 page"));

    Button b_countrys[B_COUNTY_NUM];
    int country_index = 0;

    for (int i = 0; i < B_COUNTY_NUM; i++)
    {
        b_countrys[i].set(20 + 110 * (i % (B_COUNTY_NUM / 2)), 330 + 70 * (i / (B_COUNTY_NUM / 2)), 100, 60, flag_list[i], ENABLE);
    }

    gfx->fillScreen(COLOR_BACKGROUND);

    for (int i = 0; i < B_COUNTY_NUM; i++)
        drawFlag(b_countrys[i]);

    if (wifi_init() != 1)
    {
        gfx->setTextColor(COLOR_TEXT);
        gfx->setTextSize(2);

        gfx->setCursor(0, 0);
        gfx->println("Can't connect your wifi.");
        gfx->println("Please check wifi.txt in SD card.");
        gfx->println("Your ssid:");
        gfx->println(ssid);
        gfx->println("Your password:");
        gfx->println(pwd);

        while (1)
            vTaskDelay(100);
    }

    time_zone_set(country_index);

    weather_display(20, 120, country_index);

    long runtime_1 = 0;
    touch_flag = 0;

    while (1)
    {
        if ((millis() - runtime_1) > 1000)
        {
            time_display(20, 20, country_index);
            runtime_1 = millis();
        }

        if (touch_flag == 1)
        {
            touch_x_temp = touch_x;
            touch_y_temp = touch_y;

            for (int i = 0; i < B_COUNTY_NUM; i++)
                if (b_countrys[i].checkTouch(touch_x_temp, touch_y_temp) != UNABLE)
                {
                    vTaskDelay(200);
                    Serial.println(i);
                    country_index = i;
                    time_zone_set(i);

                    time_display(20, 20, country_index);
                    weather_display(20, 120, i);
                }

            touch_flag = 0;
        }
        vTaskDelay(100);
    }
}

// 传感器x2
void page_3()
{
    Serial.println(F("3 page"));

    tvoc_task();
}

// 热成像
void page_4()
{
    float max_temp = 0.0;
    String temp = "";
    // 关闭触屏抗干扰
    touch_flag = -1;
    vTaskDelay(1000);

    Serial.println(F("4 page"));
    mlx_init();
    mlx_frame_malloc();

    gfx->fillScreen(BLACK);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(4);

    while (1)
    {
        // mlx_serial();
        max_temp = mlx_img();

        temp = "";
        temp = temp + "Max temp:" + max_temp;

        gfx->fillRect(0, 321, 480, 79, BLACK);

        gfx->setCursor(20, 340);
        gfx->println(temp);

        vTaskDelay(100);
    }
}

// Normal function
void time_display(int x, int y, int country_num)
{
    struct tm timeinfo;

    gfx->fillRect(x, y, 460, 80, COLOR_BACKGROUND);
    // gfx->drawRect(x, y, 420, 80, COLOR_LINE);
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(4);

    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        gfx->setCursor(x, y + 10);
        gfx->println("NTC initing....");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

    char time_temp[20];

    sprintf(time_temp, "%02d/%02d/%02d %s",
            timeinfo.tm_year - 100, timeinfo.tm_mon, timeinfo.tm_mday, city_list[country_num]);
    gfx->setCursor(x, y + 10);
    gfx->println(time_temp);

    sprintf(time_temp, "%02d:%02d:%02d UTC %d",
            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, zone_list[country_num]);
    gfx->setCursor(x, y + 50);
    gfx->println(time_temp);
}

void weather_display(int x, int y, int country_num)
{
    float temperature = 0.0;
    float humidty = 0.0;
    String weather = "NULL";
    int weather_num = 0;

    gfx->fillRect(x, y, 420, 160, COLOR_BACKGROUND);
    gfx->drawRect(x, y, 420, 160, COLOR_LINE);

    weather_request2(country_num, &temperature, &humidty, &weather, &weather_num);

    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(3);

    String temp = "";

    temp = temp + "Temp:" + temperature;
    gfx->setCursor(x + 10, y + 20);
    gfx->println(temp);

    temp = "";
    temp = temp + "Humi:" + humidty;
    gfx->setCursor(x + 10, y + 60);
    gfx->println(temp);

    gfx->setCursor(x + 10, y + 100);
    gfx->println(weather);

    temp = "";
    temp = temp + "/weather/" + weather_num + ".jpg";

    img_display(x + 280, y + 20, 120, 120, temp);
}

// Other function
void drawButton(Button b)
{
    int b_x;
    int b_y;
    int b_w;
    int b_h;
    int shadow_len = 4;
    String text;
    int textSize;

    b.getFoDraw(&b_x, &b_y, &b_w, &b_h, &text, &textSize);

    gfx->fillRect(b_x, b_y, b_w, b_h, COLOR_BUTTON);
    gfx->drawRect(b_x, b_y, b_w, b_h, COLOR_LINE);
    gfx->setCursor(b_x + 20, b_y + 20);
    gfx->setTextColor(COLOR_TEXT);
    gfx->setTextSize(textSize);
    gfx->println(text);

    // Add button shadow
    if (b.getValue() != UNABLE)
    {
        gfx->fillRect(b_x + shadow_len, b_y + b_h, b_w, shadow_len, COLOR_SHADOW);
        gfx->fillRect(b_x + b_w, b_y + shadow_len, shadow_len, b_h, COLOR_SHADOW);
    }
}

void drawFlag(Button b)
{
    int b_x;
    int b_y;
    int b_w;
    int b_h;
    String text;
    int textSize;

    b.getFoDraw(&b_x, &b_y, &b_w, &b_h, &text, &textSize);

    gfx->fillRect(b_x, b_y, b_w, b_h, BLACK);
    img_display(b_x, b_y, b_w, b_h, text);
}