#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	double scale_x = 1.5;
	double scale_y = 1.2;
	
	char name[80];
	char color[80];
	double x, w, h, y1, y2, y3, y4;
	while (scanf("%s %s %lf %lf %lf %lf %lf %lf %lf",
	             name, color, &x, &w, &h, &y1, &y2, &y3, &y4) > 0)
	{
		x *= scale_x;
		w *= scale_x;
		h *= scale_y;
		y1 *= scale_y;
		y2 *= scale_y;
		y3 *= scale_y;
		y4 *= scale_y;
		printf("%s %s %.1lf %.1lf %.1lf %.1lf %.1lf %.1lf %.1lf\n",
		       name, color, x, w, h, y1, y2, y3, y4);
	}

	return 0;
}
