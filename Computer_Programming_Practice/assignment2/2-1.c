//1번문제

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	double data;
	printf("실수를 입력하시오: ");
	scanf("%lf", &data);
	printf("실수형식: %f\n", data);
	printf("지수형식: %e\n", data);

	return 0;
}