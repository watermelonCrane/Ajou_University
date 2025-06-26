#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	double i, j, k;
	double sum;
	double ave;


	printf("첫번째 실수를 입력하시오: ");
	scanf("%lf", &i);
	printf("두번째 실수를 입력하시오: ");
	scanf("%lf", &j);
	printf("세번째 실수를 입력하시오: ");
	scanf("%lf", &k);

	sum = i + j + k;
	ave = sum / 3.0;

	printf("합계는 %f이고 평균은 %f입니다.", sum, ave);
	

	return 0;
}