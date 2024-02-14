#include <stdio.h>
#include <stdlib.h>

/* Function Prototype */
void file_compare(FILE*, FILE*);


/* Main open the files, calls the file_compare() function, and close the files */
int main()
{
/*************************************/
/********** Open the files ***********/
	FILE* fp1 = fopen("file.txt", "r");
	FILE* fp2 = fopen("file2.txt", "r");
	
	/* checks for NULL files, if any exits */
	if (fp1 == NULL || fp2 == NULL) 
    { 
       printf("Error : Files not open"); 
       exit(0); 
    } 
	
	/* calling file_compare() function and pass the two files */
	file_compare(fp1, fp2);
	
	/* closing the files */
	fclose(fp1);
	fclose(fp2);
	
	return 0;
}


/***************************************/
/******** Comparing two files *********/
void file_compare(FILE* fp1, FILE* fp2)
{
	char c1 = getc(fp1); // Get the first char of first file 'fp1'
	char c2 = getc(fp2); // Get the first char of second file 'fp2'
	
	int diffc=0, // different chars counter
    	pos=0,   // position of different char found
		line=0;  // line of different char found
	
	/* Main looping that search for different chars */
	/* keeping looking until reach the End of File */
	while(c1 != EOF && c2 != EOF)
	{
		++pos; // position counter increment in each char read
		
		/* reaching a new line: increment line counter and reset position var */
		if(c1 == '\n' && c2 == '\n')
		{
			++line; // line counter
			pos=0; // reset position counter
		}
		
		/* if a char don't match: increment different char varible and print it */
		if(c1 != c2)
		{
			++diffc; // increment different char varible
			
			printf("Line Number: %d \tError Position: %d\n", line, pos); 
        }

		/* Move to the next character on both files */
		c1 = getc(fp1);
		c2 = getc(fp2);
	}
}