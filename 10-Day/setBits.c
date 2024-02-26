#include <stdio.h>
#include <stdint.h>

/* cat the 3th bit */
uint8_t cat_third_bit(int32_t n)
{
	return ((n >> 2) & 1);	
}

int main()
{
	printf(">> %hhu\n", cat_third_bit(~7));
}
