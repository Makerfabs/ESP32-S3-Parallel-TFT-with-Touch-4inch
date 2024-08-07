### Library needed:

Just search in Arduino Library section: **AnimatedGIF** and install it.

### A big fan of Larry Bank work to simplify things in Arduino

Using “COOKED” pixels, it means that the library takes care of managing the palette conversion for your desired destination pixel type.
You just need to give it a buffer the size of the GIF canvas like this:

```C
uint8_t *buf = (uint8_t *)malloc(gif.getCanvasWidth() * gif.getCanvasHeight());
gif.setFrameBuf(buf);
gif.setDrawType(GIF_DRAW_COOKED);
```

### How to improve it even faster

Many things can be done. In my personal experience that would be making SD card as fast as possible.
But also improving GIF palette, making a good looking but small footprint file, as this GIFs tend to be bigger.
For example converting any crypto art MP4 to GIF it will be quite a big image and that might make things slow.
Another ideas:

- Use an external SD with MMC mode and 4 data pins (Not sure if there are enough free IOs)
- Maybe possible to store GIF in PSRAM that should be faster at 80 or even 120 Mhz than the uSD 

Thanks Larry for your help and guidance,
Martin Fasani
https://github.com/martinberlin

