#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

void oledInit(void);
void drawPixel(int x, int y);
void clearDisplay(void);

#endif
