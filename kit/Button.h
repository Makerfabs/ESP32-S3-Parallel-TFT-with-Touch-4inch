#ifndef _Button_H_
#define _Button_H_

#include <arduino.h>

#define UNABLE -1
#define ENABLE 1
#define DEFAULT_TEXT_SIZE 2

// define a button class
class Button
{

private:
    // Button Position
    int b_x;
    int b_y;
    int b_w;
    int b_h;

    // Button text
    String text;
    int textSize;

    // Button value, default value = -1 is untouchable
    int value;

public:
    Button();
    Button(int x, int y, int w, int h, String text, int value, int textSize = DEFAULT_TEXT_SIZE);

    void set(int x, int y, int w, int h, String text, int value, int textSize = DEFAULT_TEXT_SIZE);
    void getFoDraw(int *x, int *y, int *w, int *h, String *text, int *textSize);

    void setText(String t);
    String getText();
    void setTextSize(int textSize);

    void setValue(int v);
    int getValue();

    int checkTouch(int x, int y);
};

#endif