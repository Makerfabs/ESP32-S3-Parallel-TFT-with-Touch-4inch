#ifndef CAM_H
#define CAM_H

#include "config.h"

#define MINTEMP 25
#define MAXTEMP 37
#define MLX_MIRROR 0 // Set 1 when the camera is facing the screen
#define FILTER_ENABLE 0

extern Arduino_ST7701_RGBPanel *gfx;

int mlx_init();
void mlx_serial();
void mlx_frame_malloc();
float mlx_img();

void filter_frame(float *in, float *out);
void qusort(float s[], int start, int end);
int map_f(float in, float a, float b);
void interpolation(float *data, uint16_t *out);

void mirror(float *in);
void filter(float *in, float *out);

#endif
