#include <stdio.h>

int main()
{
	int m, n, r, hm, hn;
	m = n = r = 0;
	
	
	printf("Insert m: ");
	scanf("%d", &m);
	
	printf("Insert n: ");
	scanf("%d", &n);
	
	hm = m;
	hn = n;
	
	while(1)
	{
	
	/*F0*/
		if(m < n)
		{
			printf("m need to be greater than n\nTerminating...\n");
			exit(0);
		}
	
	/*F1*/
		r = m%n;
	
	/*F2*/
		if(r == 0)
		{
			printf("The Greatest Comum Divisor of %d and %d is: %d\n", hm, hn, n);
			return 0;
		}
	
	/*F3*/
		m = n; n = r;
	}
}