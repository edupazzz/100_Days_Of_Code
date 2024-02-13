/* Count digits (numbers), white space, others */

/* EXPLAINING THE CODE BELOW:
 * if(c >= '0' && c <= '9')
 * for example, int c = '7' (integer c receives the char '7')
 * The char '7' is not equal to integer 7, we need to look at ascii table:
 * Char '7' is equal to Decimal 55, and Char '0' is equal to Decimal 48,
 * so the operation if(c >= '0' && c <= '9') is equal to if(55 >= 48 && 55 <= 57)
 * then ++ndigit[c-'0']; -is equal to ++ndigit[55-48]; 
 * The operation 55-48 is equal to 7! 
 * By this way, we turn the char '7' into integer 7.
 * And it's true to all the digits from 0 to 9!
 * MAN ASCII:
 * https://man7.org/linux/man-pages/man7/ascii.7.html
*/

/* Usage (on command line): getOccurrencesOfDigits.exe < file.txt */
#include <stdio.h>

/* count digits (numbers), white space, others*/
int main()
{
	/* Variables Declaration */
	int c, i, nwhite, nother;
	int ndigit[10];
	
	/* Variables Definition */
	nwhite = nother = 0;	
	for(int i=0; i<10; i++)
	{
		ndigit[i] = 0; /* fulfilling the array with zeros */
	}
	
	/* Read file, character by character until the End Of File terminator */
	while((c = getchar()) != EOF)
	{
		/* See if the character is a digit (number) */
		if(c >= '0' && c <= '9'){ // Explanation on the top
			++ndigit[c-'0']; // if it's a digit c-'0' position in the array count's by one
		}else if(c == ' ' || c == '\n' || c == '\t'){ 
			++nwhite; // Count for white spaces, new lines and vertical tab's
		}else{
			++nother; // Count for any other type of character (example 'a', 'b'...)
		}
	}
		
		/* Here we print the all collected data*/
		printf("digits =");
		for(i = 0; i<10; ++i)
			printf(" [%d]", ndigit[i]);
		
		printf(", white space = %d, other = %d\n", nwhite, nother);
}