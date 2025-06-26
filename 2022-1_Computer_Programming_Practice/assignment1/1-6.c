#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
	double weight, mweight;

	printf("몸무게를 입력하세요(단위 kg): ");
	scanf("%lf", &weight);
		
	mweight = weight * 17 / 100;
	printf("달에서의 몸무게는 %lf입니다\n", mweight);
	

	return 0;
}