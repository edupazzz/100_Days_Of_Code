#include <stdio.h>
#include <stdint.h>
#include <windows.h>


void main()
{
	WORD *t;
	
	t = (WORD *)malloc(16);
	
	GetSystemTime(t);
	
	printf("%04d-%02d-%02d %02d:%02d:%02d\n", t[0], t[1], t[3], t[4], t[5], t[6]);
	
	free(t);
}