#include "cam.h"
#include "color.h"

#define PRINT_TEMPERATURES
// #define PRINT_ASCIIART

Adafruit_MLX90640 mlx;

float frame[32 * 24];
float *temp_frame = NULL;
uint16_t *inter_p = NULL;

#define OUTPUT_W 480
#define OUTPUT_H 320

int mlx_init()
{

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    byte error, address;

    Wire.beginTransmission(MLX_I2C_ADDR);
    error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.print(F("I2C device found at address 0x"));
        Serial.print(MLX_I2C_ADDR, HEX);
        Serial.println(F("  !"));
    }
    else if (error == 4)
    {
        Serial.print(F("Unknown error at address 0x"));
        Serial.println(MLX_I2C_ADDR, HEX);

        return 0;
    }

    if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire))
    {
        Serial.println(F("MLX90640 not found!"));
        return 0;
    }

    mlx.setMode(MLX90640_CHESS);
    mlx.setResolution(MLX90640_ADC_18BIT);
    mlx90640_resolution_t res = mlx.getResolution();
    mlx.setRefreshRate(MLX90640_16_HZ);
    mlx90640_refreshrate_t rate = mlx.getRefreshRate();
    Wire.setClock(1000000); // max 1 MHz

    return 1;
}

void mlx_serial()
{
    if (mlx.getFrame(frame) != 0)
    {
        Serial.println("Failed");
        return;
    }
    Serial.println();
    Serial.println();
    for (uint8_t h = 0; h < 24; h++)
    {
        for (uint8_t w = 0; w < 32; w++)
        {
            float t = frame[h * 32 + w];
#ifdef PRINT_TEMPERATURES
            // Serial.print(t, 1);
            // Serial.print(", ");

            Serial.print((int)t);
            Serial.print(", ");
#endif
#ifdef PRINT_ASCIIART
            char c = '&';
            if (t < 20)
                c = ' ';
            else if (t < 23)
                c = '.';
            else if (t < 25)
                c = '-';
            else if (t < 27)
                c = '*';
            else if (t < 29)
                c = '+';
            else if (t < 31)
                c = 'x';
            else if (t < 33)
                c = '%';
            else if (t < 35)
                c = '#';
            else if (t < 37)
                c = 'X';
            Serial.print(c);
#endif
        }
        Serial.println();
    }
}

void mlx_frame_malloc()
{
    temp_frame = (float *)malloc(32 * 24 * sizeof(float));
    inter_p = (uint16_t *)malloc(OUTPUT_W * OUTPUT_H * sizeof(uint16_t));
    if (inter_p == NULL)
    {
        Serial.println(F("inter_p Malloc error"));
    }

    for (int i = 0; i < 32 * 24; i++)
    {
        *(temp_frame + i) = 20.0;
    }

    for (int i = 0; i < OUTPUT_W * OUTPUT_H; i++)
    {
        *(inter_p + i) = 0x480F;
    }

    Serial.println(ESP.getFreeHeap());
}

float mlx_img()
{
    float max_temp = 0.0;

    if (mlx.getFrame(frame) != 0)
    {
        Serial.println(F("Get frame failed"));
        return 0.0;
    }

    // filter_frame(frame, temp_frame);

    mirror(frame);
    filter(frame, temp_frame);

    // Display with 32*24 pixel
    // for (uint8_t h = 0; h < 24; h++)
    // {
    //     for (uint8_t w = 0; w < 32; w++)
    //     {
    //         uint8_t colorIndex = map_f(temp_frame[h * 32 + w], MINTEMP, MAXTEMP);
    //         gfx->fillRect(10 * w, 10 * h, 10, 10, camColors[colorIndex]);
    //     }
    // }

    interpolation(temp_frame, inter_p);
    gfx->draw16bitRGBBitmap(0, 0, inter_p, OUTPUT_W, OUTPUT_H);

    qusort(frame, 0, 32 * 24 - 1);
    max_temp += frame[767];

    return max_temp;
}

// float to 0,255
int map_f(float in, float a, float b)
{
    if (in < a)
        return 0;

    if (in > b)
        return 255;

    return (int)(in - a) * 255 / (b - a);
}

// Quick sort
void qusort(float s[], int start, int end)
{
    int i, j;        // 定义变量为基本整型
    i = start;       // 将每组首个元素赋给i
    j = end;         // 将每组末尾元素赋给j
    s[0] = s[start]; // 设置基准值
    while (i < j)
    {
        while (i < j && s[0] < s[j])
            j--; // 位置左移
        if (i < j)
        {
            s[i] = s[j]; // 将s[j]放到s[i]的位置上
            i++;         // 位置右移
        }
        while (i < j && s[i] <= s[0])
            i++; // 位置左移
        if (i < j)
        {
            s[j] = s[i]; // 将大于基准值的s[j]放到s[i]位置
            j--;         // 位置左移
        }
    }
    s[i] = s[0]; // 将基准值放入指定位置
    if (start < i)
        qusort(s, start, j - 1); // 对分割出的部分递归调用qusort()函数
    if (i < end)
        qusort(s, j + 1, end);
}

// Filter temperature data and change camera direction
// void filter_frame(float *in, float *out)
// {
//     if (MLX_MIRROR == 1)
//     {
//         for (int i = 0; i < 32 * 24; i++)
//         {
//             if (FILTER_ENABLE == 1)
//                 out[i] = (out[i] + in[i]) / 2;
//             else
//                 out[i] = in[i];
//         }
//     }
//     else
//     {
//         for (int i = 0; i < 24; i++)
//             for (int j = 0; j < 32; j++)
//             {
//                 if (FILTER_ENABLE == 1)
//                     out[32 * i + 31 - j] = (out[32 * i + 31 - j] + in[32 * i + j]) / 2;
//                 else
//                     out[32 * i + 31 - j] = in[32 * i + j];
//             }
//     }
// }

void mirror(float *in)
{
    if (MLX_MIRROR == 1)
    {
        float temp = 0.0;
        for (int y = 0; y < 24; y++)
            for (int x = 0; x < 16; x++)
            {
                temp = in[x + 32 * y];
                in[x + 32 * y] = in[31 - x + 32 * y];
                in[31 - x + 32 * y] = temp;
            }
    }
}

void filter(float *in, float *out)
{
    for (int i = 0; i < 32 * 24; i++)
        if (FILTER_ENABLE == 1)
            out[i] = (out[i] + in[i]) / 2;
        else
            out[i] = in[i];
}

// Transform 32*24 to 320 * 240 pixel
void interpolation(float *data, uint16_t *out)
{

    // 把out当存储，把in的浮点温度转化成0~255
    for (uint8_t h = 0; h < 24; h++)
    {
        for (uint8_t w = 0; w < 32; w++)
        {
            out[h * 15 * 480 + w * 15] = map_f(data[h * 32 + w], MINTEMP, MAXTEMP);
        }
    }

    for (int h = 0; h < 320; h += 15)
    {
        for (int w = 1; w < 465; w += 15)
        {
            for (int i = 0; i < 14; i++)
            {
                out[h * 480 + w + i] = (out[h * 480 + w - 1] * (14 - i) + out[h * 480 + w + 14] * (i + 1)) / 15;
            }
        }
        for (int i = 0; i < 14; i++)
        {
            out[h * 480 + 466 + i] = out[h * 480 + 465];
        }
    }
    for (int w = 0; w < 480; w++)
    {
        for (int h = 1; h < 305; h += 15)
        {
            for (int i = 0; i < 14; i++)
            {
                out[(h + i) * 480 + w] = (out[(h - 1) * 480 + w] * (14 - i) + out[(h + 14) * 480 + w] * (i + 1)) / 15;
            }
        }
        for (int i = 0; i < 14; i++)
        {
            out[(306 + i) * 480 + w] = out[305 * 480 + w];
        }
    }
    for (int h = 0; h < 320; h++)
    {
        for (int w = 0; w < 480; w++)
        {
            out[h * 480 + w] = camColors[out[h * 480 + w]];
        }
    }
}