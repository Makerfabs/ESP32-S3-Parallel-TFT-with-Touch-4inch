#ifndef IMAGE_H
#define IMAGE_H

#include "config.h"
#include "JpegFunc.h"

extern Arduino_ST7701_RGBPanel *gfx;

void img_init();
void img_next();
void img_display(String img_name);

int get_img_list(fs::FS &fs, const char *dirname, String *list, int length);
int jpegDrawCallback(JPEGDRAW *pDraw);

#endif