#include "color.h"

Uint32 mapColor(int color, Uint8 alpha)
{
	Uint32 ret;
	
	switch(color)
	{
	case COLOR_RED:    ret = mapRGBA(255, 0,   0,   alpha); break;
	case COLOR_YELLOW: ret = mapRGBA(255, 255, 0,   alpha); break;
	case COLOR_GREEN:  ret = mapRGBA(0,   255, 0,   alpha); break;
	case COLOR_CYAN:   ret = mapRGBA(0,   255, 255, alpha); break;
	case COLOR_BLUE:   ret = mapRGBA(0,   0,   255, alpha); break;
	case COLOR_PURPLE: ret = mapRGBA(255, 0,   255, alpha); break;
	default: ret = mapRGBA(255, 255, 255, alpha);
	}
	
	return ret;
}

int color_name(const char *color)
{
	int ret = -1;
	if      (strcmp(color, "red") == 0)    ret = 0;
	else if (strcmp(color, "yellow") == 0) ret = 1;
	else if (strcmp(color, "green") == 0)  ret = 2;
	else if (strcmp(color, "cyan") == 0)   ret = 3;
	else if (strcmp(color, "blue") == 0)   ret = 4;
	else if (strcmp(color, "purple") == 0) ret = 5;
	return ret;
}

bool color_match(int color1, int color2)
{
	if (color1 == color2)
		return true;
	
	if (color1 == COLOR_RED    && color2 == COLOR_CYAN)   return true;
	if (color1 == COLOR_CYAN   && color2 == COLOR_RED)    return true;
	if (color1 == COLOR_YELLOW && color2 == COLOR_BLUE)   return true;
	if (color1 == COLOR_BLUE   && color2 == COLOR_YELLOW) return true;
	if (color1 == COLOR_GREEN  && color2 == COLOR_PURPLE) return true;
	if (color1 == COLOR_PURPLE && color2 == COLOR_GREEN)  return true;
	return false;
}
