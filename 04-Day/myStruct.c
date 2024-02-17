#include <stdio.h>

struct point
{
	int x;
	int y;
};

struct rect
{
	struct point pt1;
	struct point pt2;
};

/* makepoint: make a point from x and y components */
struct point makepoint(int x, int y)
{
	struct point temp;

	temp.x = x;
	temp.y = y;

	return temp;
};

struct point addpoint(struct point p1, struct point p2)
{
	p1.x += p2.x;
	p1.y += p2.y;

	return p1;
};

int main()
{
	struct point screen;
	screen.x = 9;
	printf(">> screen.x: %d\n", screen.x);

	printf(">> makepoint: %p\n", makepoint(6, 5));

	printf(">> addpoint: %p\n", addpoint(makepoint(5,6), 
                                        makepoint(5, 7)));
	
}
