#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	double base, height, area;
	
	printf("»ï°¢ÇüÀÇ ¹Øº¯: ");
	scanf("%lf", &base);
	printf("»ï°¢ÇüÀÇ ³ôÀÌ: ");
	scanf("%lf", &height);	
	
	area = base * height / 2;
	printf("»ï°¢ÇüÀÇ ³ĞÀÌ: %lf\n", area);
	
	return 0;
}