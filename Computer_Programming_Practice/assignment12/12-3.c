//3¹ø¹®Á¦
#include <stdio.h>

#define GET_BIT(n, pos) (((n) >> (pos)) & 0x01)
#define SET_BIT(n, pos) ((n) |= (0x01 << (pos)))
#define CLR_BIT(n, pos) ((n) ^= (0x01 << (pos)))

int main() {
	int n = 0x808081;

	printf("n = %#x\n", n);
	printf("GET_BIT(n, 0): %d\n", GET_BIT(n, 0));
	
	SET_BIT(n, 2);
	printf("SET_BIT(n, 2)\n");
	printf("n = %#x\n", n);

	//printf("CLR_BIT(n, 2): %#x\n", CLR_BIT(n, 2));

	return 0;
}