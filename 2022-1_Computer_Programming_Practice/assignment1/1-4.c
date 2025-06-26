#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	
	float f, c;

	printf("화씨값은 입력하시오: ");
	scanf("%f", &f);

	c = (f - 32) * 5 / 9;

	printf("섭씨값은 %f도입니다.\n", c);
	

	return 0;
}