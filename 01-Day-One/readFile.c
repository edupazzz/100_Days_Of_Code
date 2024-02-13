#include <stdio.h>
#include <stdlib.h>


/* Just read a file from the input 
 * Almost like the linux 'cat' command...
 */
 
 /* Usage (on command line): readFile.exe < File-Name.txt */
int main()
{
	int c; /* holds the current char */
	
	/* here we read char by char until the End Of File terminator */
	while((c=getchar()) != EOF)
	{
		putchar(c); /* here we print char by char */
	}
	
	return 0;
}