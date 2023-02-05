#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	double mile, km;

	printf("마일을 입력하시오: ");
	scanf("%lf", &mile);

	km = mile * 1.609;
	printf("%lf마일은 %lf킬로미터입니다.", mile, km);


	return 0;
}