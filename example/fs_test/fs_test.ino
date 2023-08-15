#include <Arduino_GFX_Library.h>
#include "FS.h"
#include "SPIFFS.h"

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

#define TFT_BL 38

#define IMAGE_COUNT 6
String image_list[IMAGE_COUNT] = {

    "/img1.jpg",
    "/img2.jpg",
    "/img3.jpg",
    "/img4.jpg",
    "/img5.jpg",
    "/img6.jpg"};

int image_index = 0;

// 4 inch
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    1 /* CS */, 12 /* SCK */, 11 /* SDA */,
    45 /* DE */, 4 /* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */,

    6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */,
    0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */,
    39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */
);

Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    true /* IPS */, 480 /* width */, 480 /* height */,
    st7701_type5_init_operations, sizeof(st7701_type5_init_operations),
    true /* BGR */);

// // 4 inch
// Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
//     1 /* CS */, 12 /* SCK */, 11 /* SDA */,
//     45 /* DE */, 4 /* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */,
//     39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */,
//     0 /* G0/P22 */, 9 /* G1/P23 */, 14 /* G2/P24 */, 47 /* G3/P25 */, 48 /* G4/P26 */, 3 /* G5 */,
//     6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */
// );

// Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
//     bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
//     true /* IPS */, 480 /* width */, 480 /* height */,
//     st7701_type1_init_operations, sizeof(st7701_type1_init_operations),
//     true /* BGR */);

// 2.1
// Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
//     1 /* CS */, 46 /* SCK */, 0 /* SDA */,
//     2 /* DE */, 42 /* VSYNC */, 3 /* HSYNC */, 45 /* PCLK */,
//     11 /* R0 */, 15 /* R1 */, 12 /* R2 */, 16 /* R3 */, 21 /* R4 */,
//     39 /* G0/P22 */, 7 /* G1/P23 */, 47 /* G2/P24 */, 8 /* G3/P25 */, 48 /* G4/P26 */, 9 /* G5 */,
//     4 /* B0 */, 41 /* B1 */, 5 /* B2 */, 40 /* B3 */, 6 /* B4 */
// );

// option 1:
// Uncomment for 4" rect display
// Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
//     bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
//     true /* IPS */, 480 /* width */, 480 /* height */,
//     st7701_type1_init_operations, sizeof(st7701_type1_init_operations),
//     true /* BGR */,
//     10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
//     10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);
// option 2:
// Uncomment for 2.1" round display
// Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
//     bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
//     false /* IPS */, 480 /* width */, 480 /* height */,
//     st7701_type5_init_operations, sizeof(st7701_type5_init_operations),
//     true /* BGR */,
//     10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
//     10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);
// option 3:
// Uncomment for 2.8" round display
// Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
//     bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
//     false /* IPS */, 480 /* width */, 480 /* height */,
//     st7701_type6_init_operations, sizeof(st7701_type6_init_operations),
//     true /* BGR */,
//     22 /* hsync_front_porch */, 3 /* hsync_pulse_width */, 45 /* hsync_back_porch */,
//     4 /* vsync_front_porch */, 12 /* vsync_pulse_width */, 40 /* vsync_back_porch */);

#include "JpegFunc.h"

// pixel drawing callback
static int jpegDrawCallback(JPEGDRAW *pDraw)
{
    // USBSerial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    USBSerial.begin(115200);

    gfx->begin();
    gfx->fillScreen(WHITE);

    gfx->fillRect(150, 360, 50, 100, RED);
    gfx->fillRect(200, 360, 50, 100, YELLOW);
    gfx->fillRect(250, 360, 50, 100, GREEN);
    gfx->fillRect(300, 360, 50, 100, BLUE);
    delay(5000);

    // listDir(SPIFFS, "/", 0);
    // writeFile(SPIFFS, "/hello.txt", "Hello ");
    // appendFile(SPIFFS, "/hello.txt", "World!\r\n");
    // readFile(SPIFFS, "/hello.txt");
    // renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    // readFile(SPIFFS, "/foo.txt");
    // deleteFile(SPIFFS, "/foo.txt");
    // testFileIO(SPIFFS, "/test.txt");
    // deleteFile(SPIFFS, "/test.txt");
    // USBSerial.println("Test complete");

    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        USBSerial.println("SPIFFS Mount Failed");
        return;
    }
    writeFile(SPIFFS, "/spiffs.txt", "Begin");
}

// void loop()
// {
//     listDir(SPIFFS, "/", 0);
//     delay(3000);
// }

void loop()
{
    listDir(SPIFFS, "/", 0);

    String str = "";
    str = str + image_list[image_index++ % IMAGE_COUNT];

    appendFile(SPIFFS, "/spiffs.txt", str.c_str());
    readFile(SPIFFS, "/spiffs.txt");

    jpegDraw(str.c_str(), jpegDrawCallback, true /* useBigEndian */,
             0,
             0,
             480 /* widthLimit */, 480 /* heightLimit */);

    delay(3000);
}

// SPIFFS

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    USBSerial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        USBSerial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        USBSerial.println("- file written");
    }
    else
    {
        USBSerial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
    USBSerial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        USBSerial.println("- failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        USBSerial.println("- message appended");
    }
    else
    {
        USBSerial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    USBSerial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        USBSerial.println("- file renamed");
    }
    else
    {
        USBSerial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char *path)
{
    USBSerial.printf("Deleting file: %s\r\n", path);
    if (fs.remove(path))
    {
        USBSerial.println("- file deleted");
    }
    else
    {
        USBSerial.println("- delete failed");
    }
}

void testFileIO(fs::FS &fs, const char *path)
{
    USBSerial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        USBSerial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    USBSerial.print("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
        {
            USBSerial.print(".");
        }
        file.write(buf, 512);
    }
    USBSerial.println("");
    uint32_t end = millis() - start;
    USBSerial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if (file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        USBSerial.print("- reading");
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
            {
                USBSerial.print(".");
            }
            len -= toRead;
        }
        USBSerial.println("");
        end = millis() - start;
        USBSerial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    }
    else
    {
        USBSerial.println("- failed to open file for reading");
    }
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    USBSerial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        USBSerial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        USBSerial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            USBSerial.print("  DIR : ");
            USBSerial.println(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            USBSerial.print("  FILE: ");
            USBSerial.print(file.name());
            USBSerial.print("\tSIZE: ");
            USBSerial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char *path)
{
    USBSerial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        USBSerial.println("- failed to open file for reading");
        return;
    }

    USBSerial.println("- read from file:");
    while (file.available())
    {
        USBSerial.write(file.read());
    }
    file.close();
}
