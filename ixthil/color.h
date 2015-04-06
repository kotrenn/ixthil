#ifndef __COLOR_H
#define __COLOR_H

#include "surface.h"

#define COLOR_RED    0
#define COLOR_YELLOW 1
#define COLOR_GREEN  2
#define COLOR_CYAN   3
#define COLOR_BLUE   4
#define COLOR_PURPLE 5
#define COLOR_BLANK -1

Uint32 mapColor(int color, Uint8 alpha = 255);
int color_name(const char *color);
bool color_match(int color1, int color2);

#endif
