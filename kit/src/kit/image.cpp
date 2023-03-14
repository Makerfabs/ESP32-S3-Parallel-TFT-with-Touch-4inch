#include "image.h"

#define IMG_LIMIT 30

String img_list[IMG_LIMIT];
int img_num = 0;
int img_index = 0;

void img_init()
{
    // Read SD
    img_num = get_img_list(SD, "/", img_list, IMG_LIMIT);
    Serial.print("Music file count:");
    Serial.println(img_num);
    Serial.println("All music:");
    for (int i = 0; i < img_num; i++)
    {
        Serial.println(img_list[i]);
    }

    // img_display(img_list[img_index]);
    img_display(0, 0, 480, 480, img_list[img_index]);
}

int get_img_list(fs::FS &fs, const char *dirname, String *list, int length)
{
    Serial.printf("Listing directory: %s\n", dirname);
    int i = 0;

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return i;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return i;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
        }
        else
        {
            String temp = "/";
            temp += file.name();
            if (temp.endsWith(".jpg"))
            {
                list[i] = temp;
                i++;
            }
            if (i >= IMG_LIMIT)
                return i;
        }
        file = root.openNextFile();
    }
    return i;
}

void img_display(int x, int y, int w, int h, String img_name)
{
    jpegDraw(img_name.c_str(), jpegDrawCallback, true, x, y, w, h);
}

void img_next()
{
    img_index++;
    if (img_index >= img_num)
    {
        img_index = 0;
    }
    img_display(0, 0, 480, 480, img_list[img_index]);
}

int jpegDrawCallback(JPEGDRAW *pDraw)
{
    // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}