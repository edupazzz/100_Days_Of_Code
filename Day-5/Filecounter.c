/* Write a program to count blanks, tabs, and newlines. */

#include <stdio.h>

void main(void)
{
	char char_var;
	int line_count = 0, blanks_count = 0, tabs_count = 0;

	while((char_var = getchar()) != EOF)
	{
		/* Blanks Count*/ if(char_var == ' '){ printf("#Blanks Count: %d\n", blanks_count); ++blanks_count; }
		/* Lines Count */ if(char_var == '\n'){ printf("#Lines Count: %d\n", line_count); ++line_count; }
		/* Tabs Count */  if(char_var == '\t'){ printf("#Tabs Count: %d\n", tabs_count); ++tabs_count; }				
	}


}
